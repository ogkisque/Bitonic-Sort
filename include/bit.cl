__kernel void bitonic_sort(__global float *data, const int n) {
    int index = get_global_id(0);

    for (int k = 2; k <= n; k <<= 1) {
        for (int j = k >> 1; j > 0; j >>= 1) {
            int ixj = index ^ j;

            if (ixj > index) {
                if ((index / k) % 2 == 0) {
                    if (data[index] > data[ixj]) {
                        float temp = data[index];
                        data[index] = data[ixj];
                        data[ixj] = temp;
                    }
                } else {
                    if (data[index] < data[ixj]) {
                        float temp = data[index];
                        data[index] = data[ixj];
                        data[ixj] = temp;
                    }
                }
            }

            barrier(CLK_GLOBAL_MEM_FENCE);
        }
    }
}
