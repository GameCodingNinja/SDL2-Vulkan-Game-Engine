//
//    FILE NAME:       point.h
//    DESCRIPTION:     point class
//

// Physical component dependency
#include "point.h"

// STD lib dependencies
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

// Forward declarations
static void CrossProduct( const point_t * const self, const point_t * const point, point_t * const result );
static void Normalize( point_t * const point );
static float LengthSquared( const point_t * const point );
static float Length( const point_t * const point );

//
// DESC: allocate type
//
point_t * Point_alloc(float x, float y, float z)
{
    point_t *point;
    if((point = malloc(sizeof *point)) != NULL)
        Point_init(point, x, y, z);

    return point;
}

//
// DESC: Init point
//
void Point_init(point_t * const point, float x, float y, float z)
{
    point->crossProduct = CrossProduct;
    point->normalize = Normalize;
    point->length = Length;
    point->lengthSquared = LengthSquared;
    point->x = x;
    point->y = y;
    point->z = z;
}

//
// DESC: Get the cross product
//
static void CrossProduct( const point_t * const self, const point_t * const point, point_t * const result )
{
    result->x = (self->y * point->z) - (point->y * self->z);
    result->y = (self->z * point->x) - (point->z * self->x);
    result->z = (self->x * point->y) - (point->x * self->y);
    Normalize( result );
}

//
// DESC: normalize this point
//
static void Normalize( point_t * const point )
{
    float length = Length( point );

    if( length != 0.0f )
    {
        point->x /= length;
        point->y /= length;
        point->z /= length;
    }
}

//
// DESC: Get the squared length of the point from the origin
//
static float LengthSquared( const point_t * const point )
{
    return ( point->x * point->x ) +  ( point->y * point->y ) + ( point->z * point->z );
}

//
// DESC: Get the length of the point from the origin
//
static float Length( const point_t * const point )
{
    return sqrtf( LengthSquared( point ) );
}
