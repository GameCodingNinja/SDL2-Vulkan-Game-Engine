/************************************************************************
*    FILE NAME:       meshbinaryfileheader.h
*
*    DESCRIPTION:     Header for mesh files outputed in binary
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/point.h>
#include <common/texture.h>

// SDL lib dependencies
#include <cstdint>

// File header tag check
const uint32_t MESH_FILE_HEADER = 0x415382AE;

// Hex for tag check
const uint32_t TAG_CHECK = 0x6A82Fc4d;

// Max character sizes for the texture path and joint name
const int TEXT_PATH_SIZE = 128;
const int JOINT_NAME_SIZE = 20;

// Header information for meshes
class CMeshBinaryFileHeader
{
public:

    CMeshBinaryFileHeader() : file_header(MESH_FILE_HEADER)
    {};
    
    uint32_t file_header;
    uint16_t vert_count;
    uint16_t uv_count;
    uint16_t vert_norm_count;
    uint16_t face_group_count;
    uint16_t text_count;
    uint16_t joint_count;

};


// class for reading in texture info
class CBinaryTexture
{
public:

    int8_t type;
    char path[TEXT_PATH_SIZE];
};


// Class for reading and writing the joint information to an animated mesh.
class CBinaryJoint
{
public:

    uint16_t vert_count;
    char name[JOINT_NAME_SIZE];
    char parentName[JOINT_NAME_SIZE];
    CPoint<float> headPos;
    CPoint<float> tailPos;
    float orientation[4][4];
};


// Hex for RSA (Rabid Squirrel Animation)
const int ANIMATION_FILE_HEADER = 0x415352;

// Hex for JNT (JoiNT)
const int JOINT = 0x544E4A;

// Hex for KEY (KEYframe)
const int KEY_FRAME = 0x59454B;

// Class for writing the header on an animation file
class CMeshAnimationBinaryFileHeader
{
public:

    CMeshAnimationBinaryFileHeader() : file_header(ANIMATION_FILE_HEADER)
    {};
    
    uint32_t file_header;
    uint16_t joint_count;
    uint16_t frame_count;

};

// Class for reading and writing the joint information to an animation file
class CBinaryJointAnimation
{
public:

    uint16_t keyframe_count;
    char name[JOINT_NAME_SIZE];
    char parentName[JOINT_NAME_SIZE];

};

// Class for reading and writing the keyframe information to an animation file
class CBinaryKeyFrame
{
public:

    float time, x, y, z, rx, ry, rz;

};

// Class for reading and writing the total face count within a group and the material
// it belongs to
class CBinaryFaceGroup
{
public:

    uint16_t groupFaceCount, vertexBufCount, indexBufCount, textureCount;

};

// Class for reading and writing face information to a collision mesh or shadow mesh
class CBinaryVertex
{
public:

    uint16_t vert;
    uint16_t norm;
    uint16_t uv;
};

class CBinaryVertexNoTxt
{
public:

    uint16_t vert;
    uint16_t norm;
};

// Class for reading and writing face information to a collision mesh or shadow mesh
class CBinaryVertFace
{
public:

    uint16_t vert[3];

};

// Class for reading and writing face information to a visual mesh or animated mesh
class CBinaryFace : public CBinaryVertFace
{
public:

    uint16_t normal[3];
    uint16_t uv[3];

};
