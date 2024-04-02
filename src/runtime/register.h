#include <map>
#include <memory> // shared_ptr needs
#include <functional>

#include "node.h"
#include "type.h"

class OpFactory {
public:
    // 通过op_type注册op
    void RegisterCreator(std::string type, std::function<std::shared_ptr<Node>()> func) {
        op_registry_[type] = func;
        return;
    }

    static OpFactory& Global(){
        static OpFactory* x = new OpFactory;
        return *x;
    }

    std::shared_ptr<Node> Create(std::string type) {
        auto it = op_registry_.find(type);
        if (it == op_registry_.end()) {
            return nullptr;
        }
        return it->second();
    }

private:
    std::map<std::string, std::function<std::shared_ptr<Node>()>> op_registry_;
};

class OpLiteRegistrar {
public:
    OpLiteRegistrar(std::string type, std::function<std::shared_ptr<Node>()> func) {
        OpFactory::Global().RegisterCreator(type, func);
    }
};


// register an op
#define REGISTER_OP(op_type__, OpClass)                                       \
    static OpLiteRegistrar op_type__##__registry(                             \
        #op_type__, []() {                                                    \
            return std::unique_ptr<Node>(new OpClass());                      \
        });                                                                   \
