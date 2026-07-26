# MLib Vector

A lightweight, header-only generic dynamic array library for C.

MLib Vector provides a simple interface for creating and managing dynamic arrays using macros. It is designed to be easy to integrate into existing projects while remaining efficient and portable.

## Features

- Header-only library
- Generic vector implementation
- Automatic memory management
- Automatic capacity growth
- Custom initial capacity
- Fast push/pop operations
- Insert and remove elements
- Resize and reserve storage
- Shrink allocated memory
- Copy and swap vectors
- Multiple iteration macros
- Access to raw data pointer

## Requirements

- GCC or Clang
- C11 or newer
- GNU extensions (`__typeof__`, statement expressions)

The library relies on GNU C extensions, so compiling with GCC is recommended.

## Dependencies

Only the C standard library is required.

```c
#include <stdlib.h>
#include <memory.h>
```

No external libraries are needed.

## Installation

Simply copy `vector.h` into your project and include it.

```c
#include "vector.h"
```

Nothing else is required.

---

# Creating a Vector

Define a vector with a custom type and initial capacity.

```c
mlib_vec_define(numbers, int, 16);
```

Initialize it before use.

```c
mlib_vec_init(&numbers);
```

Destroy it when finished.

```c
mlib_vec_destroy(&numbers);
```

---

# Basic Operations

Push an element

```c
mlib_vec_push(&numbers, 42);
```

Pop an element

```c
int value = mlib_vec_pop(&numbers);
```

Get an element

```c
int value = mlib_vec_get(&numbers, 0);
```

Set an element

```c
mlib_vec_set(&numbers, 100, 0);
```

Access first/last element

```c
mlib_vec_front(&numbers);
mlib_vec_back(&numbers);
```

---

# Capacity Management

Reserve memory

```c
mlib_vec_reserve(&numbers, 128);
```

Resize vector

```c
mlib_vec_resize(&numbers, 64);
```

Resize and initialize new elements

```c
mlib_vec_resize(&numbers, 64, -1);
```

Shrink memory usage

```c
mlib_vec_shrink_to_fit(&numbers);
```

---

# Iteration

Loop through all elements

```c
mlib_vec_for_each(&numbers, i)
{
    printf("%d\n", numbers.data[i]);
}
```

Loop over allocated capacity

```c
mlib_vec_for_each_capacity(&numbers, i)
{
    ...
}
```

Several additional iteration macros are available, including reverse iteration, pointer iteration, and value iteration.

---

# Information

```c
mlib_vec_size(&numbers);
mlib_vec_capacity(&numbers);
mlib_vec_item_size(&numbers);
mlib_vec_empty(&numbers);
mlib_vec_data(&numbers);
```

---

# Example

The repository contains `example.c`, demonstrating the basic usage of the library.

The example:

1. Creates an integer vector.
2. Initializes it.
3. Fills the vector using `mlib_vec_push()`.
4. Prints every inserted element.
5. Removes every element using `mlib_vec_pop()`.
6. Destroys the vector before exiting.

The example intentionally demonstrates only a small subset of the available API. See the list of macros in `vector.h` for the complete interface.

---

# Build

Compile the example with

```bash
make
```

or manually

```bash
gcc example.c -o vector
```

Run it

```bash
./vector
```

---

# Build Script

The repository also includes a simple helper script.

```bash
#!/bin/bash

if [ ! -f vector ]; then
    make
fi

./vector

rm vector
```

The script:

- builds the example if necessary;
- runs the executable;
- removes the generated binary afterward.

---

# API Overview

The library provides the following interfaces.

## Initialization

- `mlib_vec_define`
- `mlib_vec_type`
- `mlib_vec_setup`
- `mlib_vec_init`

## Element Access

- `mlib_vec_get`
- `mlib_vec_set`
- `mlib_vec_front`
- `mlib_vec_back`
- `mlib_vec_data`

## Capacity

- `mlib_vec_reserve`
- `mlib_vec_resize`
- `mlib_vec_shrink_to_fit`

## Modifiers

- `mlib_vec_push`
- `mlib_vec_pop`
- `mlib_vec_insert`
- `mlib_vec_remove`
- `mlib_vec_erase`
- `mlib_vec_clear`

## Information

- `mlib_vec_size`
- `mlib_vec_capacity`
- `mlib_vec_item_size`
- `mlib_vec_empty`

## Utilities

- `mlib_vec_swap`
- `mlib_vec_copy`
- `mlib_vec_destroy`

## Iteration

Includes multiple forward and reverse iteration macros for indexes, pointers, values, and capacities.

---
