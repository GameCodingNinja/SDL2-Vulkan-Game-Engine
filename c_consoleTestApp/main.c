/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: howie
 *
 * Created on May 27, 2018, 9:18 AM
 */

/*#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

int main()
{
    for( int i = 0; i < 10; ++i )
        printf("value: %d\n", i);
    
    for( int i = 0; i < 20; ++i )
        printf("value: %d\n", i);
    
    return 0;
}*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

int main()
{
    clock_t start = clock();
    
    printf("Test started...\n");
    
    int64_t value = 0;
    int64_t test;
    int64_t maxValue = 10000000;
    int64_t counter = 0;
    
    do
    {
        test = ++value;
        ++counter;

        do
        {
            if ((test & 1) == 0)
                test /= 2;
            //if( (test % 2) == 0 )
            //    test /= 2;
            else
                test = (test * 3) + 1;

            ++counter;
        }
        while (test > 1);
    }
    while ((test > 0) && (value < maxValue));
    
    printf( "Execution time: %f, Counter: %ld\n", ((double)(clock() - start) / CLOCKS_PER_SEC), counter );
    
    return 0;
}

