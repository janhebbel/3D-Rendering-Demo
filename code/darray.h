#ifndef CHC_DARRAY_H
#define CHC_DARRAY_H

#include <string.h>
#include <stdlib.h>

typedef struct
{
    size_t length;
    size_t capacity;
}
chc_darray_header;

#define darr_append(a, n) chcda_append((a), (n))
#define darr_setcap(a, n) chcda_setcap((a), (n))
#define darr_setlen(a, n) chcda_setlen((a), (n))
#define darr_length(a)    chcda_length(a)
#define darr_capacity(a)  chcda_capacity(a)
#define darr_free(a)      chcda_free(a)

void *chcda_grow_f(void *arr, size_t element_size, size_t min_cap);

#define chcda_header(a) ((chc_darray_header *)(a) - 1)
#define chcda_grow(a, n) ((a) = chcda_grow_f((a), sizeof(*(a)), (n)))
#define chcda_maybe_grow(a, n) ((!(a) || chcda_header(a)->length + (size_t)(n) > chcda_header(a)->capacity) ? \
    (chcda_grow((a), (n)), 0) : 0)

#define chcda_append(a, v) (chcda_maybe_grow((a), 1), (a)[chcda_header(a)->length++] = (v))
#define chcda_setcap(a, n) (chcda_grow((a), (n)))
#define chcda_setlen(a, n) ((chcda_capacity(a) < (size_t)(n) ? chcda_setcap((a), (size_t)(n)), 0 : 0), \
                           (a) ? chcda_header(a)->length = (size_t)(n) : 0)
#define chcda_length(a)    ((a) ? chcda_header(a)->length : 0)
#define chcda_capacity(a)  ((a) ? chcda_header(a)->capacity : 0)
#define chcda_free(a)      (((a) ? free(chcda_header(a)) : (void)0), (a) = NULL)

#ifdef CHC_DARRAY_IMPLEMENTATION

void *chcda_grow_f(void *arr, size_t element_size, size_t min_cap)
{
    if(min_cap <= chcda_capacity(arr) * 2)
        min_cap = chcda_capacity(arr) * 2;

    void *a = arr ? chcda_header(arr) : arr;

    void *b = realloc(a, element_size * min_cap + sizeof(chc_darray_header));
    b = (char *)b + sizeof(chc_darray_header);

    if(arr == NULL) 
        chcda_header(b)->length = 0;

    chcda_header(b)->capacity = min_cap;

    return(b);
}

#endif

#endif
