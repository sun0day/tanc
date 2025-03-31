![ha-c-logo](https://avatars.githubusercontent.com/u/196289381?s=200&v=4)

# Ha-C

![version](https://img.shields.io/badge/version-1.0.0-7446ff)

C library collection.

## Installation

`Ha-C` uses `CMake` to build and install libraries.

```shell
# Create output dir
mkdir build && cd build

# Configure CMake
cmake .. \
  -DCMAKE_C_COMPILER=gcc \ # choose a c compiler
  -DCMAKE_INSTALL_PREFIX=/usr \ # target install directory path prefix
  -DCMAKE_BUILD_TYPE=Release 

# Install
sudo cmake --install . 
```

After installation, the header files are in the `/usr/include/sum` while the static and dynamic libraries are in the `/usr/lib`.

## Usage

> The following is sample code usage description:

```c
#include <stdio.h>
#include "sum/sum.h"

int main() {
  printf("%d\n", sum(1, 2));
  return 0;
}
```

## API

> The following is sample code API description:

### `int sum(int a, int b)`

Calculate two `integer`s sum.



