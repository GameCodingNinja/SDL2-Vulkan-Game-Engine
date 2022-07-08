
/************************************************************************
*    FILE NAME:       matrix.cpp
*
*    DESCRIPTION:     4x4 Matrix math class
************************************************************************/

// Physical component dependency
#include <utilities/matrix.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <math.h>
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMatrix::CMatrix()
{
    initilizeMatrix();
}

/************************************************************************
*    DESC:  Copy Constructor
************************************************************************/
CMatrix::CMatrix( const CMatrix & obj )
{
    std::memcpy( matrix, obj.matrix, sizeof(matrix) );
}

/************************************************************************
*    DESC:  Copy Constructor
************************************************************************/
CMatrix::CMatrix( float mat[mMax] )
{
    std::memcpy( matrix, mat, sizeof(matrix) );
}

/************************************************************************
*    DESC:  Copy from another matrix
************************************************************************/
void CMatrix::copy( const CMatrix & obj )
{
    std::memcpy( matrix, obj.matrix, sizeof(matrix) );
}

/************************************************************************
*    DESC:  Reset the matrix to the identity matrix
************************************************************************/
void CMatrix::initilizeMatrix()
{
    initIdentityMatrix( matrix );
}

/************************************************************************
*    DESC:  Initializes a specific matrix to the identity matrix
*
*    param: float mat[16] - Matrix array
************************************************************************/
void CMatrix::initIdentityMatrix( float mat[16] )
{
    // Initializes a specific matrix to the identity matrix:
    mat[0]  = 1.0f;   mat[1] = 0.0f;   mat[2]  = 0.0f;   mat[3] = 0.0f;
    mat[4]  = 0.0f;   mat[5] = 1.0f;   mat[6]  = 0.0f;   mat[7] = 0.0f;
    mat[8]  = 0.0f;   mat[9] = 0.0f;   mat[10] = 1.0f;  mat[11] = 0.0f;
    mat[12] = 0.0f;  mat[13] = 0.0f;   mat[14] = 0.0f;  mat[15] = 1.0f;
}

/************************************************************************
*    DESC:  Clear translation data from the matrix
************************************************************************/
void CMatrix::clearTranlate()
{
    // Initializes a specific matrix to the identity matrix:
    matrix[12] = 0.0f;  matrix[13] = 0.0f;   matrix[14] = 0.0f;
}

/************************************************************************
*    DESC:  Merge matrix into master matrix
*
*    parm:  float newMatrix[16] - Matrix array
************************************************************************/
void CMatrix::mergeMatrix( const float mat[mMax] )
{
    float temp[mMax];
    
    // Converting to a two demensional array much faster for the 4 loops
    float (*Mat)[4] = (float (*)[4]) mat;
    float (*Matrix)[4] = (float (*)[4]) matrix;
    float (*Temp)[4] = (float (*)[4]) temp;

    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
            Temp[i][j] = (Matrix[i][0] * Mat[0][j])
            + (Matrix[i][1] * Mat[1][j])
            + (Matrix[i][2] * Mat[2][j])
            + (Matrix[i][3] * Mat[3][j]);
        }
    }

    // Copy temp to master Matrix
    std::memcpy( matrix, temp, sizeof(temp) );

}  // MergeMatrix

void CMatrix::mergeMatrix( const CMatrix & obj )
{
    mergeMatrix( obj.matrix );
}

/************************************************************************
*    DESC:  Merge source matrix into destination matrix.
*
*    param: float Dest[16] - Destination Matric
*           float Source[16] - Source Matrix
************************************************************************/
void CMatrix::mergeMatrices( float dest[mMax], const float source[mMax] )
{
    float temp[mMax];
    
    // Converting to a two demensional array much faster for the 4 loops
    float (*Dest)[4] = (float (*)[4]) dest;
    float (*Source)[4] = (float (*)[4]) source;
    float (*Temp)[4] = (float (*)[4]) temp;

    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
            Temp[i][j] = ( Source[i][0] * Dest[0][j] )
            + ( Source[i][1] * Dest[1][j] )
            + ( Source[i][2] * Dest[2][j] )
            + ( Source[i][3] * Dest[3][j] );
        }
    }

    // Copy Temp to Dest
    std::memcpy( dest, temp, sizeof(temp) );
}

