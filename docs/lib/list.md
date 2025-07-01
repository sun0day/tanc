# Doubly linked list

- :apple: Generic types support: `list` can store any type of data 
- :whale: Type safe: `list` getter/setter will block mismatched arguments
- :zap: High performance
- :rainbow: Allocator free: you can use your own `malloc` and `free`
- :christmas_tree: Memory efficient: `list` is intrusive

## Install

```shell
# Create output dir
mkdir build && cd build

# Configure CMake
cmake .. \
  -DCMAKE_C_COMPILER=gcc \ # choose a c compiler
  -DCMAKE_INSTALL_PREFIX=/usr \ # target installation directory path prefix
  -DCMAKE_BUILD_TYPE=Release 
  
# Build list
cmake --build . --target list list_static 

# Install list
sudo cmake --install . --component list
```

After installation, the header files are in the `/usr/include/tanc` while the shared and static libraries are in the `/usr/lib/tanc`.

## Usage

### Create `list`

```c
#include <stdio.h>
// provide malloc and free
#include <stdlib.h>
#include <tanc/list.h>

typedef struct {
  char *name;
  int age;
} Person;

// declare list node's data type
TCListOf(int)
TCListOf(Person)

int main() {
  // init list
  TCList *list1 = tc_list_new();
  TCList *list2 = tc_list_new();
  return 0;
}
```

`TCListOf(Type)` is used to declare  `list` node's data type. By default, `list` uses standard `malloc` and `free` interfaces as its internal allocator, you can decide where the `malloc` and `free` implementations come from, e.g: `stdlib`. You can also use your own `malloc` and `free`:

```c
// define my_malloc and my_free
#include "my_alloc.h" 
// use your own malloc and free
#define TCAllocator my_malloc, my_free 
#include <tanc/list.h>

TCListOf(int)
//...
```

Sometimes the `list` node's default memory alignment may be not what you want, you can define a new node type with more compact memory layout by yourself.

```c
typedef struct {
  TCListPos pos; // embed the link into Fruit, only 2 pointer size
  char *name;
  char level;
} Fruit;

TCListOf(Fruit, pos)  
//               |----> tell list what the link member is 

int main() {
  TCList *list3 = tc_list_new();
  return 0;
}
```

### Add nodes 

#### Add nodes to front or back

```c
// add nodes to the back of list
tc_list_push(list1, int, 1);
tc_list_push(list2, Person, (Person){.name="alice", .age=20});
tc_list_push(list3, Fruit, (Fruit){.name="apple", .level='A'});

// add nodes to the front of list
tc_list_unshift(list1, int, 2);
tc_list_unshift(list2, Person, (Person){.name="bob", .age=30});
tc_list_unshift(list3, Fruit, (Fruit){.name="banana", .level='B'});
```

#### Add nodes to arbitrary position

```c
// add nodes to the position after the first node
TCListIter iter = tc_list_begin(list1);
tc_list_insert(iter, int, 3);

iter = tc_list_begin(list2);
tc_list_insert(iter, Person, (Person){.name="davy", .age=12});

iter = tc_list_begin(list3);
tc_list_insert(list3, Fruit, (Fruit){.name="cherry", .level='C'});
```

### Iterate over `list`

You can access each node from front to back through `TCListIter`.

```c
TCListIter begin = tc_list_begin(list1);
TCListIter end = tc_list_end(list1)

tc_list_each(begin, end, cur) {
  printf("%d ", *tc_list_at(cur, int)); // output 2 3 1
}

begin = tc_list_begin(list2);
end = tc_list_end(list2)

tc_list_each(begin, end, cur) {
  printf("%s ", tc_list_at(cur, Person)->name); // output bob davy alice
}

begin = tc_list_begin(list3);
end = tc_list_end(list3)

tc_list_each(begin, end, cur) {
  printf("%s ", tc_list_at(cur, Fruit)->name); // output banana cherry apple  
}
```

Also `tc_list_each` supports to access nodes within a certain range.

```c
TCListIter begin = tc_list_begin(list1); 

tc_list_each(begin, tc_list_next(begin), cur) {
  printf("%d ", *tc_list_at(cur, int)); // output 2
}
```

Accessing each node from back to front is like going from front to back:

```c
TCListIter begin = tc_list_rbegin(list1);
TCListIter end = tc_list_rend(list1)

tc_list_reach(begin, end, cur) {
  printf("%d ", *tc_list_at(cur, int)); // output 1 3 2
}
```

### Remove nodes

#### Remove all nodes

```c
tc_list_clear(list1, int);
tc_list_clear(list2, Person);
tc_list_clear(list3, Fruit);
```

### Free `list`

`tc_list_free` will release the entire memory allocated by `list`.

```c
tc_list_free(list1, int);
list1 = NULL;
tc_list_free(list2, Person);
list2 = NULL;
tc_list_free(list3, Fruit);
list3 = NULL;
```

## API

