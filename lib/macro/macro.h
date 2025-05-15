/*
 * @version 0.1.0
 * @copyright 2025 sun0day
 * @description Macro for TANC
 */

#ifndef TANC_MACRO_H
#define TANC_MACRO_H

#include <stddef.h>

#ifndef TC_ALLOCATOR
#define TC_ALLOCATOR malloc, free
#endif

#ifndef _tc_get_alloc
#define _tc_get_alloc(...) __tc_get_alloc(__VA_ARGS__)
#define __tc_get_alloc(_malloc, _free) _malloc
#endif

#ifndef _tc_get_free
#define _tc_get_free(...) __tc_get_free(__VA_ARGS__)
#define __tc_get_free(_malloc, _free) _free
#endif

/*
 * Get the parent object pointer from its property,
 * it's type-unsafe and not applied to NULL ptr
 */
#ifndef tc_container_of
#define tc_container_of(ptr, type, prop) \
  ((type *)((char *)(ptr) - offsetof(type, prop)))
#endif

/*
 * Match 1 - 5 variadic arguments
 */
#ifndef tc_args_of
#define tc_args_of(_1, _2, _3, _4, _5, Name, ...) Name
#endif

typedef void *(*malloc_f)(size_t);
typedef void (*free_f)(void *);

#endif