/************************************************************************
*    DESC:  Generate 3D rotation matrix.
*
*    param: const CRadian & point - rotation point
*
*	 NOTE:	There is some mysterious problem with my new rotate function
*			that can only be seen in the game template at the moment.
*			In the template, the robots' hands are getting flattened and
*			I can't figure out why. I've tried everything to reproduce this
*			rotationally screwed up matrix but couldn't
************************************************************************/
void CMatrix::rotate( const CPoint<float> & radian )
{
    int flags = NO_ROT;
    float rMatrix[ 16 ];

    // init the rotation matrix
    initIdentityMatrix( rMatrix );

    // Apply Z rotation
    if( !radian.isZEmpty() )
    {
        rotateZ( rMatrix, radian.z, flags );
        flags |= ROT_Z;
    }

    // Apply Y rotation
    if( !radian.isYEmpty() )
    {
        rotateY( rMatrix, radian.y, flags );
        flags |= ROT_Y;
    }

    // Apply X rotation
    if( !radian.isXEmpty() )
    {
        rotateX( rMatrix, radian.x, flags );
        flags |= ROT_X;
    }

    // Merg the rotation into the master matrix
    mergeMatrix( rMatrix );
}

/************************************************************************
*    DESC:  Get the Z rotation of the matrix
************************************************************************/
float CMatrix::getZRot( bool inDegrees ) const
{
    if( inDegrees )
        return -atan2( matrix[m10], matrix[m00] ) * defs_RAD_TO_DEG;

    return -atan2( matrix[m10], matrix[m00] );
}

/************************************************************************
*    DESC:  Create 3D translation matrix
*
*    param: const CPoint & point - translation point
************************************************************************/
void CMatrix::translate( const CPoint<float> & point )
{
    matrix[12] += point.x;
    matrix[13] += point.y;
    matrix[14] += point.z;
}

void CMatrix::translate( const CSize<int16_t> & size )
{
    matrix[12] += static_cast<float>(size.w);
    matrix[13] += static_cast<float>(size.h);
}

/************************************************************************
*    DESC:  Function designed to transform a vertex using
*           the master matrix
************************************************************************/
void CMatrix::transform( CPoint<float> & dest, const CPoint<float> & source ) const
{
    // Transform vertex by master matrix:
    dest.x = ( source.x * matrix[ 0 ] )
           + ( source.y * matrix[ 4 ] )
           + ( source.z * matrix[ 8 ] )
           + matrix[ 12 ];

    dest.y = ( source.x * matrix[ 1 ] )
           + ( source.y * matrix[ 5 ] )
           + ( source.z * matrix[ 9 ] )
           + matrix[ 13 ];

    dest.z = ( source.x * matrix[ 2 ] )
           + ( source.y * matrix[ 6 ] )
           + ( source.z * matrix[ 10 ] )
           + matrix[ 14 ];
}

/************************************************************************
*    DESC:  Function designed to transform a vertex using
*           the master matrix
************************************************************************/
void CMatrix::transform( CPoint<float> * pDest, const CPoint<float> * pSource ) const
{
    // Transform vertex by master matrix:
    pDest->x = ( pSource->x * matrix[ 0 ] )
           + ( pSource->y * matrix[ 4 ] )
           + ( pSource->z * matrix[ 8 ] )
           + matrix[ 12 ];

    pDest->y = ( pSource->x * matrix[ 1 ] )
           + ( pSource->y * matrix[ 5 ] )
           + ( pSource->z * matrix[ 9 ] )
           + matrix[ 13 ];

    pDest->z = ( pSource->x * matrix[ 2 ] )
           + ( pSource->y * matrix[ 6 ] )
           + ( pSource->z * matrix[ 10 ] )
           + matrix[ 14 ];
}

/************************************************************************
*    DESC:  Function designed to transform a vertex using
*           the master matrix
************************************************************************/
void CMatrix::transform( CRect<float> & dest, const CRect<float> & source ) const
{
    // Transform vertex by master matrix:
    dest.x1 = ( source.x1 * matrix[ 0 ] )
            + ( source.y1 * matrix[ 4 ] )
            + matrix[ 12 ];

    dest.y1 = ( source.x1 * matrix[ 1 ] )
            + ( source.y1 * matrix[ 5 ] )
            + matrix[ 13 ];

    dest.x2 = ( source.x2 * matrix[ 0 ] )
            + ( source.y2 * matrix[ 4 ] )
            + matrix[ 12 ];

    dest.y2 = ( source.x2 * matrix[ 1 ] )
            + ( source.y2 * matrix[ 5 ] )
            + matrix[ 13 ];
}

/************************************************************************
*    DESC:  Function designed to transform a normal. Normals don't have
*           a position, only direction so we only use the rotation
*           portion of the matrix.
************************************************************************/
void CMatrix::transform( CNormal<float> & dest, const CNormal<float> & source ) const
{
    // Transform vertex by master matrix:
    dest.x = ( source.x * matrix[ 0 ])
           + ( source.y * matrix[ 4 ])
           + ( source.z * matrix[ 8 ]);

    dest.y = ( source.x * matrix[ 1 ])
           + ( source.y * matrix[ 5 ])
           + ( source.z * matrix[ 9 ]);

    dest.z = ( source.x * matrix[ 2 ])
           + ( source.y * matrix[ 6 ])
           + ( source.z * matrix[ 10 ]);
}

