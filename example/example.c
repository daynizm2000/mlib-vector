// A simple example demonstrating the basic usage of MLib Vector


#include "../vector/vector.h"
#include <stdio.h>


int main(void)
{
        int *p;


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