#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Function to check OpenGL errors
void checkError(GLenum error, const char* msg) {
  if (error != GL_NO_ERROR) {
    std::cerr << msg << " - OpenGL Error: " << error << std::endl;
    exit(1);
  }
}

// Function to load a shader source from a file
std::string loadShaderSource(const std::string& path) {
  std::ifstream file(path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

// Function to initialize OpenGL
void initOpenGL() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!" << std::endl;
    exit(1);
  }

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "OpenGL Vector Add", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    std::cerr << "Failed to create GLFW window!" << std::endl;
    exit(1);
  }
  glfwMakeContextCurrent(window);
  glewInit();
  checkError(glGetError(), "GLEW Initialization failed");

  std::cout << "OpenGL Initialized!" << std::endl;
}

// Function to create and compile the compute shader program
GLuint createComputeShaderProgram(const std::string& shaderSource) {
  GLuint program = glCreateProgram();
  GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
  const char* shaderSrc = shaderSource.c_str();
  glShaderSource(shader, 1, &shaderSrc, nullptr);
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    char* log = new char[logLength];
    glGetShaderInfoLog(shader, logLength, nullptr, log);
    std::cerr << "Shader Compilation Failed:\n" << log << std::endl;
    delete[] log;
    exit(1);
  }

  glAttachShader(program, shader);
  glLinkProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    char* log = new char[logLength];
    glGetProgramInfoLog(program, logLength, nullptr, log);
    std::cerr << "Program Linking Failed:\n" << log << std::endl;
    delete[] log;
    exit(1);
  }

  glDeleteShader(shader);
  return program;
}

// Function to create a buffer object and bind it to the correct binding point
GLuint createBufferObject(const std::vector<float>& data, GLuint bindingPoint) {
  GLuint buffer;
  glGenBuffers(1, &buffer);  // Generate a buffer object
  glBindBuffer(
      GL_SHADER_STORAGE_BUFFER,
      buffer);  // Bind the buffer to the GL_SHADER_STORAGE_BUFFER target
  glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(float),
               data.data(), GL_STATIC_DRAW);  // Upload data to the buffer

  // Bind buffer to the specific binding point
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, buffer);
  checkError(glGetError(), "Buffer Creation Failed");

  return buffer;
}

int main() {
  initOpenGL();

  // Load shader source code
  std::string shaderSource = loadShaderSource("add.glsl");

  // Create and use the compute shader program
  GLuint program = createComputeShaderProgram(shaderSource);
  glUseProgram(program);

  // Initialize input data
  const int size = 1024;
  std::vector<float> A(size, 1.0f);  // Input vector A
  std::vector<float> B(size, 2.0f);  // Input vector B
  std::vector<float> C(size, 0.0f);  // Output vector C (initialized to zero)

  // Create buffer objects for the input and output vectors
  GLuint bufferA = createBufferObject(A, 0);  // A binding 0
  GLuint bufferB = createBufferObject(B, 1);  // B binding 1
  GLuint bufferC = createBufferObject(C, 2);  // C binding 2

  // Dispatch compute shader to process the data
  glDispatchCompute(size / 256, 1, 1);  // 4 work groups for 1024 elements
  glMemoryBarrier(
      GL_SHADER_STORAGE_BARRIER_BIT);  // Ensure completion of writes

  // Retrieve results from buffer C
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferC);
  float* ptr = (float*)glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, C.size() * sizeof(float), GL_MAP_READ_BIT);
  checkError(glGetError(), "Mapping Buffer Failed");

  // Output the first 10 results
  std::cout << "Result of vector addition: ";
  for (int i = 0; i < 10; i++) {  // Print first 10 results
    std::cout << ptr[i] << " ";
  }
  std::cout << "..." << std::endl;

  // Clean up resources
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glDeleteBuffers(1, &bufferA);
  glDeleteBuffers(1, &bufferB);
  glDeleteBuffers(1, &bufferC);
  glDeleteProgram(program);
  glfwTerminate();

  return 0;
}
