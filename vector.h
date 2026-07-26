/*
-------- FULL-API -----------



--Type definition and initialization
mlib_vec_type(type)
mlib_vec_setup(vec, cap)
mlib_vec_define(name, type, cap)
mlib_vec_init(vec)


--Element access
mlib_vec_get(vec, idx)
mlib_vec_set(vec, elem, idx)
mlib_vec_front(vec)
mlib_vec_back(vec)
mlib_vec_data(vec)


--Size and capacity management
mlib_vec_reserve(vec, newcap)
mlib_vec_resize(vec, newsize, ...)
mlib_vec_shrink_to_fit(vec)


--Adding and removing
mlib_vec_push(vec, elem)
mlib_vec_pop(vec)
mlib_vec_insert(vec, elem, idx)
mlib_vec_remove(vec, idx)
mlib_vec_erase(vec, start, end)
mlib_vec_clear(vec)


--Destruction
mlib_vec_destroy(vec)


--Information
mlib_vec_size(vec)
mlib_vec_capacity(vec)
mlib_vec_item_size(vec)
mlib_vec_empty(vec)


--Swap and copy
mlib_vec_swap(vec1, vec2)
mlib_vec_copy(dst, src)


--Iteration (loops)
mlib_vec_for_each(vec, idx)
mlib_vec_for_each_capacity(vec, idx)
mlib_vec_for_each_val(vec, elem)
mlib_vec_for_each_ptr(vec, elem)
mlib_vec_for_each_index(vec, idx)
mlib_vec_for_each_index_capacity(vec, idx)
mlib_vec_for_each_reverse(vec, idx)
mlib_vec_for_each_capacity_reverse(vec, idx)
mlib_vec_for_each_val_reverse(vec, elem)
mlib_vec_for_each_ptr_reverse(vec, elem)
mlib_vec_for_each_index_reverse(vec, idx)



------------------------
*/




#include <memory.h>
#include <stdlib.h>




#define MLIB_VEC_DEFAULT_CAPACITY 32




// UTILS

#define MLIB_VEC_UTIL_SWAP(a, b) do {                                   \
                __typeof__(a) __mlibvec_a = (a);                        \
                __typeof__(b) __mlibvec_b = (b);                        \
                __typeof__(*a) __mlibvec_tmp = *__mlibvec_a;            \
                                                                        \
                *__mlibvec_a = *__mlibvec_b;                            \
                *__mlibvec_b = __mlibvec_tmp;                           \
        } while (0)




// LIBRARY

#define mlib_vec_type(type)                     \
        struct {                                \
                type *data;                     \
                size_t size;                    \
                size_t capacity;                \
                size_t item_size;               \
        }


#define __mlib_vec_setup(vec, cap) do {                                         \
                (vec)->data = NULL;                                             \
                (vec)->size = 0;                                                \
                (vec)->capacity = ((cap)) ? (cap) : MLIB_VEC_DEFAULT_CAPACITY;  \
                (vec)->item_size = sizeof(__typeof__((vec)->data[0]));          \
        } while (0)

#define mlib_vec_setup(vec, cap) do {                           \
                __typeof__(vec) __mlibvec_vec = (vec);          \
                __typeof__(cap) __mlibvec_cap = (cap);          \
                                                                \
                __mlib_vec_setup(__mlibvec_vec, __mlibvec_cap); \
        } while (0)


#define mlib_vec_define(name, type, cap)                                                                \
                mlib_vec_type(type) name = {                                                            \
                        .data = NULL,                                                                   \
                        .size = 0,                                                                      \
                        .capacity = ((cap)) ? (cap) : MLIB_VEC_DEFAULT_CAPACITY,                        \
                        .item_size = sizeof(type)                                                       \
                }


#define __mlib_vec_init(vec) ({                                                 \
                int __mlibvec_init_ret = 0;                                     \
                                                                                \
                if (!(vec)->capacity) {                                         \
                        (vec)->capacity = MLIB_VEC_DEFAULT_CAPACITY;            \
                }                                                               \
                                                                                \
                if (!((vec)->data =                                             \
                        malloc((vec)->item_size * (vec)->capacity))) {          \
                                __mlibvec_init_ret = -1;                        \
                }                                                               \
                                                                                \
                __mlibvec_init_ret;                                             \
        })