/************************************************************************
*    DESC:  Function designed to transform a normal. Normals don't have
*           a position, only direction so we only use the rotation
*           portion of the matrix.
************************************************************************/
void CMatrix::transform3x3( CPoint<float> & dest, const CPoint<float> & source ) const
{
    // Transform vertex by master matrix:
    dest.x = ( source.x * matrix[ 0 ])
           + ( source.y * matrix[ 4 ])
           + ( source.z * matrix[ 8 ]);

    dest.y = ( source.x * matrix[ 1 ])
           + ( source.y * matrix[ 5 ])
           + ( source.z * matrix[ 9 ]);

    dest.z = ( source.x * matrix[ 2 ])
           + ( source.y * matrix[ 6 ])
           + ( source.z * matrix[ 10 ]);

}

/************************************************************************
*    DESC:  Function designed to transform a vertex using
*           the master matrix
************************************************************************/
void CMatrix::transform( CQuad & dest, const CQuad & source ) const
{
    // Transform vertex by master matrix:
    for( int i = 0; i < 4; ++i )
        transform( dest.point[i], source.point[i] );
}

/************************************************************************
*    DESC:  Get the transpose of a matrix
************************************************************************/
CMatrix CMatrix::getTransposeMatrix() const
{
    CMatrix tmp;

    tmp[m00] = matrix[m00];

    tmp[m01] = matrix[m10];
    tmp[m10] = matrix[m01];

    tmp[m11] = matrix[m11];

    tmp[m02] = matrix[m20];
    tmp[m20] = matrix[m02];

    tmp[m12] = matrix[m21];
    tmp[m21] = matrix[m12];

    tmp[m03] = matrix[m30];
    tmp[m30] = matrix[m03];

    tmp[m22] = matrix[m22];

    tmp[m31] = matrix[m13];
    tmp[m13] = matrix[m31];

    tmp[m32] = matrix[m23];
    tmp[m23] = matrix[m32];

    tmp[m33] = matrix[m33];

    return tmp;
}

/************************************************************************
*    DESC:   Get matrix point in space
************************************************************************/
CPoint<float> CMatrix::getMatrixPoint()
{
    CPoint<float> source;
    CPoint<float> dest;
    float tempMat[16];

    // Copy over the matrix to restore after operation
    std::memcpy( tempMat, matrix, sizeof(tempMat) );

    // Get the translation part of the matrix and invert the sign
    source.x = -matrix[m30];
    source.y = matrix[m31];
    source.z = -matrix[m32];

    // Inverse the matrix
    inverse();

    // Transform only by the 3x3 part of the matrix
    transform3x3( dest, source );

    // Restore the matrix back to what it was
    std::memcpy( matrix, tempMat, sizeof(matrix) );

    return dest;
}

/************************************************************************
*    DESC:   Get matrix rotation
************************************************************************/
CPoint<float> CMatrix::getMatrixRotation()
{
    CPoint<float> tmp;

    // singularity at north pole
    if( matrix[m10] > 0.998f )
    {
        tmp.x = M_PI_2;
        tmp.y = atan2( matrix[m02], matrix[m22] );
        tmp.z = 0;
        return tmp;
    }

    // singularity at south pole
    if( matrix[m10] < -0.998f )
    {
        tmp.x = -M_PI_2;
        tmp.y = atan2( matrix[m02], matrix[m22] );
        tmp.z = 0;
        return tmp;
    }

    tmp.x = asin( matrix[m10] );
    tmp.y = atan2( -matrix[m20], matrix[m00] );
    tmp.z = atan2( -matrix[m12], matrix[m11] );

    return tmp;
}

/************************************************************************
*    DESC: Set the scale
************************************************************************/
void CMatrix::setScale( const CPoint<float> & point )
{
    // Initialize scaling matrix:
    matrix[0]  *= point.x;
    matrix[5]  *= point.y;
    matrix[10] *= point.z;
}

void CMatrix::setScale( const CSize<float> & size )
{
    // Initialize scaling matrix:
    matrix[0]  *= size.w;
    matrix[5]  *= size.h;
    matrix[10] *= 1.f;
}

void CMatrix::setScale( float scale )
{
    // Initialize scaling matrix:
    matrix[0]  *= scale;
    matrix[5]  *= scale;
    matrix[10] *= scale;
}

