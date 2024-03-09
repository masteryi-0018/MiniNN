cmake_minimum_required(VERSION 3.0)

# 1. 环境变量
set(TARGET MiniNN-test)
project(${TARGET})

# 2. 可执行文件
add_executable(${TARGET} test.cc)