#define mlib_vec_init(vec) ({                                                                   \
                int __mlibvec_ret = 0;                                                          \
                __typeof__(vec) __mlibvec_vec = (vec);                                          \
                                                                                                \
                if (!__mlibvec_vec || !__mlibvec_vec->item_size) {                              \
                        __mlibvec_ret = -1;                                                     \
                }                                                                               \
                else {                                                                          \
                        __mlibvec_ret = __mlib_vec_init(__mlibvec_vec);                         \
                }                                                                               \
                                                                                                \
                __mlibvec_ret;                                                                  \
        })


#define mlib_vec_get(vec, idx) ({ \
                __typeof__(vec) __mlibvec_vec = (vec);                                                  \
                __typeof__(idx) __mlibvec_idx = (idx);                                                  \
                                                                                                        \
                (!__mlibvec_vec || !__mlibvec_vec->data) ? (__typeof__(__mlibvec_vec->data[0])){0} :    \
                ((__mlibvec_idx) >= __mlibvec_vec->size) ? (__typeof__(__mlibvec_vec->data[0])){0} :    \
                __mlibvec_vec->data[(__mlibvec_idx)];                                                   \
        })


#define mlib_vec_set(vec, elem, idx) ({                                         \
                __typeof__(vec) __mlibvec_vec = (vec);                          \
                __typeof__(idx) __mlibvec_idx = (idx);                          \
                __typeof__(elem) __mlibvec_elem = (elem);                       \
                                                                                \
                (!__mlibvec_vec) ? -1 :                                         \
                (!__mlibvec_vec->data) ? -1 :                                   \
                (__mlibvec_idx >= __mlibvec_vec->size) ? -1 :                   \
                ((__mlibvec_vec->data[__mlibvec_idx] = __mlibvec_elem), 0);     \
        })


#define __mlib_vec_reserve(vec, newcap) ({                                      \
                int __mlibvec_reserve_ret = 0;                                  \
                __typeof__((vec)->data) __mlibvec_reserve_tmp =                 \
                        realloc((vec)->data, (newcap) * (vec)->item_size);      \
                                                                                \
                if (!__mlibvec_reserve_tmp) {                                   \
                        __mlibvec_reserve_ret = -1;                             \
                }                                                               \
                else {                                                          \
                        (vec)->data = __mlibvec_reserve_tmp;                    \
                        (vec)->capacity = (newcap);                             \
                }                                                               \
                                                                                \
                __mlibvec_reserve_ret;                                          \
        })

#define mlib_vec_reserve(vec, newcap) ({                                                \
                int __mlibvec_ret = 0;                                                  \
                __typeof__(vec) __mlibvec_vec = (vec);                                  \
                __typeof__(newcap) __mlibvec_newcap = (newcap);                         \
                                                                                        \
                if (!__mlibvec_vec) {                                                   \
                        __mlibvec_ret = -1;                                             \
                }                                                                       \
                else if (!__mlibvec_vec->data) {                                        \
                        __mlibvec_vec->capacity = __mlibvec_newcap;                     \
                                                                                        \
                        if (__mlib_vec_init(__mlibvec_vec)) {                           \
                                __mlibvec_ret = -1;                                     \
                        }                                                               \
                }                                                                       \
                else {                                                                  \
                        __mlibvec_ret =                                                 \
                                __mlib_vec_reserve(__mlibvec_vec, __mlibvec_newcap);    \
                }                                                                       \
                                                                                        \
                __mlibvec_ret;                                                          \
        })


#define mlib_vec_push(vec, elem) ({                                                             \
                int __mlibvec_ret = 0;                                                          \
                __typeof__(vec) __mlibvec_vec = (vec);                                          \
                __typeof__(elem) __mlibvec_elem = (elem);                                       \
                                                                                                \
                if (!__mlibvec_vec) {                                                           \
                        __mlibvec_ret = -1;                                                     \
                }                                                                               \
                else if (!__mlibvec_vec->data && __mlib_vec_init(__mlibvec_vec)) {              \
                        __mlibvec_ret = -1;                                                     \
                }                                                                               \
                else if (__mlibvec_vec->size >= __mlibvec_vec->capacity &&                      \
                        __mlib_vec_reserve(__mlibvec_vec, __mlibvec_vec->capacity * 2)) {       \
                                __mlibvec_ret = -1;                                             \
                }                                                                               \
                else {                                                                          \
                        __mlibvec_vec->data[__mlibvec_vec->size] = __mlibvec_elem;              \
                        __mlibvec_vec->size++;                                                  \
                }                                                                               \
                                                                                                \
                __mlibvec_ret;                                                                  \
        })


