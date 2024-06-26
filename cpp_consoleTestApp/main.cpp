//
//    FILE NAME:       main.c
//    DESCRIPTION:     main entry point for program
//

#include "point.h"
#include "point.hpp"

// STD lib dependencies
#include <stdio.h>

int main( int argc, char* args[] )
{
    // C style point
    {
        point_t point1;
        point_t point2;
        point_t result;

        Point_init(&point1, 68, 241, 609);
        Point_init(&point2, 582, 231, 78);
        Point_init(&result, 0, 0, 0);

        point1.crossProduct(&point1, &point2, &result);

        printf("C output:   %f, %f, %f\n", result.x, result.y, result.z);
    }

    // C++ style template point
    {
        CPoint<float> point1(68, 241, 609), point2(582, 231, 78);

        auto result = point1.getCrossProduct( point2 );

        printf("C++ output: %f, %f, %f\n", result.x, result.y, result.z);
    }

    return 0;
}

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
}*/