void CMatrix::setScale( float scaleX, float scaleY )
{
    // Initialize scaling matrix:
    matrix[0]  *= scaleX;
    matrix[5]  *= scaleY;
    matrix[10] *= 1.f;
}

void CMatrix::setScale( float scaleX, float scaleY, float scaleZ )
{
    // Initialize scaling matrix:
    matrix[0]  *= scaleX;
    matrix[5]  *= scaleY;
    matrix[10] *= scaleZ;
}

/************************************************************************
*    DESC: Get the scale
************************************************************************/
CPoint<float> CMatrix::getScale() const
{
    return CPoint<float>(matrix[0], matrix[5], matrix[10]);
}


/************************************************************************
*    DESC: Scale the matrix
************************************************************************/
void CMatrix::scale( const CPoint<float> & point )
{
    mergeScale( point.x, point.y, point.z );
}

void CMatrix::scale( const CSize<float> & size )
{
    mergeScale( size.w, size.h, 1.f );
}

void CMatrix::scale( float scale )
{
    mergeScale( scale, scale, scale );
}

void CMatrix::scale( float scaleX, float scaleY )
{
    mergeScale( scaleY, scaleY, 1.0f );
}

void CMatrix::scale( float scaleX, float scaleY, float scaleZ )
{
    mergeScale( scaleY, scaleY, scaleZ );
}

/************************************************************************
*    DESC:  scale and merge
************************************************************************/
void CMatrix::mergeScale( float x, float y, float z )
{
    float temp[ 16 ];

    // init the matrix
    initIdentityMatrix( temp );

    // Initialize scaling matrix:
    temp[0]  = x;
    temp[5]  = y;
    temp[10] = z;

    // Merge the scale into the master matrix
    mergeMatrix( temp );
}

/************************************************************************
*    DESC:  Inverses this matrix. Assumes that the last column is [0 0 0 1]
************************************************************************/
bool CMatrix::inverse()
{
    const float EPSILON_E5((float)(1E-5));

    float det =  ( matrix[m00] * ( matrix[m11] * matrix[m22] - matrix[m12] * matrix[m21] ) -
               matrix[m01] * ( matrix[m10] * matrix[m22] - matrix[m12] * matrix[m20] ) +
               matrix[m02] * ( matrix[m10] * matrix[m21] - matrix[m11] * matrix[m20] ) );

    // test determinate to see if it's 0
    if( fabs(det) < EPSILON_E5 )
    {
        return false;
    }

    float det_inv  = 1.0f / det;

    float tmp[ 16 ];

    // Initialize translation matrix
    initIdentityMatrix( tmp );

    tmp[m00] =  det_inv * ( matrix[m11] * matrix[m22] - matrix[m12] * matrix[m21] );
    tmp[m01] = -det_inv * ( matrix[m01] * matrix[m22] - matrix[m02] * matrix[m21] );
    tmp[m02] =  det_inv * ( matrix[m01] * matrix[m12] - matrix[m02] * matrix[m11] );
    tmp[m03] = 0.0f; // always 0

    tmp[m10] = -det_inv * ( matrix[m10] * matrix[m22] - matrix[m12] * matrix[m20] );
    tmp[m11] =  det_inv * ( matrix[m00] * matrix[m22] - matrix[m02] * matrix[m20] );
    tmp[m12] = -det_inv * ( matrix[m00] * matrix[m12] - matrix[m02] * matrix[m10] );
    tmp[m13] = 0.0f; // always 0

    tmp[m20] =  det_inv * ( matrix[m10] * matrix[m21] - matrix[m11] * matrix[m20] );
    tmp[m21] = -det_inv * ( matrix[m00] * matrix[m21] - matrix[m01] * matrix[m20] );
    tmp[m22] =  det_inv * ( matrix[m00] * matrix[m11] - matrix[m01] * matrix[m10] );
    tmp[m23] = 0.0f; // always 0

    tmp[m30] = -( matrix[m30] * matrix[m00] + matrix[m31] * matrix[m10] + matrix[m32] * matrix[m20] );
    tmp[m31] = -( matrix[m30] * matrix[m01] + matrix[m31] * matrix[m11] + matrix[m32] * matrix[m21] );
    tmp[m32] = -( matrix[m30] * matrix[m02] + matrix[m31] * matrix[m12] + matrix[m32] * matrix[m22] );
    tmp[m33] = 1.0f; // always 0

    // Copy Temp to Dest
    std::memcpy( matrix, tmp, sizeof(tmp) );

    return true;
}

