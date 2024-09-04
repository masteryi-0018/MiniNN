#ifndef REGISTER_HELPER_H
#define REGISTER_HELPER_H

// todo: don't definition variable in head file
// try other method to use op and kernel, avoid being optimized by compiler
#ifdef _MSC_VER
#define UNUSED
#else
// __attribute__ is a gcc extension, it can not be used in windows
#define UNUSED __attribute__((unused))
#endif

#define USE_OP(op_type_)                                                       \
    extern int touch_op_##op_type_();                                          \
    int op_type_##_op_registrar_ UNUSED = touch_op_##op_type_();

#define USE_KERNEL(kernel_type_)                                               \
    extern int touch_kernel_##kernel_type_();                                  \
    int kernel_type_##_kernel_registrar_ UNUSED = touch_kernel_##kernel_type_();

#endif // REGISTER_HELPER_H