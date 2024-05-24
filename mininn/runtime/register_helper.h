#ifndef REGISTER_HELPER_H
#define REGISTER_HELPER_H

#define UNUSED __attribute__((unused))

#define USE_OP(op_type_)                                                       \
    extern int touch_op_##op_type_();                                          \
    int OP_REGISTER_FAKE(op_type_) UNUSED = touch_op_##op_type_();

#define OP_REGISTER_FAKE(op_type_) op_type_##_registrar_

#endif // REGISTER_HELPER_H