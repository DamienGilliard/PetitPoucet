# Install

## build the library and the executable on ubuntu
To build PetitPoucet on your ubuntu computer and use its executable, proceed as follows:

- clone the repository if it is not already done
- create a build folder in the src folder:
```bash
cd PetitPoucet/src
mkdir build && cd build
```
- configure the build using CMake:
```bash
cmake ..
```

- build the executables and link the library:
```bash
make
```

## link the library only to your project

coming as soon as it is solved