/************************************************************************
*    DESC:  Inverses this matrix. Assumes that the last column is [0 0 0 1]
************************************************************************/
bool CMatrix::invert()
{
    float inv[16];

    inv[0] = matrix[5]  * matrix[10] * matrix[15] -
             matrix[5]  * matrix[11] * matrix[14] -
             matrix[9]  * matrix[6]  * matrix[15] +
             matrix[9]  * matrix[7]  * matrix[14] +
             matrix[13] * matrix[6]  * matrix[11] -
             matrix[13] * matrix[7]  * matrix[10];

    inv[4] = -matrix[4]  * matrix[10] * matrix[15] +
              matrix[4]  * matrix[11] * matrix[14] +
              matrix[8]  * matrix[6]  * matrix[15] -
              matrix[8]  * matrix[7]  * matrix[14] -
              matrix[12] * matrix[6]  * matrix[11] +
              matrix[12] * matrix[7]  * matrix[10];

    inv[8] = matrix[4]  * matrix[9]  * matrix[15] -
             matrix[4]  * matrix[11] * matrix[13] -
             matrix[8]  * matrix[5]  * matrix[15] +
             matrix[8]  * matrix[7]  * matrix[13] +
             matrix[12] * matrix[5]  * matrix[11] -
             matrix[12] * matrix[7]  * matrix[9];

    inv[12] = -matrix[4]  * matrix[9]  * matrix[14] +
               matrix[4]  * matrix[10] * matrix[13] +
               matrix[8]  * matrix[5]  * matrix[14] -
               matrix[8]  * matrix[6]  * matrix[13] -
               matrix[12] * matrix[5]  * matrix[10] +
               matrix[12] * matrix[6]  * matrix[9];

    inv[1] = -matrix[1]  * matrix[10] * matrix[15] +
              matrix[1]  * matrix[11] * matrix[14] +
              matrix[9]  * matrix[2]  * matrix[15] -
              matrix[9]  * matrix[3]  * matrix[14] -
              matrix[13] * matrix[2]  * matrix[11] +
              matrix[13] * matrix[3]  * matrix[10];

    inv[5] = matrix[0]  * matrix[10] * matrix[15] -
             matrix[0]  * matrix[11] * matrix[14] -
             matrix[8]  * matrix[2]  * matrix[15] +
             matrix[8]  * matrix[3]  * matrix[14] +
             matrix[12] * matrix[2]  * matrix[11] -
             matrix[12] * matrix[3]  * matrix[10];

    inv[9] = -matrix[0]  * matrix[9]  * matrix[15] +
              matrix[0]  * matrix[11] * matrix[13] +
              matrix[8]  * matrix[1]  * matrix[15] -
              matrix[8]  * matrix[3]  * matrix[13] -
              matrix[12] * matrix[1]  * matrix[11] +
              matrix[12] * matrix[3]  * matrix[9];

    inv[13] = matrix[0]  * matrix[9]  * matrix[14] -
              matrix[0]  * matrix[10] * matrix[13] -
              matrix[8]  * matrix[1]  * matrix[14] +
              matrix[8]  * matrix[2]  * matrix[13] +
              matrix[12] * matrix[1]  * matrix[10] -
              matrix[12] * matrix[2]  * matrix[9];

    inv[2] = matrix[1]  * matrix[6] * matrix[15] -
             matrix[1]  * matrix[7] * matrix[14] -
             matrix[5]  * matrix[2] * matrix[15] +
             matrix[5]  * matrix[3] * matrix[14] +
             matrix[13] * matrix[2] * matrix[7] -
             matrix[13] * matrix[3] * matrix[6];

    inv[6] = -matrix[0]  * matrix[6] * matrix[15] +
              matrix[0]  * matrix[7] * matrix[14] +
              matrix[4]  * matrix[2] * matrix[15] -
              matrix[4]  * matrix[3] * matrix[14] -
              matrix[12] * matrix[2] * matrix[7] +
              matrix[12] * matrix[3] * matrix[6];

    inv[10] = matrix[0]  * matrix[5] * matrix[15] -
              matrix[0]  * matrix[7] * matrix[13] -
              matrix[4]  * matrix[1] * matrix[15] +
              matrix[4]  * matrix[3] * matrix[13] +
              matrix[12] * matrix[1] * matrix[7] -
              matrix[12] * matrix[3] * matrix[5];

    inv[14] = -matrix[0]  * matrix[5] * matrix[14] +
               matrix[0]  * matrix[6] * matrix[13] +
               matrix[4]  * matrix[1] * matrix[14] -
               matrix[4]  * matrix[2] * matrix[13] -
               matrix[12] * matrix[1] * matrix[6] +
               matrix[12] * matrix[2] * matrix[5];

    inv[3] = -matrix[1] * matrix[6] * matrix[11] +
              matrix[1] * matrix[7] * matrix[10] +
              matrix[5] * matrix[2] * matrix[11] -
              matrix[5] * matrix[3] * matrix[10] -
              matrix[9] * matrix[2] * matrix[7] +
              matrix[9] * matrix[3] * matrix[6];

    inv[7] = matrix[0] * matrix[6] * matrix[11] -
             matrix[0] * matrix[7] * matrix[10] -
             matrix[4] * matrix[2] * matrix[11] +
             matrix[4] * matrix[3] * matrix[10] +
             matrix[8] * matrix[2] * matrix[7] -
             matrix[8] * matrix[3] * matrix[6];

    inv[11] = -matrix[0] * matrix[5] * matrix[11] +
               matrix[0] * matrix[7] * matrix[9] +
               matrix[4] * matrix[1] * matrix[11] -
               matrix[4] * matrix[3] * matrix[9] -
               matrix[8] * matrix[1] * matrix[7] +
               matrix[8] * matrix[3] * matrix[5];

    inv[15] = matrix[0] * matrix[5] * matrix[10] -
              matrix[0] * matrix[6] * matrix[9] -
              matrix[4] * matrix[1] * matrix[10] +
              matrix[4] * matrix[2] * matrix[9] +
              matrix[8] * matrix[1] * matrix[6] -
              matrix[8] * matrix[2] * matrix[5];

    float det = matrix[0] * inv[0] + matrix[1] * inv[4] + matrix[2] * inv[8] + matrix[3] * inv[12];

    if( det == 0 )
        return false;

    det = 1.0 / det;

    for( int i = 0; i < 16; i++ )
        matrix[i] = inv[i] * det;

    return true;
}

