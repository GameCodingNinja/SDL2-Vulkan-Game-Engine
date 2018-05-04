
bl_info = {
    "name": "Export Waffles 3D Mesh Format(.3dm)",
    "author": "Rosenorn",
    "version": (1, 1),
    "blender": (2, 71, 0),
    "api": 37702,
    "location": "File > Export > Waffles Engine (.3dm)",
    "description": "Export mesh Pointcache data (.3dm)",
    "warning": "",
    "wiki_url": "http://wiki.blender.org/index.php/Extensions:2.5/Py/"\
        "Scripts/Import-Export/PC2_Pointcache_export",
    "tracker_url": "https://projects.blender.org/tracker/index.php?"\
        "func=detail&aid=24703",
    "category": "Import-Export"}

import os
import bpy
import logging
from bpy.props import *
import mathutils, math, struct
from os import remove
import time
import io_export_waffles_header as header
from ctypes import *

# This is how to raise an exception
# raise RuntimeError('This export only supports one armature set')

# To translate correctly from Blender to the game world, rotate -90 on the x
# axis prior to exporting the data

#************************************************************************
#    desc:  Container for the exporter settings
#
#    NOTE:  export data class holding all data to help keep things simple
#************************************************************************
class ExportData:
    def __init__(self, context):
        self.context = context

        # Create the unique lists
        self.uniqueNormLst = []
        self.uniqueUVLst = []
        self.uniqueTextureLst = []

        # List of all the normals
        self.normLst = []

        # Create the face list used for non-texture meshes
        self.faceLst = []

        # Create the face group list to hold all our data
        self.faceGrpLst = []

        # Create a texture list that can be addressed by face material index
        self.meshMaterialLst = []

        # Create a mesh list to help keep things simple
        self.meshLst = []

        # Create a uv list
        self.uvMeshLst = []

        # Create a bone list
        self.dataBoneLst = []
        self.poseBoneLst = []

        # Create a bone dictionary
        self.boneDict = {}

        # Create a bone matrix list
        self.boneMatrixLst = []
        self.boneWorldMatrixLst = []

        # Create a vert bone list
        self.boneVertLst = []

        # Create a vert weight list
        self.weightVertLst = []

        # Total vert count
        self.totalVertCount = 0

        # Recorded armature object
        self.armatureObj = None
        self.armatureMatrix = mathutils.Matrix()

        self.armPosVec = mathutils.Vector((0, 0, 0))

        self.txtFile = None
        self.binaryFile = None

        self.exportArmature = True

        self.exportCollision = False

        self.keyFrameGrpLst = []


#************************************************************************
#    desc:  keyframe class
#************************************************************************
class CKeyFrame:
    # define as self so that these arrays are unique instances
    def __init__(self):
        # Actual data members
        self.time = 0.0

        self.posX = 0.0
        self.posY = 0.0
        self.posZ = 0.0

        self.rotEularX = 0.0
        self.rotEularY = 0.0
        self.rotEularZ = 0.0

        self.rotQuaternionX = 0.0
        self.rotQuaternionY = 0.0
        self.rotQuaternionZ = 0.0
        self.rotQuaternionW = 0.0

    #************************************************************************
    #    desc: Convert the Quaternion To Eular
    #************************************************************************
    def ConvertQuaternionToEular(self):

        sqw = self.rotQuaternionW * self.rotQuaternionW
        sqx = self.rotQuaternionX * self.rotQuaternionX
        sqy = self.rotQuaternionY * self.rotQuaternionY
        sqz = self.rotQuaternionZ * self.rotQuaternionZ

        # if normalised is one, otherwise is correction factor
        unit = sqx + sqy + sqz + sqw
        test = (self.rotQuaternionX * self.rotQuaternionY) + (self.rotQuaternionZ * self.rotQuaternionW)

        # singularity at north pole
        if test > (0.499 * unit):
            self.rotEularZ = 2.0 * math.atan2(self.rotQuaternionX, self.rotQuaternionW)
            self.rotEularX = math.pi/2
            self.rotEularY = 0
            return

        # singularity at south pole
        if test < (-0.499 * unit):
            self.rotEularZ = -2.0 * math.atan2(self.rotQuaternionX, self.rotQuaternionW)
            self.rotEularX = -math.pi/2
            self.rotEularY = 0
            return

        self.rotEularZ = math.atan2(2*self.rotQuaternionY*self.rotQuaternionW-2*self.rotQuaternionX*self.rotQuaternionZ, sqx - sqy - sqz + sqw)
        self.rotEularX = math.asin(2*test/unit)
        self.rotEularY = math.atan2(2*self.rotQuaternionX*self.rotQuaternionW-2*self.rotQuaternionY*self.rotQuaternionZ, -sqx + sqy - sqz + sqw)


#************************************************************************
#    desc:  keyframe group class
#************************************************************************
class CKeyFrameGrp:
    # define as self so that these arrays are unique instances
    def __init__(self):
        # Actual data members
        self.name = ""
        self.parent = ""
        self.keyFrameLst = []


#************************************************************************
#    desc:  face class
#************************************************************************
class CFace:
    # define as self so that these arrays are unique instances
    def __init__(self):
        # Actual data
        self.normLst = []
        self.uvLst = []

        # Indexes into the global lists
        self.vertIndexLst = []
        self.normIndexLst = []
        self.uvIndexLst = []
        self.boneIndexLst = []


#************************************************************************
#    desc:  vertex class
#************************************************************************
class CVertex:
    # define as self so that these arrays are unique instances
    def __init__(self):

        # Indexes into the global lists
        self.vertIndex = None
        self.normIndex = None
        self.uvIndex = None

    def __eq__(self, other):
        if self.vertIndex == other.vertIndex:
            if self.normIndex == other.normIndex:
                if self.uvIndex == other.uvIndex:
                    return True

        return False;


#************************************************************************
#    desc:  face group class
#************************************************************************
class CFaceGrp:
    # define as self so that these arrays are unique instances
    def __init__(self):
        self.faceLst = []
        self.textureIndexLst = []
        self.vertexBufLst = []
        self.indexBufLst = []


#************************************************************************
#    desc:  export a mesh with textures and armature
#************************************************************************
def export_mesh(exportData):

    #==========================================
    #           Orginize the data
    #==========================================

    # Hold the meshes in our own list. This keeps things
    # simple since there are a few ways to get the mesh data
    # and they don't all produce the same results
    CreateMeshLst(exportData)

    # Create the bone list if there are any
    if exportData.exportArmature:
        CreateBoneLst(exportData)

    # Create the bone matrix list
    if exportData.dataBoneLst:
        CreateBoneMatrixLst(exportData)

    # Create the bone vert index list
    if exportData.dataBoneLst:
        CreateBoneVertLst(exportData)

    # Create the UV list
    CreateUVLst(exportData)

    # Create a material list in the same order as the meshes
    # Materials require special handling to extract the textures
    # from them so it's easier to pull them out into our own list
    CreateTextureLst(exportData)

    # Create the unique texture list
    # NOTE: This needs to be done before CreateFaceGrpLst is called
    #       because all the polygons are resorted by their first texture
    CreateUniqueTextureLst(exportData)

    # Create the face groups sorted by texture of face material
    CreateFaceLst(exportData)

    # Create the unique normal list if there's an armature
    if exportData.dataBoneLst:
        CreateReorientedNormalLst(exportData)

    # Create the unique normal list and find each index into this list
    else:
        CreateUniqueNormalLst(exportData)

    # Create the unique UV list and find each index into this list
    CreateUniqueUVLst(exportData)

    # Create the vertex buffer list from the face list
    CreateVertexBufLst(exportData)

    #==========================================
    #       Write out the text file
    #==========================================

    # write out the text file header
    TextWriteHeader(exportData)

    # write out the vert list
    TextWriteVertLst(exportData)

    # write out the Normal list
    TextWriteNormalLst(exportData)

    # write out the UV list
    TextWriteUVLst(exportData)

    # Write out all face indexes
    TextWriteFaceIndexes(exportData)

    # Write out all the textures
    TextWriteTextures(exportData)

    # Write out all the bones
    TextWriteBones(exportData)


    #==========================================
    #       Write out the binary file
    #==========================================

    # write the file header
    BinaryWriteHeader(exportData)

    # Write the material paths
    BinaryWriteTextures(exportData)

    # write out all the verts
    BinaryWriteVertLst(exportData)

    # Write out all the vert normals
    BinaryWriteNormalLst(exportData)

    # write out all the UV
    BinaryWriteUVLst(exportData)

    # Write out all face indexes
    BinaryWriteFaceIndexes(exportData)

    # Write the bones
    BinaryWriteBones(exportData)


