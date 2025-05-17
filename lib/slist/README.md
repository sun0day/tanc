# Singly linked list(slist)

- :apple: Generic types support, `slist` can store any type of data 
- :whale: Type safe, `slist` getter/setter will block mismatched arguments
- :zap: High performance
- :rainbow: Allocator free, you can use your own `malloc` and `free`
- :christmas_tree: Unified APIs for both (non-)intrusive linked list

## Install

```shell
# Create output dir
mkdir build && cd build

# Configure CMake
cmake .. \
  -DCMAKE_C_COMPILER=gcc \ # choose a c compiler
  -DCMAKE_INSTALL_PREFIX=/usr \ # target installation directory path prefix
  -DCMAKE_BUILD_TYPE=Release 
  
# Build slist
cmake --build . --target slist slist_static 

# Install slist
sudo cmake --install . --component slist
```

After installation, the header files are in the `/usr/include/tanc` while the shared and static libraries are in the `/usr/lib/tanc`.

## Usage

### Create `slist`

```c
#include <stdio.h>
#include <stdlib.h>
#include <tanc/slist.h>

typedef struct {
  char *name;
  int age;
} Person;

TCSlistOf(IntList, int)
TCSlistOf(Persons, Person)

int main() {
  IntList list1 = tc_slist_new();
  Persons list2 = tc_slist_new();
  return 0;
}
```

### Add elements to `slist`

### Free `slist`

## API