#define mlib_vec_pop(vec) ({                                                            \
                __typeof__(vec) __mlibvec_vec = (vec);                                  \
                __typeof__(__mlibvec_vec->data[0]) __mlibvec_ret =                      \
                        (__typeof__(__mlibvec_vec->data[0])){0};                        \
                                                                                        \
                if (__mlibvec_vec && __mlibvec_vec->data && __mlibvec_vec->size) {      \
                        __mlibvec_ret = __mlibvec_vec->data[__mlibvec_vec->size - 1];   \
                        __mlibvec_vec->size--;                                          \
                }                                                                       \
                                                                                        \
                __mlibvec_ret;                                                          \
        })


#define mlib_vec_insert(vec, elem, idx) ({                                                      \
                int __mlibvec_ret = 0;                                                          \
                __typeof__(vec) __mlibvec_vec = (vec);                                          \
                __typeof__(elem) __mlibvec_elem = (elem);                                       \
                __typeof__(idx) __mlibvec_idx = (idx);                                          \
                                                                                                \
                if (!__mlibvec_vec || !__mlibvec_vec->data                                      \
                        || __mlibvec_idx > __mlibvec_vec->size) {                               \
                                __mlibvec_ret = -1;                                             \
                }                                                                               \
                else if (__mlibvec_vec->size >= __mlibvec_vec->capacity &&                      \
                        __mlib_vec_reserve(__mlibvec_vec, __mlibvec_vec->capacity * 2)) {       \
                                __mlibvec_ret = -1;                                             \
                }                                                                               \
                else {                                                                          \
                        memmove(&__mlibvec_vec->data[__mlibvec_idx + 1],                        \
                                &__mlibvec_vec->data[__mlibvec_idx],                            \
                                (__mlibvec_vec->size - __mlibvec_idx)                           \
                                        * __mlibvec_vec->item_size);                            \
                                                                                                \
                        __mlibvec_vec->data[__mlibvec_idx] = __mlibvec_elem;                    \
                        __mlibvec_vec->size++;                                                  \
                }                                                                               \
                                                                                                \
                __mlibvec_ret;                                                                  \
        })


#define __mlib_vec_remove(vec, idx) ({                                          \
                __typeof__((vec)->data[0]) __mlibvec_ret = (vec)->data[(idx)];  \
                                                                                \
                if ((idx) != ((vec)->size - 1)) {                               \
                        memmove(&(vec)->data[(idx)],                            \
                                &(vec)->data[(idx)] + 1,                        \
                                ((vec)->size - (idx) - 1) * (vec)->item_size);  \
                }                                                               \
                                                                                \
                (vec)->size--;                                                  \
        })

#define mlib_vec_remove(vec, idx) ({                                                    \
                __typeof__(vec) __mlibvec_vec = (vec);                                  \
                __typeof__(idx) __mlibvec_idx = (idx);                                  \
                __typeof__(__mlibvec_vec->data[0]) __mlibvec_ret =                      \
                        (__typeof__(__mlibvec_vec->data[0])){0};                        \
                                                                                        \
                if (__mlibvec_vec && __mlibvec_idx >= 0                                 \
                                && __mlibvec_idx < __mlibvec_vec->size) {               \
                        __mlibvec_ret =                                                 \
                                __mlib_vec_remove(__mlibvec_vec, __mlibvec_idx);        \
                }                                                                       \
                                                                                        \
                __mlibvec_ret;                                                          \
        })


