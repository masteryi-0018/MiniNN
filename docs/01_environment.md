# 环境问题

最理想的效果：一份代码，到处运行；不需要额外的环境开销

> Write once, run anywhere (WORA)

使用的环境：

1. Windows，vscode，git，msys2(cmake，make)
2. WSL2，Ubuntu，vscode，git，cmake，make

## Windows，x86

1. 目前使用cmake+make完成构建与编译

- 首先编写语法较为复杂的`CMakeLists.txt`文件
- 为了不让中间产物污染当前目录，新建`build`文件夹处理
- 带来问题是需要手动创建`build`，并执行`make`命令，于是需要写一个`shell`脚本
- Windows下运行shell依靠`git`，安装之后运行`sh ./build.sh`或者`bash ./build.sh`
- 没有的话也可以使用`powershell`，也写了一份

2. 编译出`.exe`文件之后运行也需要鼠标双击，给调试造成了不方便

- 不设置暂停对话框自动关闭，看不到结果
- 在Linux直接运行脚本方便，且可以看到结果

## 已知问题

1. Windows下执行`bash ./build.sh`编译产物为Linux平台下文件，执行`sh ./build.sh`正常