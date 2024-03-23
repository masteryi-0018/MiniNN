# 1. 删除之前构建结果
Remove-Item -Path .\build -Recurse -Force

# 2. 新建
New-Item -ItemType Directory -Path .\build | Out-Null
Set-Location .\build

# 3. cmake，-D变量=ON/OFF
cmake -D WITH_GTEST=ON ..
# cmake -D WITH_GTEST=ON .. -G "MSYS Makefiles"
# cmake -D WITH_GTEST=ON .. -G "MinGW Makefiles"
# cmake -D WITH_GTEST=ON .. -G Ninja

# 4. ninja or make
cmake --build .

# 5.返回
Set-Location ..