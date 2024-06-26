# 节点

> node，op，算子，在这里都是一个意思

节点需要考虑以下几个属性：

1. 算子类型。conv，add，或者其他的，最好使用枚举类型表示
2. 输入。用索引表示就好，graph使用索引从tensor中拿到对应张量
3. 输出。同上
4. 唯一标识符。可以是字符串，比较好理解，但是这里考虑索引，更加简单

考虑以下几个方法：

1. 初始化。
2. 准备。一些检查工作
3. 调用。实际的运行，这里很多框架都将这个抽象为单独的kernel了

## op注册

op是node的派生类，也就是对应不同的算子，node是不同op的抽象，在graph中表示拓扑结构。与kernel类似，op也需要有一个注册方式，就是拿到op_type，然后创建对应的节点。

## 实现

首先需要有一个容器保存所有的op_type和op的对应关系，这个容器最好是**map类型**；之后需要有一个函数去把所有待注册的op添加进去，在使用时直接根据op_type创建op，不需要if-else或者switch-case的语句，这个函数最好是**静态方法**；最后需要一个类来包装静态方法，否则静态方法直接写在源文件中，未被调用有语法问题，这个类就是`OpRegistrar`，也就回答了为什么注册时需要有这个看似多余的东西。

在pdlite中，算子注册使用的是std::string作为map的第一个类型，调用时也是根据string来创建op，这里我修改为enum类型。