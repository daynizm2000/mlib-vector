// A simple example of using the library (not all interfaces are described)


#include "vector.h"
#include <stdio.h>


int main(void)
{
        size_t end;


        mlib_vec_define(array, int, 3);
        mlib_vec_init(&array);


        mlib_vec_for_each_capacity(&array, i) {
                mlib_vec_push(&array, i);

                printf("Added: %d\n", *mlib_vec_back(&array));
        }


        for (size_t i = 0, end = mlib_vec_size(&array); i < end; i++)
                printf("Removed: %d\n", mlib_vec_pop(&array));


        mlib_vec_destroy(&array);


        return 0;
}