#************************************************************************
#    desc:  export the animation frames
#************************************************************************
def export_animationFrames(exportData):

    # Create the key frame data
    CreateKeyFrameData(exportData)

    #==========================================
    #       Write out the text file
    #==========================================

    # write out the key frame data
    TextWriteKeyFrameData(exportData)


    #==========================================
    #       Write out the binary file
    #==========================================

    # write out the key frame data
    BinaryWriteKeyFrameData(exportData)


#************************************************************************
#    desc:  Create the face groups sorted by texture of face material
#************************************************************************
def CreateFaceLst(exportData):

    # See if we have any textures
    if exportData.uniqueTextureLst:
        for i in range(len(exportData.meshLst)):
            CreateFaceGrpLst(exportData.meshLst[i],
                             exportData.uvMeshLst[i],
                             exportData.totalVertCount,
                             exportData.faceGrpLst,
                             exportData.meshMaterialLst[i],
                             exportData.uniqueTextureLst,
                             exportData)
            exportData.totalVertCount += len(exportData.meshLst[i].vertices)
    else:
        # Since there are no textures, add the only face group ever needed
        faceGrp = CFaceGrp()
        exportData.faceGrpLst.append(faceGrp)
        for mesh in exportData.meshLst:
            for face in mesh.tessfaces:
                # exportData.uvMeshLst is just a filler. It's not being used here
                AddFaceToGrp(mesh, exportData.uvMeshLst, exportData.totalVertCount, face, faceGrp, exportData)

            exportData.totalVertCount += len(mesh.vertices)


#************************************************************************
#    desc:  Create the vertex buffer list from the face list
#************************************************************************
def CreateVertexBufLst(exportData):

    for faceGrp in exportData.faceGrpLst:
        for face in faceGrp.faceLst:
            for i in range(len(face.vertIndexLst)):
                vertex = CVertex()
                vertex.vertIndex = face.vertIndexLst[i]

                if face.normIndexLst:
                    vertex.normIndex = face.normIndexLst[i]

                if face.uvIndexLst:
                    vertex.uvIndex = face.uvIndexLst[i]

                if faceGrp.vertexBufLst:
                    found = 0
                    counter = 0
                    for vert in faceGrp.vertexBufLst:
                        if vert == vertex:
                            found = 1
                            faceGrp.indexBufLst.append( counter )
                            break
                        counter += 1

                    if found == 0:
                        faceGrp.vertexBufLst.append( vertex )
                        faceGrp.indexBufLst.append( counter )

                else:
                    faceGrp.vertexBufLst.append( vertex )
                    faceGrp.indexBufLst.append( 0 )


#************************************************************************
#    desc:  Create mesh list
#           NOTE: There's a few ways to get mesh data but this seems
#                 to be the only way to get the correct UVs
#           NOTE: Here we rotate and convert the data for writing to
#                 the hard drive
#************************************************************************
def CreateMeshLst(exportData):
    for object in exportData.context.scene.objects:
        if object.type == "MESH":
            mesh = object.to_mesh(bpy.context.scene, False, "PREVIEW") # "PREVIEW" "RENDER"
            mesh.transform(object.matrix_world)
            exportData.meshLst.append( mesh )


#************************************************************************
#    desc:  Create bone list
#************************************************************************
def CreateBoneLst(exportData):
    boneCount = 0
    for object in exportData.context.scene.objects:
        if object.type == "ARMATURE":
            exportData.armPosVec.x = object.matrix_world[3][0]
            exportData.armPosVec.y = object.matrix_world[3][1]
            exportData.armPosVec.z = object.matrix_world[3][2]

            exportData.armatureObj = object
            for dataBone in object.data.bones:
                exportData.dataBoneLst.append( dataBone )
                exportData.boneDict[dataBone.name] = boneCount
                boneCount += 1

            for poseBone in object.pose.bones:
                exportData.poseBoneLst.append( poseBone )

            break


#************************************************************************
#    desc:  Create bone vert list
#
#    NOTE: The below dictionaries are used to keep duplicate data out.
#************************************************************************
def CreateBoneVertLst(exportData):
    # See if their are any armatures in use
    objectLst = []
    boneVertDict = {}
    weightVertDict = {}
    for object in exportData.context.scene.objects:
        for modifier in object.modifiers:
            if modifier.type == "ARMATURE":
                objectLst.append(object)
                break

    for object in objectLst:

        poseBones = exportData.armatureObj.pose.bones

        objectVertexGroups = {i: objGroup.name for (i, objGroup) in enumerate(object.vertex_groups)}

        vertCount = 0
        for mesh in exportData.meshLst:
            for vertex in mesh.vertices:
                found = False
                for group in vertex.groups:
                    for poseBone in (poseBones.get(objectVertexGroups.get(group.group, "")), ):
                        if poseBone is not None:
                            boneVertDict[vertCount] = exportData.boneDict[poseBone.name]
                            weightVertDict[vertCount] = group.weight
                            vertCount += 1
                            found = True
                            break
                    if found == True:
                        break

    # copy the info from the dictionary to the lists
    for k, v in boneVertDict.items():
        exportData.boneVertLst.append(v)

    for k, v in weightVertDict.items():
        exportData.weightVertLst.append(v)


#************************************************************************
#    desc:  Create a UV list in the same order as the meshes
#************************************************************************
def CreateUVLst(exportData):

    for mesh in exportData.meshLst:
        uvMesh = []
        uvCoordinates = None
        for UV in mesh.tessface_uv_textures:
            if UV.active:
                uvCoordinates = UV.data
                break

        if uvCoordinates is not None:
            for face in uvCoordinates:
                uvFace = []
                for uv in face.uv:
                    uvFace.append(uv)

                # Convert the v value to flip the texture
                for uv in uvFace:
                    uv[1] = 1 - uv[1]

                uvMesh.append(uvFace)

            exportData.uvMeshLst.append(uvMesh)


#************************************************************************
#    desc:  Create a texture list in the same order as the meshes
#************************************************************************
def CreateTextureLst(exportData):
    if exportData.exportCollision == False:
        for mesh in exportData.meshLst:
            materialLst = []
            for material in mesh.materials:
                textureLst = []
                for textureKey in material.texture_slots.keys():
                    textureSlot = material.texture_slots[textureKey]
                    if (textureSlot.texture.type == "IMAGE") and (textureSlot.texture.image.source == "FILE"):
                        textureLst.append(textureSlot)

                materialLst.append(textureLst)

            exportData.meshMaterialLst.append(materialLst)


