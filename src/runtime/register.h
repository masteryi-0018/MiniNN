#include <map>
#include <memory> // shared_ptr needs
#include <functional>

#include "node.h"
#include "type.h"

class OpFactory {
public:
    // 通过op_type注册op
    void RegisterCreator(Op type, std::function<std::shared_ptr<Node>()> func) {
        op_registry_[type] = func;
        return;
    }

    static OpFactory& Global(){
        static OpFactory* x = new OpFactory;
        return *x;
    }

    std::shared_ptr<Node> Create(Op type) {
        auto it = op_registry_.find(type);
        if (it == op_registry_.end()) {
            return nullptr;
        }
        return it->second();
    }

private:
    std::map<Op, std::function<std::shared_ptr<Node>()>> op_registry_;
};

class OpRegistrar {
public:
    OpRegistrar(Op type, std::function<std::shared_ptr<Node>()> func) {
        OpFactory::Global().RegisterCreator(type, func);
    }
};


// register an op
#define REGISTER_OP(op_type_, OpClass)                                         \
    static OpRegistrar op_type_##_registrar(                                   \
        op_type_, []() {                                                       \
            return std::unique_ptr<Node>(new OpClass());                       \
        });
