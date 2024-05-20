cmake_minimum_required(VERSION 3.0)
project(MININN)

# 一、环境变量
set(GTEST_BUILD third_party/googletest)
set(GFLAGS_BUILD third_party/gflags)
set(GLOG_BUILD third_party/glog)

# 二、子目录
add_subdirectory(third_party/googletest)
add_subdirectory(mininn)