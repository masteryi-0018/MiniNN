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

class OpKernelInfoCollector {
public:
    static OpKernelInfoCollector& Global() {
        static OpKernelInfoCollector* x = new OpKernelInfoCollector;
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

    // TODO: to understand
    void AddKernel2path(Op type, const std::string& kernel_path) {
    int index = kernel_path.find_last_of('/');
    if (index != std::string::npos) {
        kernel_path_.insert(std::pair<Op, std::string>(
        type, kernel_path.substr(index + 1)));
        }
    }

    const std::map<Op, std::string>& GetOp2PathDict() {
        return op_path_;
    }

    const std::map<Op, std::string>& GetKernel2PathDict() {
        return kernel_path_;
    }

private:
    std::map<Op, std::string> op_path_;
    std::map<Op, std::string> kernel_path_;
};

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
        OpKernelInfoCollector::Global().AddOp2path(op_type_, __FILE__);        \
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
        OpKernelInfoCollector::Global().AddKernel2path(kernel_type_, __FILE__);\
        return 0;                                                              \
    }

#endif // REGISTER_H