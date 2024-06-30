# MiniNN

Build a deep learning inference framework from scratch

## download

```sh
# https
git clone --recursive https://github.com/masteryi-0018/MiniNN.git

# ssh
git clone --recursive git@github.com:masteryi-0018/MiniNN.git
```

## build(cmake)

### windows

1. power shell

> 使用mingw：gcc+ld

```ps1
.\build.ps1
```

如需修改生成器：

- ninja（默认）

```ps1
# choose one of follows, depend on you
cmake ..
cmake .. -G Ninja

# choose one of follows, depend on you
cmake --build .
ninja
```

- make

```ps1
# choose one of follows, depend on you
cmake .. -G "MinGW Makefiles"
cmake .. -G "Unix Makefiles"

# choose one of follows, depend on you
cmake --build .
make
mingw32-make
```

2. VS power shell中

> 使用msvc：cl+link

```ps1
.\build.ps1
```

如需修改生成器：

- ninja（默认）
  - 同上
- make
  - 同上
  - 注意：使用"Unix Makefiles"生成器时，编译器使用gcc
- nmake（msvc特有）

```ps1
cmake .. -G "NMake Makefiles"

# choose one of follows, depend on you
cmake --build .
nmake
```

总的来说就是多了一个nmake，剩下和mingw是一样的。不推荐msvc的编译器，有错误还未解决

3. msys2 ucrt shell

> 使用mingw：gcc+ld

```sh
./build.sh
```

如需修改生成器：

- ninja（默认）
  - 同上
- make
  - 同上，增加以下：

```sh
cmake .. -G "MSYS Makefiles"

# choose one of follows, depend on you
cmake --build .
make
```

只是多了"MSYS Makefiles"生成器，其他都一样，更接近Linux

### linux

1. Ubuntu 20.04

> 使用gnu：gcc+ld

```sh
./build.sh
```

如需修改生成器：

- make（默认）

```sh
# choose one of follows, depend on you
cmake ..
cmake .. -G "Unix Makefiles"

# choose one of follows, depend on you
cmake --build .
make
```

- ninja

```sh
cmake .. -G Ninja

# choose one of follows, depend on you
cmake --build .
ninja
```

Linux只支持"Unix Makefiles"和"Ninja"，没有那么多花里胡哨的

## build(bazel)

施工中...

## quick start

### windows

bugfix中...

### linux

```sh
./build/mininn/gtest-main
```

应该可以看到gtest全部通过