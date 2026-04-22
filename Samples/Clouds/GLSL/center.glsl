#version 430 core // min required version 430

#define LOCAL_SIZE  (256)
layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

// binding needed
layout(std430, binding = 0) buffer Position
{
    float positions[];
};

layout(std430, binding = 1) buffer Summary
{
    uint sum[3];
};

layout(binding = 2) uniform atomic_uint nancnt;

shared vec3 local[LOCAL_SIZE];

void atomicSum(vec3 val)
{
    for (uint i = 0; i < 3; i++)
    {
        uint ov, nv;
        do
        {
            ov = atomicAdd(sum[i], 0);
            nv = floatBitsToUint(val[i] + uintBitsToFloat(ov));

        } while (ov != atomicCompSwap(sum[i], ov, nv));
    }
}

void main()
{
    uint gid = gl_GlobalInvocationID.x;
    uint lid = gl_LocalInvocationID.x;
    uint off = gid * 3;

    local[lid] = vec3(0, 0, 0);

    if (gid < positions.length() / 3)
    {
        vec3 pos = vec3(positions[off], positions[off + 1], positions[off + 2]);
        if (any(isnan(pos)))
        {
            atomicCounterIncrement(nancnt);
            pos = vec3(0, 0, 0);
        }

        local[lid] = pos;
    }

    barrier();

    for (uint mid = LOCAL_SIZE / 2; lid < mid; mid >>= 1)
    {
        local[lid] += local[lid + mid];
    }

    barrier();

    if (0 == lid)
    {
        atomicSum(local[0]);
    }
}