#************************************************************************
#    desc:  Create a unique texture list
#************************************************************************
def CreateUniqueTextureLst(exportData):
    if exportData.exportCollision == False:
        for meshMaterials in exportData.meshMaterialLst:
            for meshMaterial in meshMaterials:
                for textureSlot in meshMaterial:
                    found = 0
                    for uniqueTexture in exportData.uniqueTextureLst:
                        if textureSlot.texture.image.filepath == uniqueTexture.texture.image.filepath:
                            found = 1
                            break

                    if found == 0:
                        exportData.uniqueTextureLst.append(textureSlot);


#************************************************************************
#    desc:  Create a unique vertex normal list
#************************************************************************
def CreateReorientedNormalLst(exportData):
    for faceGrp in exportData.faceGrpLst:
        for face in faceGrp.faceLst:
            nIndex = 0
            for norm in face.normLst:
                bIndex = face.boneIndexLst[nIndex]
                found = 0
                uniqueCount = 0

                #############################################################################
                # (Joints) The Y and Z are switched here for the binary vert list
                #############################################################################
                normal = mathutils.Vector((norm[0],norm[2],norm[1]))
                matrix = mathutils.Matrix(exportData.boneWorldMatrixLst[bIndex])
                newNormal = mathutils.Vector()

                newNormal[0] = (normal[0] * matrix[0][0]) + (normal[1] * matrix[1][0]) + (normal[2] * matrix[2][0])
                newNormal[1] = (normal[0] * matrix[0][1]) + (normal[1] * matrix[1][1]) + (normal[2] * matrix[2][1])
                newNormal[2] = (normal[0] * matrix[0][2]) + (normal[1] * matrix[1][2]) + (normal[2] * matrix[2][2])

                for uniqueNorm in exportData.uniqueNormLst:
                    if newNormal[0] == uniqueNorm[0]:
                        if newNormal[1] == uniqueNorm[1]:
                            if newNormal[2] == uniqueNorm[2]:
                                found = 1
                                face.normIndexLst.append(uniqueCount)
                                break

                    uniqueCount += 1

                if found == 0:
                    exportData.uniqueNormLst.append(newNormal)
                    face.normIndexLst.append(uniqueCount)

                nIndex += 1


#************************************************************************
#    desc:  Create a unique vertex normal list
#************************************************************************
def CreateUniqueNormalLst(exportData):
    if exportData.exportCollision == False:
        for faceGrp in exportData.faceGrpLst:
            for face in faceGrp.faceLst:
                for norm in face.normLst:
                    found = 0
                    uniqueCount = 0
                    for uniqueNorm in exportData.uniqueNormLst:
                        if norm[0] == uniqueNorm[0]:
                            if norm[1] == uniqueNorm[1]:
                                if norm[2] == uniqueNorm[2]:
                                    found = 1
                                    face.normIndexLst.append(uniqueCount)
                                    break

                        uniqueCount += 1

                    if found == 0:
                        exportData.uniqueNormLst.append(norm)
                        face.normIndexLst.append(uniqueCount)


#************************************************************************
#    desc:  Create a unique UV list
#************************************************************************
def CreateUniqueUVLst(exportData):
    if exportData.exportCollision == False:
        for faceGrp in exportData.faceGrpLst:
            for face in faceGrp.faceLst:
                for uv in face.uvLst:
                    found = 0
                    uniqueCount = 0
                    for uniqueUV in exportData.uniqueUVLst:
                        if uv[0] == uniqueUV[0]:
                            if uv[1] == uniqueUV[1]:
                                found = 1
                                face.uvIndexLst.append(uniqueCount)
                                break

                        uniqueCount += 1

                    if found == 0:
                        exportData.uniqueUVLst.append(uv)
                        face.uvIndexLst.append(uniqueCount)


#************************************************************************
#    desc:  Create face group list based on first texture in the material
#
#    param: meshes -  meshes
#           meshVertOffset - offset to add to the vert index
#           faceGrpLst - face group list
#           uniqueTextureLst - unique texture list
#************************************************************************
def CreateFaceGrpLst(mesh, uvLst, meshVertOffset, faceGrpLst, meshMaterials, uniqueTextureLst, exportData):

    for face in mesh.tessfaces:

        # Get the first texture in the slot
        textureSlots = meshMaterials[face.material_index]

        # Find the texture in the unique texture list
        # We only use the first texture from each material
        textIndex = GetTextureIndex(textureSlots[0], uniqueTextureLst)

        # See if there is a face group using this texture
        found = 0
        for faceGrp in faceGrpLst:
            if faceGrp.textureIndexLst[0] == textIndex:
                AddFaceToGrp(mesh, uvLst[face.index], meshVertOffset, face, faceGrp, exportData)
                found = 1
                break

        if found == 0:
            faceGrp = CFaceGrp()

            # find the index to all the textures in the unique
            # list and add the indexes to the group
            for textureSlot in textureSlots:
                faceGrp.textureIndexLst.append(GetTextureIndex(textureSlot, uniqueTextureLst))

            # Add the first face to this group
            AddFaceToGrp(mesh, uvLst[face.index], meshVertOffset, face, faceGrp, exportData)

            # add new face group to the list
            faceGrpLst.append(faceGrp)


#************************************************************************
#    desc:  Create face group list based on first texture in the material
#
#    param: mesh -  mesh
#           meshVertOffset - offset to add to the vert index
#           face the face to add
#           faceGrp - face group to add it to
#************************************************************************
def AddFaceToGrp(mesh, uvFace, meshVertOffset, face, faceGrp, exportData):

    tmpFace1 = CFace()

    # Add the vert indexes and add the offset into that list. These are the indexes into
    # list of verts that define the face. Since the list via Blender is unique, there's no need
    # to create a unique list from these
    tmpFace1.vertIndexLst.append( face.vertices[0] + meshVertOffset )
    tmpFace1.vertIndexLst.append( face.vertices[1] + meshVertOffset )
    tmpFace1.vertIndexLst.append( face.vertices[2] + meshVertOffset )

    # Add the bone indexes to the face
    if exportData.dataBoneLst:
        tmpFace1.boneIndexLst.append( exportData.boneVertLst[face.vertices[0] + meshVertOffset] )
        tmpFace1.boneIndexLst.append( exportData.boneVertLst[face.vertices[1] + meshVertOffset] )
        tmpFace1.boneIndexLst.append( exportData.boneVertLst[face.vertices[2] + meshVertOffset] )

    # Add the normals. These are the actual normals and will be used to create the unique list
    if exportData.exportCollision == False:
        if face.use_smooth:
            tmpFace1.normLst.append( mesh.vertices[face.vertices[0]].normal )
            tmpFace1.normLst.append( mesh.vertices[face.vertices[1]].normal )
            tmpFace1.normLst.append( mesh.vertices[face.vertices[2]].normal )
        else:
            tmpFace1.normLst.append( face.normal )
            tmpFace1.normLst.append( face.normal )
            tmpFace1.normLst.append( face.normal )

        # add the UV to the list. These are the actual UV's and will be used to create the unique list
        if exportData.uniqueTextureLst:
            tmpFace1.uvLst.append( uvFace[0] )
            tmpFace1.uvLst.append( uvFace[1] )
            tmpFace1.uvLst.append( uvFace[2] )

    # Add face to face group
    faceGrp.faceLst.append(tmpFace1)


    # if quad, add another face to convert to tri
    if (len(face.vertices) == 4):
        tmpFace2 = CFace()

        # Add the bone indexes to the face
        if exportData.dataBoneLst:
            tmpFace2.boneIndexLst.append( exportData.boneVertLst[face.vertices[2] + meshVertOffset] )
            tmpFace2.boneIndexLst.append( exportData.boneVertLst[face.vertices[3] + meshVertOffset] )
            tmpFace2.boneIndexLst.append( exportData.boneVertLst[face.vertices[0] + meshVertOffset] )

        # Add the vert indexes and add the offset into that list.
        tmpFace2.vertIndexLst.append( face.vertices[2] + meshVertOffset )
        tmpFace2.vertIndexLst.append( face.vertices[3] + meshVertOffset )
        tmpFace2.vertIndexLst.append( face.vertices[0] + meshVertOffset )

        # Add the normals. These are the actual normals and will be used to create the unique list
        if exportData.exportCollision == False:
            if face.use_smooth:
                tmpFace2.normLst.append( mesh.vertices[face.vertices[2]].normal )
                tmpFace2.normLst.append( mesh.vertices[face.vertices[3]].normal )
                tmpFace2.normLst.append( mesh.vertices[face.vertices[0]].normal )
            else:
                tmpFace2.normLst.append( face.normal )
                tmpFace2.normLst.append( face.normal )
                tmpFace2.normLst.append( face.normal )

            # add the UV to the list. These are the actual UV's and will be used to create the unique list
            if exportData.uniqueTextureLst:
                tmpFace2.uvLst.append( uvFace[2] )
                tmpFace2.uvLst.append( uvFace[3] )
                tmpFace2.uvLst.append( uvFace[0] )

        # Add face to face group
        faceGrp.faceLst.append(tmpFace2)