/************************************************************************
*    DESC:  Inverse the Y.
************************************************************************/
void CMatrix::invertX()
{
    matrix[12] = -matrix[12];
}

/************************************************************************
*    DESC:  Inverse the Y.
************************************************************************/
void CMatrix::invertY()
{
    matrix[13] = -matrix[13];
}

/************************************************************************
*    DESC:  Inverse the Z.
************************************************************************/
void CMatrix::invertZ()
{
    matrix[14] = -matrix[14];
}

/************************************************************************
*    DESC:  Set the quaternion
************************************************************************/
void CMatrix::set( const CQuaternion & quat )
{
    float temp[mMax];

    // Initialize translation matrix
    initIdentityMatrix( temp );

    float x2 = quat.x * quat.x;
    float y2 = quat.y * quat.y;
    float z2 = quat.z * quat.z;
    float xy = quat.x * quat.y;
    float xz = quat.x * quat.z;
    float yz = quat.y * quat.z;
    float wx = quat.w * quat.x;
    float wy = quat.w * quat.y;
    float wz = quat.w * quat.z;

    temp[m00] = 1.0f - 2.0f * (y2 + z2);
    temp[m01] = 2.0f * (xy - wz);
    temp[m02] = 2.0f * (xz + wy);

    temp[m10] = 2.0f * (xy + wz);
    temp[m11] = 1.0f - 2.0f * (x2 + z2);
    temp[m12] = 2.0f * (yz - wx);

    temp[m20] = 2.0f * (xz - wy);
    temp[m21] = 2.0f * (yz + wx);
    temp[m22] = 1.0f - 2.0f * (x2 + y2);

    // Merge matrix with master matrix:
    mergeMatrix( temp );
}

/************************************************************************
*    DESC:  Create the matrix based on where the camera is looking
************************************************************************/
void CMatrix::lookAt( const CPoint<float> & pos, const CPoint<float> & target, const CPoint<float> & cameraUp )
{
    CPoint<float> zAxis = target - pos;
    zAxis.normalize();

    CPoint<float> xAxis = cameraUp.getCrossProduct( zAxis );
    xAxis.normalize();

    CPoint<float> yAxis = zAxis.getCrossProduct( xAxis );

    matrix[m00] = xAxis.x;  matrix[m01] = yAxis.x;  matrix[m02] = zAxis.x;  matrix[m03] = 0.f;
    matrix[m10] = xAxis.y;  matrix[m11] = yAxis.y;  matrix[m12] = zAxis.y;  matrix[m13] = 0.f;
    matrix[m20] = xAxis.z;  matrix[m21] = yAxis.z;  matrix[m22] = zAxis.z;  matrix[m23] = 0.f;

    matrix[m30] = -pos.getDotProduct(xAxis);
    matrix[m31] = -pos.getDotProduct(yAxis);
    matrix[m32] = -pos.getDotProduct(zAxis);
    matrix[m33] = 1.f;
}

