#ifndef KERNEL_H
#define KERNEL_H

class Kernel {
public:
    Kernel();

    // must be a virtual func
    virtual void run();
};

#endif // KERNEL_H