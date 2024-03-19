# MiniNN

Build a deep learning inference framework from scratch

## download

```sh
# https
git clone --recursive https://github.com/masteryi-0018/MiniNN.git

# ssh
git clone --recursive git@github.com:masteryi-0018/MiniNN.git
```

## build

1. windows

- ninja
```ps1
.\build.ps1
```

- make + msys2

```ps1
cmake -D WITH_GTEST=ON .. -G "MSYS Makefiles"
make
```

- make + mingw

```ps1
cmake -D WITH_GTEST=ON .. -G "MinGW Makefiles"
make
```

2. linux
```sh
./build.sh
```