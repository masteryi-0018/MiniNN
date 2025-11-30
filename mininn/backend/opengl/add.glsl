#version 430

layout(local_size_x = 256) in;

layout(std430, binding = 0) buffer A { float a[]; };
layout(std430, binding = 1) buffer B { float b[]; };
layout(std430, binding = 2) buffer C { float c[]; };

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx < a.length())  // avoid out-of-bounds
        c[idx] = a[idx] + b[idx];
}