/************************************************************************
*    DESC:  Calculate an orthographic matrix
************************************************************************/
void CMatrix::orthographicRH( float w, float h, float zn, float zf )
{
    // Formula for a right handed orthographic matrix
    //	2/w  0    0           0
    //  0    2/h  0           0
    //  0    0    1/(zn-zf)   0
    //  0    0    zn/(zn-zf)  1

    matrix[m00] = 2 / w;
    matrix[m11] = 2 / h;
    matrix[m22] = 1 / (zn-zf);
    matrix[m32] = zn / (zn-zf);
}

void CMatrix::orthographicLH( float w, float h, float zn, float zf )
{
    // Formula for a left handed orthographic matrix
    //  2/w  0    0           0
    //  0    2/h  0           0
    //  0    0    1/(zf-zn)   0
    //  0    0   -zn/(zf-zn)  1

    matrix[m00] = 2 / w;
    matrix[m11] = 2 / h;
    matrix[m22] = 1 / (zf-zn);
    matrix[m32] = -zn / (zf-zn);
}

/************************************************************************
*    DESC:  Calculate an perspective matrix
************************************************************************/
void CMatrix::perspectiveFovRH( float fovy, float aspect, float zn, float zf )
{
    // Formula for a right handed perspective matrix
    //  yScale = cot(fovY/2)
    //  xScale = yScale / aspect ratio
    //  xScale     0          0              0
    //  0        yScale       0              0
    //  0        0        zf/(zn-zf)        -1
    //  0        0        zn*zf/(zn-zf)      0

    float yScale = 1 / tan(fovy/2);
    float xScale = yScale / aspect;

    matrix[m00] = xScale;
    matrix[m11] = yScale;
    matrix[m22] = zf / (zn-zf);
    matrix[m23] = -1;
    matrix[m32] = zn * zf / (zn-zf);
}

void CMatrix::perspectiveFovLH( float fovy, float aspect, float zn, float zf )
{
    // Formula for a left handed perspective matrix
    //  yScale = cot(fovY/2)
    //  xScale = yScale / aspect ratio
    //  xScale     0          0               0
    //  0        yScale       0               0
    //  0          0       zf/(zf-zn)         1
    //  0          0       -zn*zf/(zf-zn)     0

    float yScale = 1 / tan(fovy/2);
    float xScale = yScale / aspect;

    matrix[m00] = xScale;
    matrix[m11] = yScale;
    matrix[m22] = zf / (zf-zn);
    matrix[m23] = 1;
    matrix[m32] = -zn * zf / (zf-zn);
}

/************************************************************************
*    DESC:	Multiply the matrices only using the rotation/scale portion
************************************************************************/
void CMatrix::multiply3x3( const CMatrix & obj )
{
    float tmp[mMax];

    // init the matrix
    initIdentityMatrix( tmp );

    for( int i = 0; i < 3; ++i )
    {
        for( int j = 0; j < 3; ++j )
        {
            tmp[(i*4)+j] = ( matrix[i*4] * obj[j]   )
                         + ( matrix[(i*4)+1] * obj[4+j] )
                         + ( matrix[(i*4)+2] * obj[8+j] );
        }
    }

    // Copy the tmp matrix to the class's matrix
    std::memcpy( matrix, tmp, sizeof(matrix) );
}

/************************************************************************
*    DESC:  Rotate the matrix along the z axis
************************************************************************/
void CMatrix::rotateZ( float dest[mMax], float value, int rotFlags )
{
    float cosZ = cos(value);
    float sinZ = sin(value);

    dest[0] = cosZ;
    dest[1] = sinZ;
    dest[4] = -sinZ;
    dest[5] = cosZ;
}

/************************************************************************
*    DESC:  Rotate the matrix along the y axis
************************************************************************/
void CMatrix::rotateY( float dest[mMax], float value, int rotFlags )
{
    float cosY = cos(value);
    float sinY = sin(value);

    switch( rotFlags )
    {
        case ROT_Z:
        {
            float tmp0, tmp1, tmp8, tmp9;
            tmp0 = dest[0] * cosY;
            tmp1 = dest[1] * cosY;
            tmp8 = dest[0] * sinY;
            tmp9 = dest[1] * sinY;
            dest[0] = tmp0;
            dest[1] = tmp1;
            dest[2] = -sinY;
            dest[8] = tmp8;
            dest[9] = tmp9;
            dest[10] = cosY;
            break;
        }
        case NO_ROT:
        {
            dest[0]  =  cosY;
            dest[2]  = -sinY;
            dest[8]  =  sinY;
            dest[10] =  cosY;
            break;
        }
    }
}

