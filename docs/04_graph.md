# 图

> 也就是深度学习模型的代码表示，有向无环图

节点需要考虑以下几个属性：

1. 全部node。
2. 全部tensor。
3. 输入。这是整个网络的输入，是一个特殊的node的输入
4. 输出。同上

考虑以下几个方法：

1. 添加节点。是实际的构图方法，但是需要注意tensor索引的正确性，比较复杂
2. 获取node。
3. 获取tensor。