# 1. 删除之前构建结果
rm -rf ./build

# 2. 新建
mkdir ./build
cd ./build

# 3. cmake
cmake ..

# 4. make
make

# 5.返回
cd ..