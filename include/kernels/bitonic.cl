__kernel void bitonic_sort(__global float *data,
                           __local float *l_data) {
    int gid = get_global_id(0);
    int lid = get_local_id(0);
    int group = get_group_id(0);
    int localSize = get_local_size(0);

    int ascending = (group % 2 == 0);

    l_data[lid] = data[gid];
    barrier(CLK_LOCAL_MEM_FENCE);

    for (unsigned int k = 2; k <= localSize; k <<= 1) {
        for (unsigned int j = k >> 1; j > 0; j >>= 1) {
            int lid1 = lid ^ j;
            if (lid1 > lid) {
                if ((lid & k) == 0) {
                    if (ascending) {
                        if (l_data[lid] > l_data[lid1]) {
                            float temp = l_data[lid];
                            l_data[lid] = l_data[lid1];
                            l_data[lid1] = temp;
                        }
                    } 
                    else {
                        if (l_data[lid] < l_data[lid1]) {
                            float temp = l_data[lid];
                            l_data[lid] = l_data[lid1];
                            l_data[lid1] = temp;
                        }
                    }
                }
                else {
                    if (ascending) {
                        if (l_data[lid] < l_data[lid1]) {
                            float temp = l_data[lid];
                            l_data[lid] = l_data[lid1];
                            l_data[lid1] = temp;
                        }
                    }
                    else {
                        if (l_data[lid] > l_data[lid1]) {
                            float temp = l_data[lid];
                            l_data[lid] = l_data[lid1];
                            l_data[lid1] = temp;
                        }
                    }
                }
            }
            barrier(CLK_LOCAL_MEM_FENCE);
        }
    }
    
    data[gid] = l_data[lid];
}

__kernel void bitonic_merge(__global float *data,
                            const unsigned int k,
                            const unsigned int j) {
    int gid = get_global_id(0);
    int gid1 = gid ^ j;
    if (gid1 > gid) {
        if ((gid & k) == 0) {
            if (data[gid] > data[gid1]) {
                float temp = data[gid];
                data[gid] = data[gid1];
                data[gid1] = temp;
            }
        }
        else {
            if (data[gid] < data[gid1]) {
                float temp = data[gid];
                data[gid] = data[gid1];
                data[gid1] = temp;
            }
        }
    }
}