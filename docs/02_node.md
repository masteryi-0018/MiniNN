# 节点

> node，op，算子，在这里都是一个意思

op是node的派生类，也就是对应不同的算子，node是不同op的抽象，在graph中表示拓扑结构。

## 抽象类node

1. 初始化，持有算子类型，例如add或者其他的枚举类型
2. 需要通过抽象node来创建kernel，所以创建kernel是父类的方法
3. 设置输入和输出，纯虚函数
4. 初始化kernel，纯虚函数

## 具体类op

1. 设置输入和输出，子类具体实现，因为不同的op的输入输出不相同
2. 初始化kernel，子类具体实现，因为不同的op的输入输出不相同

## op注册的实现

与kernel类似，op也需要有一个注册方式，就是拿到op_type，然后创建对应的节点。

首先需要有一个容器保存所有的op_type和op的对应关系，这个容器最好是**map类型**；之后需要有一个函数去把所有待注册的op添加进去，在使用时直接根据op_type创建op，不需要if-else或者switch-case的语句，这个函数最好是**静态方法**；最后需要一个类来包装静态方法，否则静态方法直接写在源文件中，未被调用有语法问题，这个类就是`OpRegistrar`，也就回答了为什么注册时需要有这个看似多余的东西。

在pdlite中，算子注册使用的是std::string作为map的第一个类型，调用时也是根据string来创建op，这里我修改为enum类型。