#************************************************************************
#    desc:  Get the texture index from the global list
#************************************************************************
def GetTextureIndex(textureSlot, uniqueTextureLst):
    result = 0
    for uniqueTexture in uniqueTextureLst:
        if textureSlot.texture.image.filepath == uniqueTexture.texture.image.filepath:
            break
        result += 1

    return result


#************************************************************************
#    desc:  Create the key frame data
#************************************************************************
def CreateKeyFrameData(exportData):

    for object in exportData.context.scene.objects:
        if object.type == "ARMATURE" and object.animation_data:
            exportData.armatureObj = object
            break;

    if exportData.armatureObj is not None:
        action = exportData.armatureObj.animation_data.action

        boneCounter = 0

        if action:
            for bone in exportData.armatureObj.pose.bones:

                inverseMat = bone.matrix.inverted()
                inverseQuat = inverseMat.to_quaternion()
                boneVec = bone.vector.normalized()

                posFCurves = [None, None, None]
                rotFCurves = [None, None, None, None]
                found = False

                for fCurve in action.fcurves:
                    if fCurve.data_path == "pose.bones[\"{}\"].location".format(bone.name):
                        posFCurves[fCurve.array_index] = fCurve
                        found = True
                    elif fCurve.data_path == "pose.bones[\"{}\"].rotation_quaternion".format(bone.name):
                        rotFCurves[fCurve.array_index] = fCurve
                        found = True

                # If this bone has no animation data, skip it
                if found == False:
                    continue

                # Make a key frame group
                keyFrameGrp = CKeyFrameGrp()

                keyFrameGrp.name = bone.name
                if bone.parent is not None:
                    keyFrameGrp.parent = bone.parent.name

                # Like much in Blender, getting the key frame data is an odd practice
                for i in range(len(rotFCurves[0].keyframe_points)):
                    keyFrame = CKeyFrame()

                    keyFrame.time = rotFCurves[0].keyframe_points[i].co[0]

                    pVec = mathutils.Vector((
                        posFCurves[0].keyframe_points[i].co[1],
                        posFCurves[1].keyframe_points[i].co[1],
                        posFCurves[2].keyframe_points[i].co[1]))

                    # convert to left handed system and rotate to the correct axis
                    bMatrix = exportData.armatureObj.matrix_world.inverted()
                    #bMatrix *= exportData.conversionMatrix
                    pVec = pVec * bMatrix

                    keyFrame.posX = pVec.x
                    keyFrame.posY = pVec.y
                    keyFrame.posZ = pVec.z

                    # Quaternion
                    quat = mathutils.Quaternion()
                    quat.x = rotFCurves[1].keyframe_points[i].co[1]
                    quat.y = rotFCurves[2].keyframe_points[i].co[1]
                    quat.z = rotFCurves[3].keyframe_points[i].co[1]
                    quat.w = rotFCurves[0].keyframe_points[i].co[1]

                    # Convert to euler
                    euler = quat.to_euler("XYZ")

                    # Make into a vector for rotation conversion
                    rVec = mathutils.Vector((euler.x, euler.y, euler.z))

                    # Get the data bone
                    dataBone = exportData.armatureObj.data.bones[boneCounter]

                    # Copy over x, y, z
                    keyFrame.rotEularX = -rVec.x
                    keyFrame.rotEularY = -rVec.z
                    keyFrame.rotEularZ = -rVec.y

                    keyFrameGrp.keyFrameLst.append(keyFrame)

                boneCounter = boneCounter + 1
                exportData.keyFrameGrpLst.append(keyFrameGrp)


