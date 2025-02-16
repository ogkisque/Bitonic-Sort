__kernel void bitonic_sort(__global int *data, const int n, const int j, const int k) {
    int index = get_global_id(0);
    int ixj = index ^ j; // Bitwise XOR to generate the partner index

    // Ensure we stay within range
    if (ixj > index) {
        if ((index / k) % 2 == 0) {
            // Ascending order
            if (data[index] > data[ixj]) {
                // Swap
                int temp = data[index];
                data[index] = data[ixj];
                data[ixj] = temp;
            }
        } else {
            // Descending order
            if (data[index] < data[ixj]) {
                // Swap
                int temp = data[index];
                data[index] = data[ixj];
                data[ixj] = temp;
            }
        }
    }
}
