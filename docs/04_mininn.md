# 一、tensor

> 一个项目首先从数据开始

张量是推理框架中很重要的概念，所有的数据都由张量类来维护。

1. 构造函数
2. 通过设置形状信息，分配内存
3. 获取形状信息，获取 buffer 地址与 buffer 长度
4. 设置数据 buffer 内容

# 二、node (op)

> node，op，算子，在这里都是一个意思

op 是 node 的派生类，也就是对应不同的算子，node 是不同 op 的抽象，在 graph 中表示拓扑结构。

## 抽象类 node

1. 构造函数，持有算子类型，例如 add 或者其他的枚举类型
2. 设置输入和输出，纯虚函数
3. 设置 op 属性，纯虚函数
4. 需要通过抽象 node 来创建 kernel，所以创建 kernel 是父类的方法
5. 初始化 kernel，纯虚函数

## 具体类 op

1. 设置输入和输出，子类具体实现，因为不同的 op 的输入输出不相同
2. 初始化 kernel，子类具体实现，因为不同的 op 的输入输出不相同

## op 注册的实现

与 kernel 类似，op 也需要有一个注册方式，就是拿到 op_type，然后创建对应的节点。

首先需要有一个容器保存所有的 op_type 和 op 的对应关系，这个容器最好是**map 类型**；之后需要有一个函数去把所有待注册的 op 添加进去，在使用时直接根据 op_type 创建 op，不需要 if-else 或者 switch-case 的语句，这个函数最好是**静态方法**；最后需要一个类来包装静态方法，否则静态方法直接写在源文件中，未被调用有语法问题，这个类就是`OpRegistrar`，也就回答了为什么注册时需要有这个看似多余的东西。

在 pdlite 中，算子注册使用的是 std::string 作为 map 的第一个类型，调用时也是根据 string 来创建 op，这里我修改为 enum 类型。

# 三、graph

> 很重要的概念：IR，也就是深度学习模型的代码表示，有向无环图

1. 构造函数
1. 添加 node，获取 node
1. 添加 tensor，获取 tensor
1. 设置模型输入，获取模型输入
1. 设置模型输出，获取模型输出

# 四、kernel

> 真正执行计算的对象

op_compute 是 kernel 的派生类，也就是对应不同的算子的具体计算实现，kernel 是 op_compute 的抽象。

graph 持有 node 和 tensor，能够完整的表示深度学习的网络模型。在真正推理的时候，一般是遍历 graph 中的 node，以 node 为单位按照拓扑顺序逐个计算每个节点的输出，如下：

1. 拿到整个图的输入，和第一个节点，算出第一个节点的输出
2. 拿到上一个节点的输出（也就是本节点的输入），和本节点，算出下一个节点的输出
3. 将最后一个节点的输出，作为整个图的输出

## kernel 类存在的必要性

1. 计算关系虽然是 node 为主导，但实际执行计算的时候，需要拿到 tensor 的信息，也就是 node 并不能单独完成计算任务，需要一个能够持有 node 和 tensor 的类完成计算，也就是 kernel。
2. 不同的 backend 对于同一个具体计算的表达是不同的，也就是 kernel 是与后端有关的，而 op（node）是与 kernel 无关的，所以无法在 node 类中实现不同后端的计算逻辑，也会让 node 负担更重。

## kernel 注册的实现

需求是：在遍历 graph 的时候，根据每个 op，创建对应的 kernel，这是需要从注册器中创建的。

实际在注册的时候，是将 op 与创建 kernel 的`函数/方法`成对放在了 map 中，并不是真的创建了一个 kernel，在准备阶段，使用注册器，根据 op 来创建 kernel，运行时直接执行 kernel 的方法就可以。

## 参考资料

1. tflite 的 kernel 都是 tflite cpu 或者 xnnpack 的，比较固定，所以先创建 kernel 再创建 op；与此同时，node 还需要能够找到对应的 kernel，在 tflite 中采用了`nodes_and_registration_`将 node 和 kernel 绑定。
2. pdlite 的 kernel 是多后端的，支持运行时指定后端，所以先创建 op 再创建 kernel；与此同时，在 pdlite 中 op 和 kernel 都有对应的注册方法，运行时 node 与 kernel 绑定，node 执行 infer shape 的功能，kernel 执行计算。

- <https://cloud.baidu.com/article/3275457>
- <https://zhuanlan.zhihu.com/p/594087648>
- <https://zhuanlan.zhihu.com/p/100418869>

# 五、parser

> 将 flatbuffer 的序列化模型转换成 mininn 的 graph

使用 python 将 onnx 等模型转化为 mininn 所需要的格式，再使用 load_graph 将其转化为 cpp 的类，加载至内存。

# 六、predictor

> 最上层接口，加载模型，运行模型等功能

1. 获取输入张量，设置输入数据
2. 准备，分配内存，初始化 kernel 等
3. 同步异步运行
4. 获取输出张量，获取最终结果
