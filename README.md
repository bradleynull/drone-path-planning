# drone-path-planning

Drone path planning exercise for debugging drone algorithms.

## Dependencies 

This project uses standard C++ 14. To build you will need to install `cmake`:

* [CMake](https://cmake.org/download/)

## Build

```bash
$ cd ./drone-path-planning
$ mkdir ./build
$ cd ./build
$ cmake ..
```

### Building and running on Windows

The CMake project will generate a solution file you can use to debug in Visual 
Studio or just open the folder with VS Code. To run tests:

```bash
$ ctest -C Debug 
```

### Building and running on Linux

From the `build` directory

```bash
$ make test
```
