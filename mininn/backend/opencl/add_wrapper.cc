#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include <cstring>  // strcpy
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const char* readKernelSource(std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
    std::cerr << "Unable to open file " << filename << std::endl;
    exit(1);
  }
  std::ostringstream ss;
  ss << file.rdbuf();
  std::string content = ss.str();
  char* source = new char[content.size() + 1];
  strcpy_s(source, content.size() + 1, content.c_str());
  return source;
}

void opencl_add_wrapper(float* h_A, float* h_B, float* h_C, int numElements) {
  size_t size = numElements * sizeof(float);

  // Set up OpenCL environment
  cl_platform_id platform;
  clGetPlatformIDs(1, &platform, nullptr);

  cl_device_id device;

#ifdef __linux__
  clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, nullptr);
#elif defined(_WIN32)
  clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
  // todo: use intel opencl in windows, error: SEH exception with code
  // 0xc0000005 thrown in the test body. clGetDeviceIDs(platform,
  // CL_DEVICE_TYPE_CPU, 1, &device, nullptr);
#endif

  cl_context context =
      clCreateContext(nullptr, 1, &device, nullptr, nullptr, nullptr);
  cl_command_queue queue =
      clCreateCommandQueueWithProperties(context, device, nullptr, nullptr);

  // Create buffers
  cl_mem d_A = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                              size, h_A, nullptr);
  cl_mem d_B = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                              size, h_B, nullptr);
  cl_mem d_C =
      clCreateBuffer(context, CL_MEM_WRITE_ONLY, size, nullptr, nullptr);

  // Read kernel source code
  // todo: why absolute path?
  std::string path = "mininn/backend/opencl/add.cl";
  const char* kernelSource = readKernelSource(path);

  // Create program
  cl_program program =
      clCreateProgramWithSource(context, 1, &kernelSource, nullptr, nullptr);
  clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
  // name "vectorAdd" must match the function name in opencl kernel
  cl_kernel kernel = clCreateKernel(program, "vectorAdd", nullptr);

  // Set kernel arguments
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_A);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_B);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_C);
  clSetKernelArg(kernel, 3, sizeof(unsigned int), &numElements);

  // Execute kernel
  size_t globalWorkSize = numElements;
  clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &globalWorkSize, nullptr, 0,
                         nullptr, nullptr);

  // Read results
  clEnqueueReadBuffer(queue, d_C, CL_TRUE, 0, size, h_C, 0, nullptr, nullptr);

  // Clean up resources
  clReleaseMemObject(d_A);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_C);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);
}