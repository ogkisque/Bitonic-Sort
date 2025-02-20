# Bitonic sort realization on OpenCL

## Introduction

This project is the implementation of the wrapper over opencl C API, and the implementation of the bitonic sorting algorithm using it.

## Build and Run

Cloning repository:
```
git clone --recursive https://github.com/ogkisque/OpenCL.git
git submodule update --init --recursive
cd libhayai
git fetch
```

If you want to build the project, write this in the project directory:
```
cmake -S . -B build
cmake --build build
```

After that, you can run main target program:

```
./build/src/main
```

## Tests
### Unit

If you want to run unit tests, generate Makefiles with the WITH_TESTS flag:
```
cmake [...] -DWITH_TESTS=1
```

Then build `tests` target:
```
cmake --build build --target tests
```

After that, run:
```
./build/tests/tests
```

### End to end

If you want to run end-to-end tests, type it:
```
python3 tests/check_end_to_end.py
```

### Benchmarks

Also you can compare the performance of our bitonic sort and std::sort.

Generate Makefiles with the WITH_BENCHMARKS flag:
```
cmake [...] -DWITH_BENCHMARKS=1
```

Then build `benchs` target:
```
cmake --build build --target benchs
```

Finally, run the program:
```
./build/benchs/benchs
```
