//
//    FILE NAME:       point.h
//    DESCRIPTION:     point class
//

#pragma once

typedef struct point point_t;
struct point
{
    void (* crossProduct)( const point_t * const self, const point_t * const point, point_t * const result );
    void (* normalize)( point_t * const point);
    float (* length)( const point_t * const point);
    float (* lengthSquared)( const point_t * const point);
    float x, y, z;
};

// allocate type
point_t * Point_alloc(float x, float y, float z);

// Init point
void Point_init(point_t * const point, float x, float y, float z);