#define mlib_vec_erase(vec, start, end) ({                                      \
                __typeof__(vec) __mlibvec_vec = (vec);                          \
                __typeof__(start) __mlibvec_start = (start);                    \
                __typeof__(end) __mlibvec_end = (end);                          \
                int __mlibvec_ret = 0;                                          \
                                                                                \
                if (!__mlibvec_vec || !__mlibvec_vec->data                      \
                                || __mlibvec_end < 0 || __mlibvec_start < 0     \
                                || __mlibvec_end > __mlibvec_vec->size          \
                                || __mlibvec_start > __mlibvec_vec->size        \
                                || __mlibvec_start > __mlibvec_end) {           \
                        __mlibvec_ret = -1;                                     \
                }                                                               \
                else {                                                          \
                        memmove(&__mlibvec_vec->data[__mlibvec_start],          \
                                &__mlibvec_vec->data[__mlibvec_end],            \
                                (__mlibvec_vec->size - __mlibvec_end)           \
                                        * __mlibvec_vec->item_size);            \
                                                                                \
                        __mlibvec_vec->size -= __mlibvec_end - __mlibvec_start; \
                }                                                               \
                                                                                \
                __mlibvec_ret;                                                  \
        })


#define mlib_vec_shrink_to_fit(vec) ({                                                          \
                __typeof__(vec) __mlibvec_vec = (vec);                                          \
                int __mlibvec_ret = 0;                                                          \
                                                                                                \
                if (!__mlibvec_vec || !__mlibvec_vec->data) {                                   \
                        __mlibvec_ret = -1;                                                     \
                }                                                                               \
                else {                                                                          \
                        if (__mlibvec_vec->size) {                                              \
                                __mlibvec_ret =                                                 \
                                        __mlib_vec_reserve(__mlibvec_vec, __mlibvec_vec->size); \
                        }                                                                       \
                }                                                                               \
                                                                                                \
                __mlibvec_ret;                                                                  \
        })


#define mlib_vec_resize(vec, newsize, ...) ({                                                           \
                int __mlibvec_ret = 0;                                                                  \
                __typeof__(vec) __mlibvec_vec = (vec);                                                  \
                __typeof__(newsize) __mlibvec_newsize = (newsize);                                      \
                __typeof__(__mlibvec_vec->data[0]) __mlibvec_fill_val =                                 \
                        __VA_OPT__(__VA_ARGS__ +) (__typeof__(__mlibvec_vec->data[0])){0};              \
                                                                                                        \
                if (!__mlibvec_vec) {                                                                   \
                        __mlibvec_ret = -1;                                                             \
                }                                                                                       \
                else {                                                                                  \
                        if (!__mlibvec_vec->data) {                                                     \
                                __mlib_vec_setup(__mlibvec_vec, __mlibvec_newsize);                     \
                        }                                                                               \
                                                                                                        \
                        if (!__mlibvec_vec->data && __mlib_vec_init(__mlibvec_vec)) {                   \
                                __mlibvec_ret = -1;                                                     \
                        }                                                                               \
                        else if (__mlibvec_vec->capacity < __mlibvec_newsize &&                         \
                                        __mlib_vec_reserve(__mlibvec_vec, __mlibvec_newsize)) {         \
                                __mlibvec_ret = -1;                                                     \
                        }                                                                               \
                        else {                                                                          \
                                for (size_t __mlibvec_idx = __mlibvec_vec->size;                        \
                                                __mlibvec_idx < __mlibvec_newsize; __mlibvec_idx++) {   \
                                        __mlibvec_vec->data[__mlibvec_idx] = __mlibvec_fill_val;        \
                                }                                                                       \
                                                                                                        \
                                __mlibvec_vec->size = __mlibvec_newsize;                                \
                        }                                                                               \
                }                                                                                       \
                                                                                                        \
                                                                                                        \
                __mlibvec_ret;                                                                          \
        })


#define mlib_vec_clear(vec) do {                        \
                __typeof__(vec) __mlibvec_vec = (vec);  \
                                                        \
                if (__mlibvec_vec) {                    \
                        __mlibvec_vec->size = 0;        \
                }                                       \
        } while (0)


#define __mlib_vec_destroy(vec) do {            \
                if ((vec)->data) {              \
                        free((vec)->data);      \
                        (vec)->data = NULL;     \
                }                               \
                                                \
                (vec)->capacity = 0;            \
                (vec)->item_size = 0;           \
                (vec)->size =  0;               \
        } while (0)

