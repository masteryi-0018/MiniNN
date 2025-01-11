#version 430

layout(local_size_x = 256) in; // Define work group size

layout(std140, binding = 0) buffer A {
    float A_data[];
};

layout(std140, binding = 1) buffer B {
    float B_data[];
};

layout(std140, binding = 2) buffer C {
    float C_data[];
};

void main() {
    uint id = gl_GlobalInvocationID.x;
    C_data[id] = A_data[id] + B_data[id]; // Perform vector addition
}