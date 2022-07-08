/************************************************************************
*    FILE NAME:       matrix.h
*
*    DESCRIPTION:     4x4 Matrix math class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/normal.h>
#include <common/quaternion.h>
#include <common/rect.h>
#include <common/size.h>
#include <common/quad.h>
#include <common/point.h>

// Standard lib dependencies
#include <cstdint>

// The 3D view matrix class:
class CMatrix
{
public:

    enum
    {
        NO_ROT = 0,
        ROT_Z = 1,
        ROT_Y = 2,
        ROT_X = 4,
        ROT_ALL = ROT_Z | ROT_Y | ROT_X
    };

    enum
    {
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33,
        mMax
    };

    // Constructor
    CMatrix();

    // Copy constructor
    CMatrix( const CMatrix & obj );
    CMatrix( float mat[mMax] );
    
    // Copy from another matrix
    void copy( const CMatrix & obj );

    // Reset the matrix to the identity matrix
    void initilizeMatrix();

    // Clear translation data from the matrix
    void clearTranlate();

    // Generate 3D rotation matrix
    void rotate( const CPoint<float> & point );

    // Get the Z rotation of the matrix
    float getZRot( bool inDegrees = true ) const;

    // Create 3D translation matrix
    void translate( const CPoint<float> & point );
    void translate( const CSize<int16_t> & size );

    // Function designed to merge scaling matrix with master matrix
    void setScale( const CPoint<float> & point );
    void setScale( const CSize<float> & size );
    void setScale( float scale );
    void setScale( float scaleX, float scaleY );
    void setScale( float scaleX, float scaleY, float scaleZ );
    void scale( const CPoint<float> & point );
    void scale( const CSize<float> & size );
    void scale( float scale );
    void scale( float scaleX, float scaleY );
    void scale( float scaleX, float scaleY, float scaleZ );

    // Get the scale
    CPoint<float> getScale() const;

    // Functions designed to transform using the master matrix
    void transform( CPoint<float> & dest, const CPoint<float> & source ) const;
    void transform( CPoint<float> * pDest, const CPoint<float> * pSource ) const;
    void transform( CNormal<float> & dest, const CNormal<float> & source ) const;
    void transform( CRect<float> & dest, const CRect<float> & source ) const;
    void transform( CQuad & dest, const CQuad & source ) const;
    void transform3x3( CPoint<float> & dest, const CPoint<float> & source ) const;
    
    // Merge matrix into master matrix
    void mergeMatrix( const CMatrix & obj );

    // Get the transpose of a matrix
    CMatrix getTransposeMatrix() const;

    // Get the matrix point in space
    CPoint<float> getMatrixPoint();

    // Get matrix rotation
    CPoint<float> getMatrixRotation();

    // Inverses this matrix.
    void invertX();
    void invertY();
    void invertZ();
    bool inverse();
    bool invert();

    // Set quaternion
    void set( const CQuaternion & quat );
    
    // Use a point to set a row
    void setRow( int row, const CPoint<float> & point );
    
    // Use a point to set a column
    void setColumn( const int col, const float x, const float y, const float z );

    // Create the matrix based on where the camera is looking
    void lookAt( const CPoint<float> & pos, const CPoint<float> & target, const CPoint<float> & cameraUp );

    // Multiply the matrices only using the rotation/scale portion
    void multiply3x3( const CMatrix & obj );

    // Calulate an orthographic matrix
    void orthographicRH( float w, float h, float zn, float zf );
    void orthographicLH( float w, float h, float zn, float zf );

    // Calulate an perspective matrix
    void perspectiveFovRH( float fovy, float aspect, float zn, float zf );
    void perspectiveFovLH( float fovy, float aspect, float zn, float zf );

    // Overloaded operators
    CMatrix operator * ( const CMatrix & obj ) const;
    CMatrix operator *= ( const CMatrix & obj );
    CMatrix operator = ( const float mat[4][4] );
    float operator [] ( uint index ) const;
    float & operator [] ( uint index );
    const float * operator () () const;

private:

    // Initializes a specific matrix to the identity matrix
    void initIdentityMatrix( float mat[mMax] );

    // Merge source matrix into destination matrix
    void mergeMatrices( float dest[mMax], const float source[mMax] );

    // Merge matrix into master matrix
    void mergeMatrix( const float newMatrix[mMax] );
    void mergeMatrix3x3( const float mat[mMax] );

    // Separate axis rotation functions
    void rotateZ( float dest[mMax], float value, int rotFlags );
    //void RotateZDeg( float dest[mMax], float value, int rotFlags );
    void rotateY( float dest[mMax], float value, int rotFlags );
    //void RotateYDeg( float dest[mMax], float value, int rotFlags );
    void rotateX( float dest[mMax], float value, int rotFlags );
    //void RotateXDeg( float dest[mMax], float value, int rotFlags );
    
    // scale and merge
    void mergeScale( float x, float y, float z );

private:

    // Master matrix
    // a single dimension array (array[]) is much faster
    // then a multi-dimension array (array[][])
    float matrix[mMax];

};
