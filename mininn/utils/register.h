#ifndef REGISTER_H
#define REGISTER_H

#include "type.h"
#include "node.h"
#include "kernel.h"
#include "log.h"

#include <map>
#include <memory> // shared_ptr needs
#include <functional>
#include <string>

class OpFactory {
public:
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

class KernelFactory {
public:
    void RegisterCreator(Op type, std::function<std::shared_ptr<Kernel>()> func) {
        kernel_registry_[type] = func;
        return;
    }

    static KernelFactory& Global(){
        static KernelFactory* x = new KernelFactory;
        return *x;
    }

    std::shared_ptr<Kernel> Create(Op type) {
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
        KernelFactory::Global().RegisterCreator(type, func);
    }
    // it is IMPORTANT!
    // Touch function is used to guarantee registrar was initialized.
    void touch() {}
};

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