# Singly linked list(slist)

- :apple: Generic types support: `slist` can store any type of data 
- :whale: Type safe: `slist` getter/setter will block mismatched arguments
- :zap: High performance
- :rainbow: Allocator free: you can use your own `malloc` and `free`
- :christmas_tree: Memory efficient: `slist` is intrusive

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
// provide malloc and free
#include <stdlib.h>
#include <tanc/slist.h>

typedef struct {
  char *name;
  int age;
} Person;

// declare slist node's data type
TCSlistOf(int)
TCSlistOf(Person)

int main() {
  // init slist
  TCSlist *list1 = tc_slist_new();
  TCSlist *list2 = tc_slist_new();
  return 0;
}
```

`TCSlistOf(Type)` is used to declare  `slist` node's data type. By default, `slist` uses standard `malloc` and `free` interfaces as its internal allocator, you can decide where the `malloc` and `free` implementations come from, e.g: `stdlib`. You can also use your own `malloc` and `free`:

```c
// define my_malloc and my_free
#include "my_alloc.h" 
// use your own malloc and free
#define TC_ALLOCATOR my_malloc, my_free 
#include <tanc/slist.h>

TCSlistOf(int)
//...
```

Sometimes the `slist` node's default memory alignment may be not what you want, you can define a new node type with more compact memory layout by yourself.

```c
typedef struct {
  TCSlistPos pos; // embed the link into Fruit, only 1 pointer size
  int price;
  char kind;
} Fruit;

TCSlistOf(Fruit, pos)  
//               |----> tell slist what the link member is 
```

### Add nodes 

### Free `slist`

## API

