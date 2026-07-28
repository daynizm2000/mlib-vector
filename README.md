# MLib Vector

A lightweight, header-only generic dynamic array library for C, now with custom allocator support.

MLib Vector provides a simple macro-based interface for creating and managing dynamic arrays.  
All operations are implemented as macros, keeping the library fast and dependency-free.  
You can optionally plug in your own memory allocator to control how memory is obtained and released.

---

## Features

- Header-only – no compilation needed
- Generic vector implementation for any data type
- Automatic memory management with pluggable allocators
- Custom initial capacity
- Automatic capacity growth
- Fast push/pop operations
- Insert and remove elements
- Resize and reserve storage
- Shrink to fit allocated memory
- Copy and swap vectors (including allocator state)
- Pointer and index iteration macros (forward and reverse)
- Raw data pointer access

---

## Requirements

- GCC or Clang with GNU extensions (`typeof`, statement expressions)
- C11 or later
- Only the C standard library is required (`<stdlib.h>`, `<memory.h>`)

---

## Installation

Copy `vector.h` into your project and include it.

```c
#include "vector.h"
```

---

## Basic Example

```c
#include "vector.h"
#include <stdio.h>

int main(void)
{
    int *p;

    /* Define a vector of int with initial capacity 3 and default allocator (use 0) */
    mlib_vec_define(array, int, 3, 0);
    mlib_vec_init(&array);

    for (size_t i = 0; i < mlib_vec_capacity(&array); i++) {
        mlib_vec_push(&array, i);
        printf("Added: %d\n", *mlib_vec_back(&array));
    }

    mlib_vec_for_each(&array, p)
        printf("Removed: %d\n", mlib_vec_pop(&array));

    mlib_vec_destroy(&array);
    return 0;
}
```

Output:

```
Added: 0
Added: 1
Added: 2
Removed: 2
Removed: 1
Removed: 0
```

---

## Custom Memory Allocators

The library lets you supply your own memory management functions through the `mlib_vec_attr_t` structure.  
This is useful for memory pools, tracking allocations, or non‑standard heaps.

### mlib_vec_attr_t

```c
typedef struct {
    struct {
        void *(*alloc)(size_t size, void *arg);
        void *(*realloc)(void *addr, size_t newsize, void *arg);
        void (*free)(void *addr, void *arg);
    } mem_ops;

    void *private_data;
} mlib_vec_attr_t;
```

- `mem_ops.alloc`   – allocate memory (signature like `malloc`, receives `private_data`)
- `mem_ops.realloc` – reallocate memory (receives `private_data`)
- `mem_ops.free`    – free memory (receives `private_data`)
- `private_data`    – user‑defined pointer passed to all three functions

If a function pointer is `NULL` when the vector is initialised, the library falls back to the standard `malloc`/`realloc`/`free`.

### Using custom allocators

You must supply the allocator at definition time using `mlib_vec_define` (or later with `mlib_vec_setup`).

```c
mlib_vec_attr_t my_attr = {
    .mem_ops = {
        .alloc   = my_alloc,
        .realloc = my_realloc,
        .free    = my_free
    },
    .private_data = my_context
};

mlib_vec_define(my_vec, double, 8, my_attr);
mlib_vec_init(&my_vec);
```

Passing `0` as the fourth argument to `mlib_vec_define` uses a zero‑initialised attribute, which results in default standard library allocators and `private_data = NULL`.

Alternatively, use `mlib_vec_setup` to install an attribute on an already‑defined structure:

```c
mlib_vec_type(int) vec;
mlib_vec_setup(&vec, 16, &my_attr);
```

The attribute is stored inside the vector. `mlib_vec_init` ensures default functions where needed.  
`mlib_vec_swap` exchanges the attributes, and `mlib_vec_destroy` uses the installed `free` function.

---

## Creating a Vector

Define a vector with a custom type, initial capacity, and allocator attribute (use `0` for default):

```c
mlib_vec_define(numbers, int, 16, 0);
```

Initialize before use:

```c
mlib_vec_init(&numbers);
```

Destroy when finished:

```c
mlib_vec_destroy(&numbers);
```

If you already have a `mlib_vec_type(int)` structure, you can set it up separately:

```c
mlib_vec_type(float) vec;
mlib_vec_setup(&vec, 64, 0);
mlib_vec_init(&vec);
```

---

## Basic Operations

**Push an element**

```c
mlib_vec_push(&numbers, 42);
```

**Pop an element** (returns the removed value)

