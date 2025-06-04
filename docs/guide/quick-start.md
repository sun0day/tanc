# Quick start

`Tanc` uses `CMake` to build and install libraries.

```shell
# Create output dir
mkdir build && cd build

# Configure CMake
cmake .. \
  -DCMAKE_C_COMPILER=gcc \ # choose a c compiler
  -DCMAKE_INSTALL_PREFIX=/usr \ # target installation directory path prefix
  -DCMAKE_BUILD_TYPE=Release 
  
# Build all libs
cmake --build .

# Build specific shared and static libs
# cmake --build . --target {lib} {lib}_static 

# Install
sudo cmake --install .  # make sure you have the right permission to access the installation path

# Install specific lib files
# sudo cmake --install . --component {lib}
```

After installation, the header files are in the `/usr/include/tanc` while the shared and static libraries are in the `/usr/lib/tanc`.