# 1. 删除之前构建结果
rm -rf ./build

# 2. 新建
mkdir ./build
cd ./build

# 3. cmake
cmake ..
# cmake .. -G Ninja
# cmake .. -G "MinGW Makefiles"
# cmake .. -G "Unix Makefiles"
# cmake .. -G "MSYS Makefiles"

# 4. make or ninja
cmake --build .

# 5.返回
cd ..

cp build/python/mininn.cpython-311-x86_64-linux-gnu.so python/test/mininn.so