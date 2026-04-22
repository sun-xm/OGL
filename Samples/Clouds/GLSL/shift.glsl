#version 430 core // min required version 430

#define LOCAL_SIZE  (256)
layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

// binding needed
layout(std430, binding = 0) buffer Position
{
    float positions[];
};

layout(std430, binding = 1) buffer Center
{
    vec3 center;
};

void main()
{
    uint gid = gl_GlobalInvocationID.x;
    uint lid = gl_LocalInvocationID.x;
    uint off = gid * 3;

    if (gid < positions.length())
    {
        positions[off + 0] -= center.x;
        positions[off + 1] -= center.y;
        positions[off + 2] -= center.z;
    }
}