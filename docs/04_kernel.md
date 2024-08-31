# kernel

> 真正执行计算的对象

op_compute是kernel的派生类，也就是对应不同的算子的具体计算实现，kernel是op_compute的抽象。

graph持有node和tensor，能够完整的表示深度学习的网络模型。在真正推理的时候，一般是遍历graph中的node，以node为单位按照拓扑顺序逐个计算每个节点的输出，如下：

1. 拿到整个图的输入，和第一个节点，算出第一个节点的输出
2. 拿到上一个节点的输出（也就是本节点的输入），和本节点，算出下一个节点的输出
3. 将最后一个节点的输出，作为整个图的输出

## kernel类存在的必要性

1. 计算关系虽然是node为主导，但实际执行计算的时候，需要拿到tensor的信息，也就是node并不能单独完成计算任务，需要一个能够持有node和tensor的类完成计算，也就是kernel。
2. 不同的backend对于同一个具体计算的表达是不同的，也就是kernel是与后端有关的，而op（node）是与kernel无关的，所以无法在node类中实现不同后端的计算逻辑，也会让node负担更重。

## kernel注册的实现

需求是：在遍历graph的时候，根据每个op，创建对应的kernel，这是需要从注册器中创建的。

实际在注册的时候，是将op与创建kernel的`函数/方法`成对放在了map中，并不是真的创建了一个kernel，在准备阶段，使用注册器，根据op来创建kernel，运行时直接执行kernel的方法就可以。

## 参考资料

1. tflite的kernel都是tflite cpu或者xnnpack的，比较固定，所以先创建kernel再创建op；与此同时，node还需要能够找到对应的kernel，在tflite中采用了`nodes_and_registration_`将node和kernel绑定。
2. pdlite的kernel是多后端的，支持运行时指定后端，所以先创建op再创建kernel；与此同时，在pdlite中op和kernel都有对应的注册方法，运行时node与kernel绑定，node执行infer shape的功能，kernel执行计算。

- <https://cloud.baidu.com/article/3275457>
- <https://zhuanlan.zhihu.com/p/594087648>
- <https://zhuanlan.zhihu.com/p/100418869>