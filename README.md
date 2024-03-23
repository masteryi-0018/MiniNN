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

- ninja（默认）

```ps1
.\build.ps1
```

- make

```ps1
# choose one of follows, depend on you
cmake -D WITH_GTEST=ON .. -G "MSYS Makefiles"
cmake -D WITH_GTEST=ON .. -G "MinGW Makefiles"

# maybe mingw32-make, you can rename
make
```

2. linux

```sh
./build.sh
```