#define mlib_vec_destroy(vec) do {                              \
                __typeof__(vec) __mlibvec_vec = (vec);          \
                                                                \
                if (__mlibvec_vec) {                            \
                        __mlib_vec_destroy(__mlibvec_vec);      \
                }                                               \
        } while (0)


#define mlib_vec_for_each(vec, p) \
        for ((p) = (vec)->data; (vec)->data && (vec)->size && (p) < ((vec)->data + (vec)->size); (p)++)

#define mlib_vec_for_each_index(vec, idx) \
        for ((idx) = 0; (vec)->data && (idx) < (vec)->size; (idx)++)

#define mlib_vec_for_each_reverse(vec, p) \
        for ((p) = (vec)->data + (vec)->size - 1; (vec)->size && (vec)->data && (p) >= (vec)->data; (p)--)

#define mlib_vec_for_each_index_reverse(vec, idx) \
        for ((idx) = (vec)->size; (vec)->data && (idx) > 0; ) \
                if (--(idx), 1)


#define mlib_vec_size(vec)              ((vec)->size)
#define mlib_vec_capacity(vec)          ((vec)->capacity)
#define mlib_vec_item_size(vec)         ((vec)->item_size)


#define mlib_vec_back(vec) ({                                                           \
                __typeof__(vec) __mlibvec_vec = (vec);                                  \
                                                                                        \
                (__mlibvec_vec->size) ? &__mlibvec_vec->data[__mlibvec_vec->size - 1]   \
                : (__typeof__(__mlibvec_vec->data[0])*)0;                               \
        })


#define mlib_vec_front(vec) ({ \
                __typeof__(vec) __mlibvec_vec = (vec);                                  \
                                                                                        \
                (__mlibvec_vec->size) ? &__mlibvec_vec->data[0]                         \
                : (__typeof__(__mlibvec_vec->data[0])*)0;                               \
        })


#define mlib_vec_empty(vec)     (((vec)->size) ? 0 : 1)
#define mlib_vec_data(vec)      ((vec)->data)


#define mlib_vec_swap(vec1, vec2) do {                                                          \
                __typeof__(vec1) __mlibvec_vec1 = (vec1);                                       \
                __typeof__(vec2) __mlibvec_vec2 = (vec2);                                       \
                                                                                                \
                MLIB_VEC_UTIL_SWAP(&__mlibvec_vec1->data, &__mlibvec_vec2->data);               \
                MLIB_VEC_UTIL_SWAP(&__mlibvec_vec1->size, &__mlibvec_vec2->size);               \
                MLIB_VEC_UTIL_SWAP(&__mlibvec_vec1->capacity, &__mlibvec_vec2->capacity);       \
                MLIB_VEC_UTIL_SWAP(&__mlibvec_vec1->item_size, &__mlibvec_vec2->item_size);     \
        } while (0)


#define mlib_vec_copy(dst, src) ({                                                      \
                __typeof__(dst) __mlibvec_dst = (dst);                                  \
                __typeof__(src) __mlibvec_src = (src);                                  \
                int __mlibvec_ret = 0;                                                  \
                                                                                        \
                if (!__mlibvec_dst || !__mlibvec_src) {                                 \
                        __mlibvec_ret = -1;                                             \
                }                                                                       \
                else {                                                                  \
                        __mlib_vec_destroy(__mlibvec_dst);                              \
                        __mlib_vec_setup(__mlibvec_dst, __mlibvec_src->capacity);       \
                                                                                        \
                        if (__mlib_vec_init(__mlibvec_dst)) {                           \
                                __mlibvec_ret = -1;                                     \
                        }                                                               \
                        else {                                                          \
                                mlib_vec_for_each(__mlibvec_src, __mlibvec_idx) {       \
                                        __mlibvec_dst->data[__mlibvec_idx] =            \
                                                __mlibvec_src->data[__mlibvec_idx];     \
                                }                                                       \
                                                                                        \
                                __mlibvec_dst->size = __mlibvec_src->size;              \
                        }                                                               \
                }                                                                       \
                                                                                        \
                __mlibvec_ret;                                                          \
        })
