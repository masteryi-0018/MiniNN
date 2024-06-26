#ifndef REGISTER_HELPER_H
#define REGISTER_HELPER_H

#define UNUSED __attribute__((unused))

#define USE_OP(op_type_)                                                       \
    extern int touch_op_##op_type_();                                          \
    int OP_REGISTER_FAKE(op_type_) UNUSED = touch_op_##op_type_();

// why kernel do not need USE_KERNEL to guarantee kernel has init?
#define USE_KERNEL(kernel_type_)                                               \
    extern int touch_kernel_##kernel_type_();                                  \
    int kernel_type_##_registrar_ UNUSED = touch_kernel_##kernel_type_();

#define OP_REGISTER_FAKE(op_type_) op_type_##_registrar_

#endif // REGISTER_HELPER_H