#************************************************************************
#    desc:  Create the key frame data
#************************************************************************
def CreateBoneMatrixLst(exportData):
    # Set up armature matrix
    armMatrix = exportData.armatureObj.matrix_world
    newArmMatrix = mathutils.Matrix()

    # Swap the second and third column and row of the armature's matrix. This matches up with our coordinate system.
    newArmMatrix[0][0],newArmMatrix[0][1],newArmMatrix[0][2],newArmMatrix[0][3] = armMatrix[0][0],armMatrix[0][2],armMatrix[0][1],armMatrix[0][3]
    newArmMatrix[1][0],newArmMatrix[1][1],newArmMatrix[1][2],newArmMatrix[1][3] = armMatrix[2][0],armMatrix[2][2],armMatrix[2][1],armMatrix[2][3]
    newArmMatrix[2][0],newArmMatrix[2][1],newArmMatrix[2][2],newArmMatrix[2][3] = armMatrix[1][0],armMatrix[1][2],armMatrix[1][1],armMatrix[1][3]
    newArmMatrix[3][0],newArmMatrix[3][1],newArmMatrix[3][2],newArmMatrix[3][3] = armMatrix[3][0],armMatrix[3][2],armMatrix[3][1],armMatrix[3][3]

    # Set the armature matrix to a data value in the exportData object, in case we need it later
    exportData.armatureMatrix = newArmMatrix

    for bone in exportData.dataBoneLst:

        # Set up matrices
        absBoneMatrix = mathutils.Matrix()
        newAbsBoneMatrix = mathutils.Matrix()
        boneMatrix = bone.matrix_local

        # Swap the second and third column and rows of the bone's matrix. This matches up with our matrices.
        absBoneMatrix[0][0],absBoneMatrix[0][1],absBoneMatrix[0][2],absBoneMatrix[0][3] = boneMatrix[0][0],boneMatrix[0][2],boneMatrix[0][1],boneMatrix[0][3]
        absBoneMatrix[1][0],absBoneMatrix[1][1],absBoneMatrix[1][2],absBoneMatrix[1][3] = boneMatrix[2][0],boneMatrix[2][2],boneMatrix[2][1],boneMatrix[2][3]
        absBoneMatrix[2][0],absBoneMatrix[2][1],absBoneMatrix[2][2],absBoneMatrix[2][3] = boneMatrix[1][0],boneMatrix[1][2],boneMatrix[1][1],boneMatrix[1][3]
        absBoneMatrix[3][0],absBoneMatrix[3][1],absBoneMatrix[3][2],absBoneMatrix[3][3] = boneMatrix[3][0],boneMatrix[3][2],boneMatrix[3][1],boneMatrix[3][3]

        # Multiply the armature and bone matrices to get a bone matrix in terms of the model environment
        newAbsBoneMatrix = newArmMatrix * absBoneMatrix

        # Apply the inverse of the bone matrix to the world matrix list. This is the matrix applied to
        # the vertices of the object so that all vertices of a bone are aligned with the bone along the x axis.
        exportData.boneWorldMatrixLst.append(newAbsBoneMatrix.inverted())

        if bone.parent is None:

            rootBoneMatrix = mathutils.Matrix()

            # Calculate the new root bone
            rootBoneMatrix = newArmMatrix * absBoneMatrix

            # Apply the new root bone matrix to the matrix list to export
            exportData.boneMatrixLst.append(rootBoneMatrix)

        else:

            # Set up matrices
            orientMatrix = mathutils.Matrix()
            tmpMatrix = mathutils.Matrix()
            newParentMatrix = mathutils.Matrix()

            parentMatrix = bone.parent.matrix_local

            # Swap the second and third column and rows of the parent's matrix. This matches up with our coordinate system.
            newParentMatrix[0][0],newParentMatrix[0][1],newParentMatrix[0][2],newParentMatrix[0][3] = parentMatrix[0][0],parentMatrix[0][2],parentMatrix[0][1],parentMatrix[0][3]
            newParentMatrix[1][0],newParentMatrix[1][1],newParentMatrix[1][2],newParentMatrix[1][3] = parentMatrix[2][0],parentMatrix[2][2],parentMatrix[2][1],parentMatrix[2][3]
            newParentMatrix[2][0],newParentMatrix[2][1],newParentMatrix[2][2],newParentMatrix[2][3] = parentMatrix[1][0],parentMatrix[1][2],parentMatrix[1][1],parentMatrix[1][3]
            newParentMatrix[3][0],newParentMatrix[3][1],newParentMatrix[3][2],newParentMatrix[3][3] = parentMatrix[3][0],parentMatrix[3][2],parentMatrix[3][1],parentMatrix[3][3]

            # Find the new orientation matrix for the bone
            tmpMatrix = parentMatrix.inverted() * boneMatrix

            # Swap the second and third column and rows of the bone's matrix. This matches up with our coordinate system.
            orientMatrix[0][0],orientMatrix[0][1],orientMatrix[0][2],orientMatrix[0][3] = tmpMatrix[0][0],tmpMatrix[0][2],tmpMatrix[0][1],tmpMatrix[0][3]
            orientMatrix[1][0],orientMatrix[1][1],orientMatrix[1][2],orientMatrix[1][3] = tmpMatrix[2][0],tmpMatrix[2][2],tmpMatrix[2][1],tmpMatrix[2][3]
            orientMatrix[2][0],orientMatrix[2][1],orientMatrix[2][2],orientMatrix[2][3] = tmpMatrix[1][0],tmpMatrix[1][2],tmpMatrix[1][1],tmpMatrix[1][3]
            orientMatrix[3][0],orientMatrix[3][1],orientMatrix[3][2],orientMatrix[3][3] = tmpMatrix[3][0],tmpMatrix[3][2],tmpMatrix[3][1],tmpMatrix[3][3]

            # Apply the new bone matrix to the matrix list to export
            exportData.boneMatrixLst.append(orientMatrix)


#************************************************************************
#    desc:  write out the text file header
#************************************************************************
def TextWriteHeader(exportData):
    exportData.txtFile.write("Original Mesh List Count: {}\n".format(len(exportData.meshLst)))
    for object in exportData.context.scene.objects:
        if object.type == "MESH":
            exportData.txtFile.write("  Object Name: {}, Type: {}\n".format(object.name, object.type))
            if object.animation_data:
                exportData.txtFile.write("    Object has animation data\n")
            for modifier in object.modifiers:
                exportData.txtFile.write("    Modifier Name: {}, Type: {}\n".format(modifier.name, modifier.type))

    exportData.txtFile.write("Total Unique Vert Count: {}\n".format(exportData.totalVertCount))

    if exportData.exportCollision == False:
        if exportData.uniqueUVLst:
            exportData.txtFile.write("Total Unique UV Count: {}\n".format(len(exportData.uniqueUVLst)))

        if exportData.uniqueNormLst:
            exportData.txtFile.write("Total Unique Normal Count: {}\n".format(len(exportData.uniqueNormLst)))

    if exportData.faceGrpLst:
        exportData.txtFile.write("Face Group Count: {}\n".format(len(exportData.faceGrpLst)))
        for faceGrp in exportData.faceGrpLst:
            exportData.txtFile.write("  Face List Count: {}; Text Count: {}; Text Indexes:".format(len(faceGrp.faceLst), len(faceGrp.textureIndexLst)))
            for textureIndex in faceGrp.textureIndexLst:
                exportData.txtFile.write(" " + str(textureIndex))
            exportData.txtFile.write("\n")
    else:
        exportData.txtFile.write("Face List Count: {}\n".format(len(exportData.faceLst)))

    if exportData.exportCollision == False:
        if exportData.uniqueTextureLst:
            exportData.txtFile.write("Total Unique Texture Count: {}\n".format(len(exportData.uniqueTextureLst)))

    if exportData.dataBoneLst:
        exportData.txtFile.write("Total Bone Count: {}\n".format(len(exportData.dataBoneLst)))

    exportData.txtFile.write("\n")


#************************************************************************
#    desc:  write out the vert list
#************************************************************************
def TextWriteVertLst(exportData):
    # Write out all the verts
    exportData.txtFile.write("Vert List: {}\n".format(exportData.totalVertCount))

    # Export the vert data with bones
    if exportData.dataBoneLst:
        vCounter = 0
        for mesh in exportData.meshLst:
            for vert in mesh.vertices:
                bIndex = exportData.boneVertLst[vCounter]
                weight = exportData.weightVertLst[vCounter]

                # Get bone data the vert belongs to
                bone = exportData.dataBoneLst[bIndex]

                # The Y and Z are switched to match our engine's coordinate system
                vertex = mathutils.Vector((vert.co.x, vert.co.z, vert.co.y, 1))
                matrix = mathutils.Matrix(exportData.boneWorldMatrixLst[bIndex])
                newVertex = mathutils.Vector()

                # Calculate the new vert position
                newVertex[0] = (vertex[0] * matrix[0][0]) + (vertex[1] * matrix[1][0]) + (vertex[2] * matrix[2][0]) + (vertex[3] * matrix[3][0])
                newVertex[1] = (vertex[0] * matrix[0][1]) + (vertex[1] * matrix[1][1]) + (vertex[2] * matrix[2][1]) + (vertex[3] * matrix[3][1])
                newVertex[2] = (vertex[0] * matrix[0][2]) + (vertex[1] * matrix[1][2]) + (vertex[2] * matrix[2][2]) + (vertex[3] * matrix[3][2])

                # Write the vert information to the text file
                exportData.txtFile.write(" x: {:10f}, y: {:10f}, z: {:10f}, w: {:10f}, b: {} - {}\n".format(newVertex[0], newVertex[1], newVertex[2], weight, bIndex, bone.name))
                vCounter += 1

    # Export the vert data without bones
    else:
        for mesh in exportData.meshLst:
            for vert in mesh.vertices:

                # Write the vert information to the text file. The Y and Z are switched to match our engine's coordinate system.
                exportData.txtFile.write(" x: {:10f}, y: {:10f}, z: {:10f}\n".format(vert.co.x, vert.co.y, vert.co.z))


#************************************************************************
#    desc:  write out the UV list
#************************************************************************
def TextWriteUVLst(exportData):
    if exportData.exportCollision == False:
        exportData.txtFile.write("\nUV List: {}\n".format(len(exportData.uniqueUVLst)))
        for uv in exportData.uniqueUVLst:
            exportData.txtFile.write(" u: {:8f}, v: {:8f}\n".format(uv[0], uv[1]))


