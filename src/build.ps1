# 1. 删除之前构建结果
Remove-Item -Path .\build -Recurse -Force

# 2. 新建
New-Item -ItemType Directory -Path .\build | Out-Null
Set-Location .\build

# 3. cmake，-D变量=ON/OFF
cmake ..
# cmake .. -G Ninja
# cmake .. -G "MinGW Makefiles"
# cmake .. -G "Unix Makefiles"
# cmake .. -G "NMake Makefiles"

# 4. ninja，make or nmake
cmake --build .

# 5.返回
Set-Location ..