```c
int value = mlib_vec_pop(&numbers);
```

**Access element by index** (returns the value)

```c
int v = mlib_vec_get(&numbers, 0);
```

**Set element at index** (returns 0 on success, -1 on error)

```c
mlib_vec_set(&numbers, 100, 0);
```

**Access first/last element (pointers)**

```c
int *first = mlib_vec_front(&numbers);
int *last  = mlib_vec_back(&numbers);
```

**Clear all elements (size becomes 0, memory kept)**

```c
mlib_vec_clear(&numbers);
```

---

## Capacity Management

```c
mlib_vec_reserve(&numbers, 128);          // ensure space for 128 elements
mlib_vec_resize(&numbers, 64);            // resize to 64 elements (zero‑initialised)
mlib_vec_resize(&numbers, 64, -1);        // resize with fill value -1
mlib_vec_shrink_to_fit(&numbers);         // free unused capacity
```

---

## Insertion & Removal

```c
mlib_vec_insert(&numbers, 99, 2);        // insert 99 at index 2
int removed = mlib_vec_remove(&numbers, 1); // remove element at index 1, return it
mlib_vec_erase(&numbers, 2, 5);          // remove elements in range [2, 5)
```

---

## Iteration

Forward by pointer:

```c
int *it;
mlib_vec_for_each(&numbers, it) {
    printf("%d\n", *it);
}
```

Forward by index:

```c
size_t i;
mlib_vec_for_each_index(&numbers, i) {
    printf("%d\n", numbers.data[i]);
}
```

Reverse by pointer:

```c
int *it;
mlib_vec_for_each_reverse(&numbers, it) {
    printf("%d\n", *it);
}
```

Reverse by index:

```c
size_t i;
mlib_vec_for_each_index_reverse(&numbers, i) {
    printf("%d\n", numbers.data[i]);
}
```

---

## Information

```c
size_t sz = mlib_vec_size(&numbers);
size_t cap = mlib_vec_capacity(&numbers);
size_t isize = mlib_vec_item_size(&numbers);
int empty = mlib_vec_empty(&numbers);        // 1 if empty, 0 otherwise
void *raw = mlib_vec_data(&numbers);         // direct pointer to data
```

---

## Swap & Copy

```c
mlib_vec_swap(&vec1, &vec2);    // exchanges contents and allocator attributes
mlib_vec_copy(&dest, &src);     // deep copy, dest gets default allocator
```

---

## API Overview

### Type & Initialisation

- `mlib_vec_type(type)` – declare the struct type
- `mlib_vec_define(name, type, cap, attr_val)` – define and partially initialise a vector
- `mlib_vec_setup(vec, cap, attr)` – set capacity and allocator attribute on an existing structure
- `mlib_vec_init(vec)` – allocate memory, install default allocators if needed
- `mlib_vec_destroy(vec)` – free memory using the installed allocator

### Element Access

- `mlib_vec_get(vec, idx)`
- `mlib_vec_set(vec, elem, idx)`
- `mlib_vec_front(vec)`
- `mlib_vec_back(vec)`
- `mlib_vec_data(vec)`

### Capacity

- `mlib_vec_reserve(vec, newcap)`
- `mlib_vec_resize(vec, newsize, ...)`
- `mlib_vec_shrink_to_fit(vec)`

### Modifiers

- `mlib_vec_push(vec, elem)`
- `mlib_vec_pop(vec)`
- `mlib_vec_insert(vec, elem, idx)`
- `mlib_vec_remove(vec, idx)`
- `mlib_vec_erase(vec, start, end)`
- `mlib_vec_clear(vec)`

### Information

- `mlib_vec_size(vec)`
- `mlib_vec_capacity(vec)`
- `mlib_vec_item_size(vec)`
- `mlib_vec_empty(vec)`

### Utilities

- `mlib_vec_swap(vec1, vec2)`
- `mlib_vec_copy(dst, src)`

### Iteration

- `mlib_vec_for_each(vec, p)`
- `mlib_vec_for_each_index(vec, idx)`
- `mlib_vec_for_each_reverse(vec, p)`
- `mlib_vec_for_each_index_reverse(vec, idx)`

---

## Notes

- All macros evaluate their arguments multiple times; avoid side effects.
- The `mlib_vec_define` fourth argument accepts `0` for default allocators, or a `mlib_vec_attr_t` variable for custom ones.
- The library relies on GNU C extensions; compile with GCC or Clang.
```