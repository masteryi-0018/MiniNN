#ifndef REGISTER_H
#define REGISTER_H

#include "type.h"
#include "node.h"
#include "log.h"

#include <map>
#include <memory> // shared_ptr needs
#include <functional>

class OpInfoCollector {
public:
    static OpInfoCollector& Global() {
        static OpInfoCollector* x = new OpInfoCollector;
        return *x;
    }

    // TODO: to understand
    void AddOp2path(Op type, const std::string& op_path) {
    int index = op_path.find_last_of('/');
    if (index != std::string::npos) {
        op_path_.insert(std::pair<Op, std::string>(
        type, op_path.substr(index + 1)));
        }
    }

    const std::map<Op, std::string>& GetOp2PathDict() {
        return op_path_;
    }

private:
    std::map<Op, std::string> op_path_;
};

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
    // it is IMPORTANT!
    // Touch function is used to guarantee registrar was initialized.
    void touch() {}
};


// register an op
#define REGISTER_OP(op_type_, OpClass)                                         \
    static OpRegistrar op_type_##_registrar(                                   \
        op_type_, []() {                                                       \
            return std::unique_ptr<Node>(new OpClass());                       \
        });                                                                    \
    int touch_op_##op_type_() {                                                \
        op_type_##_registrar.touch();                                          \
        OpInfoCollector::Global().AddOp2path(op_type_, __FILE__);              \
        return 0;                                                              \
    }

#endif // REGISTER_H