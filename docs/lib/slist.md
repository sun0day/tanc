# Singly linked list(slist)

> A singly linked list is a linked data structure that consists of a set of sequentially linked records called nodes. Each node contains two fields: **one link field** (references to the next node in the sequence of nodes) and **one data field**. 

In `Tanc`, the node's link field is called `TCSlistPos`. `TCSlistPos` stores the next `TCSlistPos` address. This makes it easy to embed the `TCSlistPos` to your own data structure for a more compact memory layout. By default, the node's data field `data` of any type is placed together with a `TCSlistPos` field. The singly linked list `TCSlist` itself only has two sentinel `TCSlistPos`s for manipulating and traversing its nodes.

![slist](./slist.svg)

## Features

- :apple: Generic types: `TCSlist` are suitable for any data type
- :zap: High performance
- :christmas_tree: Memory efficient: only 1 additional pointer size for each node and 2 additional pointer size for the list
- :rainbow: Allocator free: you can use your own `malloc` and `free`

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
#define TCAllocator my_malloc, my_free 
#include <tanc/slist.h>

TCSlistOf(int)
//...
```

Sometimes the `slist` node's default memory alignment may be not what you want, you can define a new node type with more compact memory layout by yourself.

```c
typedef struct {
  TCSlistPos pos; // embed the link into Fruit, only 1 pointer size
  char *name;
  char level;
} Fruit;

TCSlistOf(Fruit, pos)  
//               |----> tell slist what the link member is 

int main() {
  TCSlist *list3 = tc_slist_new();
  return 0;
}
```

### Add nodes 

#### Add nodes to front or back

```c
// add nodes to the back of slist
tc_slist_append(list1, int, 1);
tc_slist_append(list2, Person, (Person){.name="alice", .age=20});
tc_slist_append(list3, Fruit, (Fruit){.name="apple", .level='A'});

// add nodes to the front of slist
tc_slist_prepend(list1, int, 2);
tc_slist_prepend(list2, Person, (Person){.name="bob", .age=30});
tc_slist_prepend(list3, Fruit, (Fruit){.name="banana", .level='B'});
```

#### Add nodes to arbitrary position

```c
// add nodes to the position after the first node
TCSlistIter iter = tc_slist_begin(list1);
tc_slist_insert(iter, int, 3);

iter = tc_slist_begin(list2);
tc_slist_insert(iter, Person, (Person){.name="davy", .age=12});

iter = tc_slist_begin(list3);
tc_slist_insert(list3, Fruit, (Fruit){.name="cherry", .level='C'});
```

### Iterate over `slist`

You can access every node from front to back through `TCSlistIter`.

```c
TCSlistIter begin = tc_slist_begin(list1); 

tc_slist_each(begin, NULL, cur) {
  printf("%d ", *tc_slist_at(cur, int)); // output 2 3 1
}

begin = tc_slist_begin(list2);

tc_slist_each(begin, NULL, cur) {
  printf("%s ", tc_slist_at(cur, Person)->name); // output bob davy alice
}

begin = tc_slist_begin(list3);

tc_slist_each(begin, NULL, cur) {
  printf("%s ", tc_slist_at(cur, Fruit)->name); // output banana cherry apple  
}
```

Also `tc_slist_each` supports to access nodes within a certain range.

```c
TCSlistIter begin = tc_slist_begin(list1); 

tc_slist_each(begin, tc_slist_next(begin), cur) {
  printf("%d ", *tc_slist_at(cur, int)); // output 2
}
```

### Remove nodes

#### Remove all nodes

```c
tc_slist_clear(list1, int);
tc_slist_clear(list2, Person);
tc_slist_clear(list3, Fruit);
```

### Free `slist`

`tc_slist_free` will release the entire memory allocated by `slist`.

```c
tc_slist_free(list1, int);
tc_slist_free(list2, Person);
tc_slist_free(list3, Fruit);
// list1, list2, list3 are set to NULL automatically after free
```

## API