/************************************************************************
*    DESC:  Rotate the matrix along the x axis
************************************************************************/
void CMatrix::rotateX( float dest[mMax], float value, int rotFlags )
{
    float cosX = cos(value);
    float sinX = sin(value);

    switch( rotFlags )
    {
        case ROT_Z:
        {
            float tmp4, tmp5, tmp8, tmp9;
            tmp4 = dest[4] * cosX;
            tmp5 = dest[5] * cosX;
            tmp8 = dest[4] * -sinX;
            tmp9 = dest[5] * -sinX;
            dest[4] = tmp4;
            dest[5] = tmp5;
            dest[6] = sinX;
            dest[8] = tmp8;
            dest[9] = tmp9;
            dest[10] = cosX;
            break;
        }

        case ROT_Y:
        {
            float tmp4, tmp6, tmp8, tmp10;
            tmp4 = dest[8] * sinX;
            tmp6 = dest[10] * sinX;
            tmp8 = dest[8] * cosX;
            tmp10 = dest[10] * cosX;
            dest[4] = tmp4;
            dest[5] = cosX;
            dest[6] = tmp6;
            dest[8] = tmp8;
            dest[9] = -sinX;
            dest[10] = tmp10;
            break;
        }

        case ROT_Z | ROT_Y:
        {
            float tmp4, tmp5, tmp6, tmp8, tmp9, tmp10;
            tmp4 = ( dest[4] * cosX ) + ( dest[8] * sinX );
            tmp5 = ( dest[5] * cosX ) + ( dest[9] * sinX );
            tmp6 = dest[10] * sinX;
            tmp8 = ( dest[4] * -sinX ) + ( dest[8] * cosX );
            tmp9 = ( dest[5] * -sinX ) + ( dest[9] * cosX );
            tmp10 = dest[10] * cosX;
            dest[4] = tmp4;
            dest[5] = tmp5;
            dest[6] = tmp6;
            dest[8] = tmp8;
            dest[9] = tmp9;
            dest[10] = tmp10;
            break;
        }

        case NO_ROT:
        {
            dest[5]  =  cosX;
            dest[6]  =  sinX;
            dest[9]  = -sinX;
            dest[10] =  cosX;
            break;
        }
    }
}

/************************************************************************
*    DESC:  Use a point to set a row
************************************************************************/
void CMatrix::setRow( int row, const CPoint<float> & point )
{
    float *pRow = matrix + (row * 4);

    *pRow = point.x; ++pRow;
    *pRow = point.y; ++pRow;
    *pRow = point.z;
}

/************************************************************************
*    DESC:  Use a point to set a column
************************************************************************/
void CMatrix::setColumn( const int col, const float x, const float y, const float z )
{
    float *pCol = matrix + col;

    *pCol = x; pCol += 4;
    *pCol = y; pCol += 4;
    *pCol = z;
}

/************************************************************************
*    DESC:  The multiplication operator
************************************************************************/
CMatrix CMatrix::operator * ( const CMatrix & obj ) const
{
    float tmp[mMax];

    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
            tmp[(i*4)+j] =   (matrix[i*4]     * obj[j])
                           + (matrix[(i*4)+1] * obj[4+j])
                           + (matrix[(i*4)+2] * obj[8+j])
                           + (matrix[(i*4)+3] * obj[12+j]);
        }
    }

    return CMatrix(tmp);
}

/************************************************************************
*    DESC:  The multiplication operator
************************************************************************/
CMatrix CMatrix::operator *= ( const CMatrix & obj )
{
    float tmp[mMax];

    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
            tmp[(i*4)+j] =    (matrix[i*4]     * obj[j])
                            + (matrix[(i*4)+1] * obj[4+j])
                            + (matrix[(i*4)+2] * obj[8+j])
                            + (matrix[(i*4)+3] * obj[12+j]);
        }
    }

    // Copy the tmp matrix to the class matrix
    std::memcpy( matrix, tmp, sizeof(tmp) );

    return *this;
}

/************************************************************************
*    DESC:  The = operator
************************************************************************/
CMatrix CMatrix::operator = ( const float mat[4][4] )
{
    for( int i = 0; i < 4; ++i )
        for( int j = 0; j < 4; ++j )
            matrix[i*4 + j] = mat[i][j];

    return *this;
}

/************************************************************************
*    DESC:  The [] operator
************************************************************************/
float CMatrix::operator [] ( uint index ) const
{
    if( index >= mMax )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index exceeds allowable range (%d,%d).\n\n%s\nLine: %s") % index % mMax % __FUNCTION__ % __LINE__ ));

    return matrix[index];
}

float & CMatrix::operator [] ( uint index )
{
    if( index >= mMax )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index exceeds allowable range (%d,%d).\n\n%s\nLine: %s") % index % mMax % __FUNCTION__ % __LINE__ ));

    return matrix[index];
}

/************************************************************************
*    DESC:  The () operator
************************************************************************/
const float * CMatrix::operator () () const
{
    return matrix;
}
