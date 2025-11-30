#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Load shader source from file
std::string loadShaderSource(const std::string& path) {
  std::ifstream file(path);
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

// Check OpenGL errors
void checkError(GLenum error, const char* msg) {
  if (error != GL_NO_ERROR) {
    std::cerr << msg << " - OpenGL Error: " << error << std::endl;
    exit(1);
  }
}

// Create compute shader program
GLuint createComputeProgram(const std::string& src) {
  GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
  const char* c_src = src.c_str();
  glShaderSource(shader, 1, &c_src, nullptr);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    GLint len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    std::vector<char> log(len);
    glGetShaderInfoLog(shader, len, nullptr, log.data());
    std::cerr << "Compute Shader Compile Error:\n" << log.data() << std::endl;
    exit(1);
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, shader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    GLint len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    std::vector<char> log(len);
    glGetProgramInfoLog(program, len, nullptr, log.data());
    std::cerr << "Program Link Error:\n" << log.data() << std::endl;
    exit(1);
  }

  glDeleteShader(shader);
  return program;
}

// Create and bind buffer object
GLuint createBuffer(const std::vector<float>& data, GLuint binding) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(float),
               data.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer);
  return buffer;
}

int main() {
  // Initialize GLFW + OpenGL
  if (!glfwInit()) {
    std::cerr << "Failed to init GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);  // hidden window
  GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW init failed\n";
    return -1;
  }

  // Load compute shader
  std::string shaderSrc = loadShaderSource("add.glsl");
  GLuint program = createComputeProgram(shaderSrc);
  glUseProgram(program);

  // Vector size
  const int size = 1024;
  std::vector<float> A(size, 1.0f);
  std::vector<float> B(size, 2.0f);
  std::vector<float> C(size, 0.0f);

  // Create buffers
  GLuint bufA = createBuffer(A, 0);
  GLuint bufB = createBuffer(B, 1);
  GLuint bufC = createBuffer(C, 2);

  // Dispatch compute shader
  const int localSize = 256;                           // must match shader
  int numGroups = (size + localSize - 1) / localSize;  // ceil division
  glDispatchCompute(numGroups, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // Read back results safely
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufC);
  glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size * sizeof(float),
                     C.data());

  // Print first 10 results
  std::cout << "Result of vector addition: ";
  for (int i = 0; i < 10; ++i) std::cout << C[i] << " ";
  std::cout << "..." << std::endl;

  // Cleanup
  glDeleteBuffers(1, &bufA);
  glDeleteBuffers(1, &bufB);
  glDeleteBuffers(1, &bufC);
  glDeleteProgram(program);
  glfwTerminate();

  return 0;
}