#************************************************************************
#    desc:  write out the Normal list
#************************************************************************
def TextWriteNormalLst(exportData):
    if exportData.exportCollision == False:
        exportData.txtFile.write("\nNormal List: {}\n".format(len(exportData.uniqueNormLst)))
        for norm in exportData.uniqueNormLst:
            normal = None
            if exportData.dataBoneLst:
                normal = mathutils.Vector((norm[0],norm[1],norm[2]))

            else:
                # The Y and Z are switched here to match our engine's coordinate system
                normal = mathutils.Vector((norm[0],norm[1],norm[2]))

            exportData.txtFile.write(" x: {:10f}, y: {:10f}, z: {:10f}\n".format(normal[0], normal[1], normal[2]))


#************************************************************************
#    desc:  Write out all face indexes
#************************************************************************
def TextWriteFaceIndexes(exportData):

    exportData.txtFile.write("\nFace Group Count: {}\n".format(len(exportData.faceGrpLst)))
    exportData.txtFile.write("\n  Each group of numbers represents one vertex.\n")
    exportData.txtFile.write("  Each row of numbers represents one face (triangle).\n")
    for faceGrp in exportData.faceGrpLst:
        exportData.txtFile.write("\n  Face List Count: {}; Text Count: {}; Text Indexes:".format(len(faceGrp.faceLst), len(faceGrp.textureIndexLst)))
        for textureIndex in faceGrp.textureIndexLst:
            exportData.txtFile.write(" " + str(textureIndex))
        exportData.txtFile.write("\n")

        for face in faceGrp.faceLst:

            if exportData.exportCollision == False:
                # May not have any textures
                if face.uvIndexLst:
                    exportData.txtFile.write("  {}, {}, {}".format(face.vertIndexLst[0], face.normIndexLst[0], face.uvIndexLst[0]))
                    exportData.txtFile.write("    {}, {}, {}".format(face.vertIndexLst[1], face.normIndexLst[1], face.uvIndexLst[1]))
                    exportData.txtFile.write("    {}, {}, {}\n".format(face.vertIndexLst[2], face.normIndexLst[2], face.uvIndexLst[2]))
                else:
                    exportData.txtFile.write("  {}, {}".format(face.vertIndexLst[0], face.normIndexLst[0]))
                    exportData.txtFile.write("    {}, {}".format(face.vertIndexLst[1], face.normIndexLst[1]))
                    exportData.txtFile.write("    {}, {}\n".format(face.vertIndexLst[2], face.normIndexLst[2]))
            else:
                exportData.txtFile.write("  {}".format(face.vertIndexLst[0]))
                exportData.txtFile.write("    {}".format(face.vertIndexLst[1]))
                exportData.txtFile.write("    {}\n".format(face.vertIndexLst[2]))

        # Wite out the VBO
        exportData.txtFile.write("\n    Vertex buffer List: {}\n".format(len(faceGrp.vertexBufLst)))
        for vert in faceGrp.vertexBufLst:
            if exportData.exportCollision == False:
                if face.uvIndexLst:
                    exportData.txtFile.write("    {}, {}, {}\n".format(vert.vertIndex, vert.normIndex, vert.uvIndex))
                else:
                    exportData.txtFile.write("    {}, {}\n".format(vert.vertIndex, vert.normIndex))
            else:
                exportData.txtFile.write("    {}\n".format(vert.vertIndex))

        # Write out the IBO
        exportData.txtFile.write("\n    Index buffer List: {}\n".format(len(faceGrp.indexBufLst)))
        counter = 1
        indexLst = []
        for index in faceGrp.indexBufLst:
            #exportData.txtFile.write("    {}\n".format(index))
            indexLst.append(index)
            if counter % 3 == 0:
                exportData.txtFile.write("    {}, {}, {}\n".format(indexLst[0], indexLst[1], indexLst[2]))
                del indexLst[:]
            counter += 1


#************************************************************************
#    desc:  Write out textures
#************************************************************************
def TextWriteTextures(exportData):
    if exportData.exportCollision == False:
        exportData.txtFile.write("\nTexture List: {}\n".format(len(exportData.uniqueTextureLst)))
        for textureSlot in exportData.uniqueTextureLst:
            textureType = ""
            if textureSlot.use_map_color_diffuse:
                textureType = "  diffuse: "
            elif textureSlot.use_map_normal:
                textureType = "  normal: "
            elif textureSlot.use_map_specular:
                textureType = "  specular: "
            elif textureSlot.use_map_displacement:
                textureType = "  displacement: "

            # Include the character length of the path so that it can be monitored
            textPathStr = textureSlot.texture.image.filepath.lstrip("/.\\")
            exportData.txtFile.write("{} len: {} - {} \n".format(textureType, len(textPathStr), textPathStr))


#************************************************************************
#    desc:  Write out bones
#************************************************************************
def TextWriteBones(exportData):
    if exportData.dataBoneLst:
        exportData.txtFile.write("\nBone Count: {}\n".format(len(exportData.dataBoneLst)))
        boneIndex = 0

        armHeadVec = mathutils.Vector((0, 0, 0))
        armTailVec = mathutils.Vector((0, 0, 0))

        for bone in exportData.dataBoneLst:
            parentName = "none"
            if bone.parent is not None:
                parentName = bone.parent.name

            elif bone.parent is None:
                parentName = "armature"
                armHeadVec = bone.head_local
                armTailVec = bone.tail_local

            # Convert to a vector to subtract armature offset
            headBoneVec = mathutils.Vector((bone.head_local[0], bone.head_local[1], bone.head_local[2]))
            headBoneVec += exportData.armPosVec

            # Convert to a vector to subtract armature offset
            tailBoneVec = mathutils.Vector((bone.tail_local[0], bone.tail_local[1], bone.tail_local[2]))
            tailBoneVec += exportData.armPosVec

            exportData.txtFile.write("  index: {}, vert count: {}, name: {}, parent: {}\n".format(boneIndex, exportData.boneVertLst.count(boneIndex), bone.name, parentName))

            # The Y and Z are switched here to match our coordinate system
            exportData.txtFile.write("  head: {:10f},{:10f},{:10f}\n".format(headBoneVec.x, headBoneVec.z, headBoneVec.y))
            exportData.txtFile.write("  tail: {:10f},{:10f},{:10f}\n".format(tailBoneVec.x, tailBoneVec.z, tailBoneVec.y))

            # Export the root bone's local matrix
            orientMatrix = mathutils.Matrix()
            orientMatrix = exportData.boneMatrixLst[boneIndex]

            exportData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(orientMatrix[0][0],orientMatrix[0][1],orientMatrix[0][2],orientMatrix[0][3]))
            exportData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(orientMatrix[1][0],orientMatrix[1][1],orientMatrix[1][2],orientMatrix[1][3]))
            exportData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(orientMatrix[2][0],orientMatrix[2][1],orientMatrix[2][2],orientMatrix[2][3]))
            exportData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n\n".format(orientMatrix[3][0],orientMatrix[3][1],orientMatrix[3][2],orientMatrix[3][3]))

            boneIndex += 1


