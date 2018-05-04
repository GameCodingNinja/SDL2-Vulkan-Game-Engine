#************************************************************************
#    FILE NAME:       io_export_waffles_header.py
#
#    DESCRIPTION:     Header to sprite files outputed in binary
#***********************************************************************/

from ctypes import *

# Hex for 3D mesh file
SPRITE_FILE_HEADER = 0x415382AE

# Hex for tag check
TAG_CHECK = 0x6A82Fc4d

# Max character sizes for the material path and joint name
TEXTURE_PATH_SIZE = 128
JOINT_NAME_SIZE = 20

# Hex for RSA (Rabid Squirrel Animation)
ANIMATION_FILE_HEADER = 0x415352

# Texture types
TEXTURE_NULL           = -1,
TEXTURE_DIFFUSE        = 0
TEXTURE_NORMAL         = 1
TEXTURE_SPECULAR       = 2
TEXTURE_DISPLACEMENT   = 3
TEXTURE_MAX_TEXT_TYPES = 4

# Tag check
class CTagCheck(Structure):
     _fields_ = [ ('tagCheck',c_uint32) ]

# Header information for meshs
class CSpriteBinaryFileHeader(Structure):
     _fields_ = [ ('file_header',c_uint32),
                  ('vert_count',c_uint16),
                  ('uv_count',c_uint16),
                  ('vert_norm_count',c_uint16),
                  ('face_group_count',c_uint16),
                  ('mat_count',c_uint16),
                  ('joint_count',c_uint16) ]

# Class for writing the vert list
class CBinaryVert(Structure):
     _fields_ = [ ('x',c_float),
                  ('y',c_float),
                  ('z',c_float) ]

class CBinaryVertBone(Structure):
     _fields_ = [ ('x',c_float),
                  ('y',c_float),
                  ('z',c_float),
                  ('w',c_float),
                  ('b',c_int) ]

# Class for writing the uv list
class CBinaryUV(Structure):
     _fields_ = [ ('u',c_float),
                  ('v',c_float) ]

# Class for writing face information to a collision mesh or shadow mesh
class CBinaryVertFace(Structure):
     _fields_ = [ ('vert_1',c_uint16),
                  ('vert_2',c_uint16),
                  ('vert_3',c_uint16) ]

# Class for writing face information to a visual mesh or animated mesh
class CBinaryFace(Structure):
     _fields_ = [ ('vert_1',c_uint16),
                  ('vert_2',c_uint16),
                  ('vert_3',c_uint16),
                  ('norm_1',c_uint16),
                  ('norm_2',c_uint16),
                  ('norm_3',c_uint16),
                  ('uv_1',c_uint16),
                  ('uv_2',c_uint16),
                  ('uv_3',c_uint16), ]

# Version of the above class that doesn't require UV's for textures
class CBinaryFaceNoText(Structure):
     _fields_ = [ ('vert_1',c_uint16),
                  ('vert_2',c_uint16),
                  ('vert_3',c_uint16),
                  ('norm_1',c_uint16),
                  ('norm_2',c_uint16),
                  ('norm_3',c_uint16) ]

# Class for writing vertex information to a visual mesh or animated mesh
class CBinaryVertex(Structure):
     _fields_ = [ ('vert',c_uint16),
                  ('norm',c_uint16),
                  ('uv',c_uint16) ]

class CBinaryVertexNoText(Structure):
     _fields_ = [ ('vert',c_uint16),
                  ('norm',c_uint16) ]

class CBinaryIBO(Structure):
     _fields_ = [ ('ibo',c_uint16) ]


# Class for writing the total face count within a group and the material it belongs to
class CBinaryFaceGroup(Structure):
     _fields_ = [ ('groupFaceCount',c_uint16),
                  ('vertexBufCount',c_uint16),
                  ('indexBufCount',c_uint16),
                  ('textureCount',c_uint16) ]

# Class for writing texture indexes
class CBinaryTextIndex(Structure):
     _fields_ = [ ('textureIndex',c_uint16) ]

# Face count
class CFaceCount(Structure):
     _fields_ = [ ('faceCount',c_uint16) ]

# Texture type and path
class CTexturePath(Structure):
     _fields_ = [ ('textureType',c_uint8),
                  ('path',c_char * TEXTURE_PATH_SIZE) ]

# Joint binary array
class CBinaryJoint(Structure):
     _fields_ = [ ('vert_count',c_int),
                  ('name',c_char * JOINT_NAME_SIZE),
                  ('parent',c_char * JOINT_NAME_SIZE),
                  ('headX',c_float), ('headY',c_float), ('headZ',c_float),
                  ('tailX',c_float), ('tailY',c_float), ('tailZ',c_float),
                  ('m00',c_float), ('m01',c_float), ('m02',c_float), ('m03',c_float),
                  ('m10',c_float), ('m11',c_float), ('m12',c_float), ('m13',c_float),
                  ('m20',c_float), ('m21',c_float), ('m22',c_float), ('m23',c_float),
                  ('m30',c_float), ('m31',c_float), ('m32',c_float), ('m33',c_float) ]

# Class for writing the header on an animation file
class CSpriteAnimationBinaryFileHeader(Structure):
     _fields_ = [ ('file_header',c_uint32),
                  ('bone_count',c_uint16),
                  ('frame_count',c_uint16)]

# Class for reading and writing the joint information to an animation file
class CBinaryJointAnimation(Structure):
     _fields_ = [ ('keyframe_count',c_uint16),
                  ('name',c_char * JOINT_NAME_SIZE),
                  ('parent',c_char * JOINT_NAME_SIZE)]

# Class for reading and writing the keyframe information to an animation file
class CBinaryKeyFrame(Structure):
     _fields_ = [ ('time',c_float),
                  ('x',c_float),
                  ('y',c_float),
                  ('z',c_float),
                  ('rx',c_float),
                  ('ry',c_float),
                  ('rz',c_float)]


