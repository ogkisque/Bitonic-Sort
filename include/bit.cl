__kernel void bitonic_sort(__global int *data, const int n, const int j, const int k) {
    int index = get_global_id(0);
    int ixj = index ^ j;

    if (ixj > index) {
        if ((index / k) % 2 == 0) {
            if (data[index] > data[ixj]) {
                int temp = data[index];
                data[index] = data[ixj];
                data[ixj] = temp;
            }
        } else {
            if (data[index] < data[ixj]) {
                int temp = data[index];
                data[index] = data[ixj];
                data[ixj] = temp;
            }
        }
    }
}