#************************************************************************
#    desc:  write out the key frame data
#************************************************************************
def TextWriteKeyFrameData(exportData):
    if exportData.armatureObj is not None:
        exportData.txtFile.write("Bone Count: {}, Frame Start: {}, Frame End {}\n".format(len(exportData.armatureObj.data.bones),
            exportData.context.scene.frame_start,
            exportData.context.scene.frame_end))

        for keyFrameGrp in exportData.keyFrameGrpLst:
            exportData.txtFile.write("\n  Key Frame Count: {}, Name: {}, Parent: {}\n".format(len(keyFrameGrp.keyFrameLst), keyFrameGrp.name, keyFrameGrp.parent))

            for keyFrame in keyFrameGrp.keyFrameLst:
                exportData.txtFile.write("    Time: {:10f}, x: {:9f}, y: {:9f}, z: {:9f} -- rx: {:9f}, ry: {:9f}, rz: {:9f}\n".format(
                    keyFrame.time,
                    keyFrame.posX,
                    keyFrame.posY,
                    keyFrame.posZ,
                    keyFrame.rotEularX,
                    keyFrame.rotEularY,
                    keyFrame.rotEularZ))


#************************************************************************
#    desc:  write out the key frame data
#************************************************************************
def BinaryWriteKeyFrameData(exportData):
    if exportData.armatureObj is not None:
        # Fill in the file header
        fileHeader = header.CSpriteAnimationBinaryFileHeader()
        fileHeader.file_header = header.ANIMATION_FILE_HEADER
        fileHeader.bone_count = len(exportData.armatureObj.data.bones)
        fileHeader.frame_count = exportData.context.scene.frame_end

        # write the file header
        exportData.binaryFile.write(fileHeader)

        binaryJointAnimation = header.CBinaryJointAnimation()
        binaryKeyFrame = header.CBinaryKeyFrame()

        for keyFrameGrp in exportData.keyFrameGrpLst:
            binaryJointAnimation.keyframe_count = len(keyFrameGrp.keyFrameLst)
            binaryJointAnimation.name = keyFrameGrp.name.encode('utf-8')
            binaryJointAnimation.parent = keyFrameGrp.parent.encode('utf-8')
            exportData.binaryFile.write(binaryJointAnimation)

            # write the tag check
            BinaryWriteTagCheck(exportData)

            for keyFrame in keyFrameGrp.keyFrameLst:
                binaryKeyFrame.time = keyFrame.time
                binaryKeyFrame.x = keyFrame.posX
                binaryKeyFrame.y = keyFrame.posY
                binaryKeyFrame.z = keyFrame.posZ
                binaryKeyFrame.rx = keyFrame.rotEularX
                binaryKeyFrame.ry = keyFrame.rotEularY
                binaryKeyFrame.rz = keyFrame.rotEularZ
                exportData.binaryFile.write(binaryKeyFrame)


#************************************************************************
#    desc:  write out the binary file header
#************************************************************************
def BinaryWriteHeader(exportData):
    # Fill in the file header
    fileHeader = header.CSpriteBinaryFileHeader()
    fileHeader.file_header = header.SPRITE_FILE_HEADER
    fileHeader.vert_count = exportData.totalVertCount
    fileHeader.uv_count = len(exportData.uniqueUVLst)
    fileHeader.vert_norm_count = len(exportData.uniqueNormLst)
    fileHeader.face_group_count = len(exportData.faceGrpLst)
    fileHeader.mat_count = len(exportData.uniqueTextureLst)
    fileHeader.joint_count = len(exportData.dataBoneLst)

    # write the file header
    exportData.binaryFile.write(fileHeader)


#************************************************************************
#    desc:  write out the binary tag check
#************************************************************************
def BinaryWriteTagCheck(exportData):

    # Header object for writing tage checks
    tagCheck = header.CTagCheck()

    # write the vert list tag check
    tagCheck.tagCheck = header.TAG_CHECK;
    exportData.binaryFile.write(tagCheck)


#************************************************************************
#    desc:  write out the binary vert
#************************************************************************
def BinaryWriteVertLst(exportData):

    # write the tag check
    BinaryWriteTagCheck(exportData)

    # Export the vert data with bones
    if exportData.dataBoneLst:
        binaryVert = header.CBinaryVertBone()
        vCounter = 0
        for mesh in exportData.meshLst:
            for vert in mesh.vertices:

                bIndex = exportData.boneVertLst[vCounter]

                # The Y and Z are switched to match our engine's coordinate system
                vertex = mathutils.Vector((vert.co.x, vert.co.z, vert.co.y, 1))
                matrix = mathutils.Matrix(exportData.boneWorldMatrixLst[bIndex])
                newVertex = mathutils.Vector()

                # Calculate the new vert position
                newVertex[0] = (vertex[0] * matrix[0][0]) + (vertex[1] * matrix[1][0]) + (vertex[2] * matrix[2][0]) + (vertex[3] * matrix[3][0])
                newVertex[1] = (vertex[0] * matrix[0][1]) + (vertex[1] * matrix[1][1]) + (vertex[2] * matrix[2][1]) + (vertex[3] * matrix[3][1])
                newVertex[2] = (vertex[0] * matrix[0][2]) + (vertex[1] * matrix[1][2]) + (vertex[2] * matrix[2][2]) + (vertex[3] * matrix[3][2])

                # Write the vert information to the binary file
                binaryVert.x = newVertex[0]
                binaryVert.y = newVertex[1]
                binaryVert.z = newVertex[2]
                binaryVert.w = exportData.weightVertLst[vCounter]
                binaryVert.b = bIndex
                exportData.binaryFile.write(binaryVert)
                vCounter += 1

    # Export the vert data without bones
    else:
        binaryVert = header.CBinaryVert()
        for mesh in exportData.meshLst:
            for vert in mesh.vertices:

                # The Y and Z are switched here for the binary vert list
                binaryVert.x = vert.co.x
                binaryVert.y = vert.co.y
                binaryVert.z = vert.co.z

                # Write the vert information to the binary file
                exportData.binaryFile.write(binaryVert)


#************************************************************************
#    desc:  write out the binary UV
#************************************************************************
def BinaryWriteUVLst(exportData):

    # Only write the header if there's UV's to write
    if exportData.uniqueUVLst:
        # write the tag check
        BinaryWriteTagCheck(exportData)

    # Write out all the UVs
    binaryUV = header.CBinaryUV()
    for uv in exportData.uniqueUVLst:
        binaryUV.u = uv[0]
        binaryUV.v = uv[1]
        exportData.binaryFile.write(binaryUV)


#************************************************************************
#    desc:  write out the Normal list
#************************************************************************
def BinaryWriteNormalLst(exportData):

    # Only write the header if there's normals to write
    if exportData.uniqueNormLst:
        BinaryWriteTagCheck(exportData)

    # Write out all the vert normals
    binaryNorm = header.CBinaryVert()
    for norm in exportData.uniqueNormLst:
        normal = None
        if exportData.dataBoneLst:
            normal = mathutils.Vector((norm[0],norm[1],norm[2]))

        #############################################################################
        # The Y and Z are switched here for the binary normal list
        #############################################################################
        else:
            normal = mathutils.Vector((norm[0],norm[1],norm[2]))
            
        binaryNorm.x = normal[0]
        binaryNorm.y = normal[1]
        binaryNorm.z = normal[2]
        exportData.binaryFile.write(binaryNorm)


