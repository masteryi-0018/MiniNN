#ifndef REGISTER_H
#define REGISTER_H

#include "mininn/graph/type.h"
#include "mininn/graph/node.h"
#include "mininn/graph/kernel.h"

#include <map>
#include <memory> // shared_ptr needs
#include <functional>
#include <string>

class OpFactory {
public:
    void register_creator(Op type, std::function<std::shared_ptr<Node>()> func) {
        op_registry_[type] = func;
        return;
    }

    static OpFactory& global(){
        static OpFactory* x = new OpFactory;
        return *x;
    }

    std::shared_ptr<Node> create(Op type) {
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
        OpFactory::global().register_creator(type, func);
    }
    // it is IMPORTANT!
    // Touch function is used to guarantee registrar was initialized.
    void touch() {}
};

class KernelFactory {
public:
    void register_creator(Op type, std::function<std::shared_ptr<Kernel>()> func) {
        kernel_registry_[type] = func;
        return;
    }

    static KernelFactory& global(){
        static KernelFactory* x = new KernelFactory;
        return *x;
    }

    std::shared_ptr<Kernel> create(Op type) {
        auto it = kernel_registry_.find(type);
        if (it == kernel_registry_.end()) {
            return nullptr;
        }
        return it->second();
    }

private:
    std::map<Op, std::function<std::shared_ptr<Kernel>()>> kernel_registry_;
};

class KernelRegistrar {
public:
    KernelRegistrar(Op type, std::function<std::shared_ptr<Kernel>()> func) {
        KernelFactory::global().register_creator(type, func);
    }
    // it is IMPORTANT!
    // Touch function is used to guarantee registrar was initialized.
    void touch() {}
};

// todo: why below is a unique_ptr?
// register an op
#define REGISTER_OP(op_type_, OpClass)                                         \
    static OpRegistrar op_type_##_registrar(                                   \
        op_type_, []() {                                                       \
            return std::unique_ptr<Node>(new OpClass(op_type_));               \
        });                                                                    \
    int touch_op_##op_type_() {                                                \
        op_type_##_registrar.touch();                                          \
        return 0;                                                              \
    }

// register an kernel
#define REGISTER_KERNEL(kernel_type_, KernelClass)                             \
    static KernelRegistrar kernel_type_##_registrar(                           \
        kernel_type_, []() {                                                   \
            return std::unique_ptr<Kernel>(new KernelClass());                 \
        });                                                                    \
    int touch_kernel_##kernel_type_() {                                        \
        kernel_type_##_registrar.touch();                                      \
        return 0;                                                              \
    }

#endif // REGISTER_H