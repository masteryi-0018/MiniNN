# 1. 删除之前构建结果
Remove-Item -Path .\build -Recurse -Force

# 2. 新建
New-Item -ItemType Directory -Path .\build | Out-Null
Set-Location .\build

# 3. cmake，-D变量=ON/OFF
cmake -D WITH_GTEST=ON ..

# 4. make
make

# 5.返回
Set-Location ..