#************************************************************************
#    desc:  Write out all face indexes
#************************************************************************
def BinaryWriteFaceIndexes(exportData):

    # Write out all face indexes
    for faceGrp in exportData.faceGrpLst:

        # write the tag check
        BinaryWriteTagCheck(exportData)

        # all index lists should be the same size
        faceInfo = header.CBinaryFaceGroup()
        faceInfo.groupFaceCount = len(faceGrp.faceLst)
        faceInfo.vertexBufCount = len(faceGrp.vertexBufLst)
        faceInfo.indexBufCount = len(faceGrp.indexBufLst)
        faceInfo.textureCount = len(faceGrp.textureIndexLst)
        exportData.binaryFile.write(faceInfo)

        # List of texture indexes
        binaryTextIndex = header.CBinaryTextIndex()
        for textIndex in faceGrp.textureIndexLst:
            binaryTextIndex.textureIndex = textIndex
            exportData.binaryFile.write(binaryTextIndex)

        # May not have any textures
        # Write out the indexes into the vert, norm & UV list to create the VBO
        binaryVertex = None
        if exportData.uniqueUVLst:
            binaryVertex = header.CBinaryVertex()
        else:
            binaryVertex = header.CBinaryVertexNoText()

        for vert in faceGrp.vertexBufLst:
            binaryVertex.vert = vert.vertIndex
            if exportData.uniqueNormLst:
                binaryVertex.norm = vert.normIndex
            if exportData.uniqueUVLst:
                binaryVertex.uv = vert.uvIndex

            exportData.binaryFile.write(binaryVertex)

        # Write out the indexes to create the IBO
        binaryIBO = header.CBinaryIBO()

        for index in faceGrp.indexBufLst:
            binaryIBO.ibo = index
            exportData.binaryFile.write(binaryIBO)


#************************************************************************
#    desc:  Write out textures
#************************************************************************
def BinaryWriteTextures(exportData):

    # Only write the header if there's textures to write
    if exportData.uniqueTextureLst:
        # write the tag check
        BinaryWriteTagCheck(exportData)

    # Write the material paths
    for textureSlot in exportData.uniqueTextureLst:
        texturePath = header.CTexturePath()
        textPathStr = textureSlot.texture.image.filepath.lstrip("/.\\")
        texturePath.path = textPathStr.encode('utf-8')

        # Make sure we haven't exceeded the space allowed for file paths
        if len(textPathStr) > header.TEXTURE_PATH_SIZE:
            raise RuntimeError("Texture path exceeds space allowed! - " + textPathStr)

        textureType = header.TEXTURE_NULL
        if textureSlot.use_map_color_diffuse:
            textureType = header.TEXTURE_DIFFUSE
        elif textureSlot.use_map_normal:
            textureType = header.TEXTURE_NORMAL
        elif textureSlot.use_map_specular:
            textureType = header.TEXTURE_SPECULAR
        elif textureSlot.use_map_displacement:
            textureType = header.TEXTURE_DISPLACEMENT

        texturePath.textureType = textureType
        exportData.binaryFile.write(texturePath)


#************************************************************************
#    desc:  Write out bones
#************************************************************************
def BinaryWriteBones(exportData):

    # Only write the header if there's bones to write
    if exportData.dataBoneLst:
        # write the tag check
        BinaryWriteTagCheck(exportData)

    # write out the bone data
    boneIndex = 0
    for bone in exportData.dataBoneLst:
        binaryJoint = header.CBinaryJoint()
        parentName = ""
        if bone.parent is not None:
            parentName = bone.parent.name

        binaryJoint.vert_count = exportData.boneVertLst.count(boneIndex)
        binaryJoint.name = bone.name.encode('utf-8')
        binaryJoint.parent = parentName.encode('utf-8')

        # Convert to a vector to subtract armature offset
        headBoneVec = mathutils.Vector((bone.head_local[0], bone.head_local[1], bone.head_local[2]))
        headBoneVec += exportData.armPosVec

        #############################################################################
        # The Y and Z are switched here for the binary joint head
        #############################################################################
        binaryJoint.headX = headBoneVec.x
        binaryJoint.headY = headBoneVec.y
        binaryJoint.headZ = headBoneVec.z

        # Convert to a vector to subtract armature offset
        tailBoneVec = mathutils.Vector((bone.tail_local[0], bone.tail_local[1], bone.tail_local[2]))
        tailBoneVec += exportData.armPosVec

        #############################################################################
        # The Y and Z are switched here for the binary joint tail
        #############################################################################
        binaryJoint.tailX = tailBoneVec.x
        binaryJoint.tailY = tailBoneVec.y
        binaryJoint.tailZ = tailBoneVec.z

        orientMatrix = mathutils.Matrix()
        orientMatrix = exportData.boneMatrixLst[boneIndex]

        # X column
        binaryJoint.m00 = orientMatrix[0][0]
        binaryJoint.m10 = orientMatrix[1][0]
        binaryJoint.m20 = orientMatrix[2][0]
        binaryJoint.m30 = orientMatrix[3][0]

        # Y column
        binaryJoint.m01 = orientMatrix[0][1]
        binaryJoint.m11 = orientMatrix[1][1]
        binaryJoint.m21 = orientMatrix[2][1]
        binaryJoint.m31 = orientMatrix[3][1]

        # Z column
        binaryJoint.m02 = orientMatrix[0][2]
        binaryJoint.m12 = orientMatrix[1][2]
        binaryJoint.m22 = orientMatrix[2][2]
        binaryJoint.m32 = orientMatrix[3][2]

        # Fourth column
        binaryJoint.m03 = orientMatrix[0][3]
        binaryJoint.m13 = orientMatrix[1][3]
        binaryJoint.m23 = orientMatrix[2][3]
        binaryJoint.m33 = orientMatrix[3][3]

        exportData.binaryFile.write(binaryJoint)

        boneIndex += 1

#************************************************************************
#    desc:  Export Operator
#************************************************************************
class Export_waffles(bpy.types.Operator):
    '''Exports scene as Waffles 3D Mesh Format .3dm file.'''
    bl_idname = "export_scene.3dm"
    bl_label = "Export 3D Mesh (.3dm)"

    filepath = StringProperty(subtype='FILE_PATH')

    # General Options
    exportCollisionMesh = BoolProperty(name="Export as Collision Mesh", description="Export the mesh as a collision mesh", default=False)
    exportArmature = BoolProperty(name="Export Armature (when available)", description="Export the mesh and armature if the armature is present", default=False)
    exportAnimFrames = BoolProperty(name="Export Animation Frames Only", description="Only export the animation frame data", default=False)

    def execute(self, context):

        # Create the export data
        exportData = ExportData(context)

        filename_3d = ".3dm"
        filename_txt = ".txt"

        if self.exportCollisionMesh:
            filename_3d = "_collision.3dm"
            filename_txt = "_collision.txt"

        elif self.exportAnimFrames:
            filename_3d = "_animation.3da"
            filename_txt = "_animation.txt"

        filepath = self.filepath
        filename_3d = bpy.path.ensure_ext(filepath, filename_3d)
        filepath_txt = bpy.path.ensure_ext(filepath, filename_txt)

        # Open the files for writing
        exportData.txtFile = open(filepath_txt, "w")
        exportData.binaryFile = open(filename_3d, "wb")

        # Set if we are to export armatures
        exportData.exportArmature = self.exportArmature

        # Set if we are exporting collision mesh
        exportData.exportCollision = self.exportCollisionMesh

        # Call the functions that export the requested data
        if self.exportAnimFrames:
            export_animationFrames(exportData)

        else:
            export_mesh(exportData)

        # Flush and close the files
        exportData.txtFile.flush()
        exportData.txtFile.close()

        exportData.binaryFile.flush()
        exportData.binaryFile.close()

        return {'FINISHED'}


    def invoke(self, context, event):
        wm = context.window_manager

        if True:
            # File selector
            wm.fileselect_add(self) # will run self.execute()
            return {'RUNNING_MODAL'}
        elif True:
            # search the enum
            wm.invoke_search_popup(self)
            return {'RUNNING_MODAL'}
        elif False:
            # Redo popup
            return wm.invoke_props_popup(self, event)
        elif False:
            return self.execute(context)


#************************************************************************
#    desc:  Plugin Register realed functions
#************************************************************************

def menu_func(self, context):
    self.layout.operator(Export_waffles.bl_idname, text="Export Waffles (.3dm)")

def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()


