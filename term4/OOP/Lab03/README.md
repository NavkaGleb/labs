# Sort Algorithms Visualization

## Description

For Rendering using own [GameEngine](https://github.com/NavkaGleb/Ziben)

![QuickSortExample](Resources/QuickSortExample.gif)

### Implemented Sorting Algorithm
1. BubbleSort
2. SelectionSort
3. InsertionSort
4. ShellSort
5. QuickSort
6. ParallelQuickSort
7. MergeSort
8. BottomUpMergeSort
9. ParallelMergeSort

### Implemented Design Patterns
1. Singleton
2. Factory Method
3. ObjectPool
4. Observer

## Installation

Application supports only Windows Operating System and supports only MinGW compiler

Download the latest version of [MinGW](https://winlibs.com/) Compiler.
Add compiler to PATH environment variable

Download the [CMake](https://cmake.org/download/). Add CMake to PATH environment variable

```bash
> git clone --recursive https://github.com/navkagleb/Labs/tree/master/Term4/OOP/Lab03
```

Go to created directory

```bash
> mkdir build
> cd build
> cmake .. -G "MinGW Makefiles"
> mingw32-make
```

Go to 'Source' directory and run the program

```bash
> cd Source
> ./SortAlgorithms
```

## License
[MIT](https://choosealicense.com/licenses/mit/)
