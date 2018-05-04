# mesh_relax.py Copyright (C) 2010, Fabian Fricke
#
# Relaxes selected vertices while retaining the shape as much as possible
#
# ***** BEGIN GPL LICENSE BLOCK *****
#
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ***** END GPL LICENCE BLOCK *****

bl_info = {
    "name": "Zero Joints",
    "author": "Howard Rosenorn",
    "version": (1,1),
    "blender": (2, 5, 9),
    "api": 37702,
    "location": "View3D > Specials > Zero Joints",
    "description": "Zero out the joints of an Armature",
    "warning": "",
    "wiki_url": "http://wiki.blender.org/index.php/Extensions:2.5/Py/"\
        "Scripts/Modeling/ZeroJoints",
    "tracker_url": "https://projects.blender.org/tracker/index.php?"\
        "func=detail&aid=21421",
    "category": "Rigging"}

"""
Usage:

Launch from "W-menu" or from "Mesh -> Vertices -> ZeroJoints"


Additional links:
    Author Site: http://frigi.designdevil.de
    e-mail: frigi.f {at} gmail {dot} com
"""

import bpy
from bpy.props import IntProperty
import mathutils, math, struct

#************************************************************************
#    desc:  Container for the exporter settings
#************************************************************************
class ToolData:
    def __init__(self, context):
        self.context = context

        # debug Text file output
        self.txtFile = None


#************************************************************************
#    desc:  Zero out the joints
# 
#    param: ToolData 
#************************************************************************
def zero_joints(toolData):

    # deselect everything that's not related
    for object in toolData.context.selected_objects:
        object.select = False

    # get active object
    object = toolData.context.active_object
    tmpMatrix = object.matrix_world.inverted()

    toolData.txtFile.write("Armature world matrix (4x4):\n")
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(object.matrix_world[0][0],object.matrix_world[0][1],object.matrix_world[0][2],object.matrix_world[0][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(object.matrix_world[1][0],object.matrix_world[1][1],object.matrix_world[1][2],object.matrix_world[1][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(object.matrix_world[2][0],object.matrix_world[2][1],object.matrix_world[2][2],object.matrix_world[2][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(object.matrix_world[3][0],object.matrix_world[3][1],object.matrix_world[3][2],object.matrix_world[3][3]))

    toolData.txtFile.write("\nArmature world matrix inverted (4x4):\n")
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(tmpMatrix[0][0],tmpMatrix[0][1],tmpMatrix[0][2],tmpMatrix[0][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(tmpMatrix[1][0],tmpMatrix[1][1],tmpMatrix[1][2],tmpMatrix[1][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(tmpMatrix[2][0],tmpMatrix[2][1],tmpMatrix[2][2],tmpMatrix[2][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(tmpMatrix[3][0],tmpMatrix[3][1],tmpMatrix[3][2],tmpMatrix[3][3]))

    # Rotation conversion matric
    rotMatrix = mathutils.Matrix()
    # Rotate 180 on Z
    rotMatrix *= mathutils.Matrix.Rotation(math.pi/2, 4, "Z")
    # Rotate 180 on Y
    rotMatrix *= mathutils.Matrix.Rotation(math.pi/2, 4, "Y")
    # Rotate 180 on X
    rotMatrix *= mathutils.Matrix.Rotation(math.pi/2, 4, "X")

    toolData.txtFile.write("\nRotation Matrix (4x4):\n")
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(rotMatrix[0][0],rotMatrix[0][1],rotMatrix[0][2],rotMatrix[0][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(rotMatrix[1][0],rotMatrix[1][1],rotMatrix[1][2],rotMatrix[1][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(rotMatrix[2][0],rotMatrix[2][1],rotMatrix[2][2],rotMatrix[2][3]))
    toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(rotMatrix[3][0],rotMatrix[3][1],rotMatrix[3][2],rotMatrix[3][3]))

    armaturePosVec = mathutils.Vector((object.matrix_world[3][0], object.matrix_world[3][1], object.matrix_world[3][2]))

    # conversion matric
    conversionMatrix = mathutils.Matrix()
    # converts to left hand system
    conversionMatrix *= mathutils.Matrix.Scale(-1, 4, mathutils.Vector((0, 1, 0)))
    # Rotate X
    conversionMatrix *= mathutils.Matrix.Rotation(math.pi/2, 4, "X")
    # Rotate Z
    conversionMatrix *= mathutils.Matrix.Rotation(math.pi, 4, "Z")

    for bone in object.data.bones:
        boneVecNor = bone.vector.normalized()
        localQuat = bone.matrix_local.to_3x3().to_quaternion()
        globalQuat = bone.matrix.to_quaternion()

        headLocalVec = mathutils.Vector((bone.head_local[0]+armaturePosVec.x, -(bone.head_local[1]+armaturePosVec.y), -(bone.head_local[2]+armaturePosVec.z)))
        headLocalVec = headLocalVec * conversionMatrix

        tailLocalVec = mathutils.Vector((bone.tail_local[0]+armaturePosVec.x, -(bone.tail_local[1]+armaturePosVec.y), -(bone.tail_local[2]+armaturePosVec.z)))
        tailLocalVec = tailLocalVec * conversionMatrix

        toolData.txtFile.write("\n{} Bone Data\n".format(bone.name))
        toolData.txtFile.write("\n  Local Unconverted (X,Y,Z):\n")
        toolData.txtFile.write("  Head - {:9f},{:9f},{:9f}\n".format(bone.head_local[0], bone.head_local[1], bone.head_local[2]))
        toolData.txtFile.write("  Tail - {:9f},{:9f},{:9f}\n".format(bone.tail_local[0], bone.tail_local[1], bone.tail_local[2]))

        toolData.txtFile.write("\n  Relative to Armature (X,Y,Z):\n")
        toolData.txtFile.write("  Head - {:9f},{:9f},{:9f}\n".format(headLocalVec.x, headLocalVec.z, headLocalVec.y))
        toolData.txtFile.write("  Tail - {:9f},{:9f},{:9f}\n".format(tailLocalVec.x, tailLocalVec.y, tailLocalVec.z))

        localVec = headLocalVec - tailLocalVec
        toolData.txtFile.write("\n  Bone Local Vector:\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f}\n".format(localVec.x,localVec.y,localVec.z))

        localVec.normalize()
        toolData.txtFile.write("\n  Bone Local Normalized Vector:\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f}\n".format(localVec.x,localVec.y,localVec.z))

        toolData.txtFile.write("\n  Relative to Parent (X,Y,Z):\n")
        toolData.txtFile.write("  Head - {:9f},{:9f},{:9f}\n".format(bone.head[0], bone.head[1], bone.head[2]))
        toolData.txtFile.write("  Tail - {:9f},{:9f},{:9f}\n".format(bone.tail[0], bone.tail[1], bone.tail[2]))

        toolData.txtFile.write("\n  Matrix Local (4x4):\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(bone.matrix_local[0][0],bone.matrix_local[0][1],bone.matrix_local[0][2],bone.matrix_local[0][3]))
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(bone.matrix_local[1][0],bone.matrix_local[1][1],bone.matrix_local[1][2],bone.matrix_local[1][3]))
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(bone.matrix_local[2][0],bone.matrix_local[2][1],bone.matrix_local[2][2],bone.matrix_local[2][3]))
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(bone.matrix_local[3][0],bone.matrix_local[3][1],bone.matrix_local[3][2],bone.matrix_local[3][3]))

        matrixLocalInverted = bone.matrix_local.inverted()

        toolData.txtFile.write("\n  Inverted Matrix Local (4x4):\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(matrixLocalInverted[0][0],matrixLocalInverted[0][1],matrixLocalInverted[0][2],matrixLocalInverted[0][3]))
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(matrixLocalInverted[1][0],matrixLocalInverted[1][1],matrixLocalInverted[1][2],matrixLocalInverted[1][3]))
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(matrixLocalInverted[2][0],matrixLocalInverted[2][1],matrixLocalInverted[2][2],matrixLocalInverted[2][3]))
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(matrixLocalInverted[3][0],matrixLocalInverted[3][1],matrixLocalInverted[3][2],matrixLocalInverted[3][3]))

        toolData.txtFile.write("\n  Matrix (3x3):\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f}\n".format(bone.matrix[0][0],bone.matrix[0][1],bone.matrix[0][2]))
        toolData.txtFile.write("  {:9f},{:9f},{:9f}\n".format(bone.matrix[1][0],bone.matrix[1][1],bone.matrix[1][2]))
        toolData.txtFile.write("  {:9f},{:9f},{:9f}\n".format(bone.matrix[2][0],bone.matrix[2][1],bone.matrix[2][2]))

        toolData.txtFile.write("\n  Bone Vector:\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f}\n".format(bone.vector.x,bone.vector.y,bone.vector.z))

        toolData.txtFile.write("\n  Bone Vector Normalized:\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f}\n".format(boneVecNor.x,boneVecNor.y,boneVecNor.z))

        toolData.txtFile.write("\n  Bone Axis Vector:\n")
        toolData.txtFile.write("  X - {:9f},{:9f},{:9f}\n".format(bone.x_axis.x,bone.x_axis.y,bone.x_axis.z))
        toolData.txtFile.write("  Y - {:9f},{:9f},{:9f}\n".format(bone.y_axis.x,bone.y_axis.y,bone.y_axis.z))
        toolData.txtFile.write("  Z - {:9f},{:9f},{:9f}\n".format(bone.z_axis.x,bone.z_axis.y,bone.z_axis.z))

        toolData.txtFile.write("\n  Local Quaternion (X,Y,Z,W):\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(localQuat.x,localQuat.y,localQuat.z,localQuat.w))

        toolData.txtFile.write("\n  Global Quaternion (X,Y,Z,W):\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f},{:9f}\n".format(globalQuat.x,globalQuat.y,globalQuat.z,globalQuat.w))

        euler = localQuat.to_euler("XYZ")

        toolData.txtFile.write("\n  Local Quaternion to Euler (X,Y,Z):\n")
        toolData.txtFile.write("  {:9f},{:9f},{:9f}\n".format(euler.x,euler.y,euler.z))

        toolData.txtFile.write("\n")


        eulerLocal = bone.matrix_local.to_3x3().to_quaternion().to_euler("XYZ")
        euler = bone.matrix.to_quaternion().to_euler("XYZ")
        quat = bone.matrix.to_quaternion()
        boneVec = bone.vector.normalized()
        #toolData.txtFile.write("{:9} - {:9f},{:9f},{:9f} - {:9f},{:9f},{:9f} - {:9f},{:9f},{:9f} - {:9f},{:9f},{:9f}\n".format(bone.name,eulerLocal.x,eulerLocal.y,eulerLocal.z,euler.x,euler.y,euler.z,bone.vector.x,bone.vector.y,bone.vector.z,boneVec.x,boneVec.y,boneVec.z))

        #toolData.txtFile.write("{:9} - {:9f},{:9f},{:9f} - {:9f},{:9f},{:9f} - {:9f},{:9f},{:9f}\n".format(bone.name,bone.x_axis.x,bone.x_axis.y,bone.x_axis.z,bone.y_axis.x,bone.y_axis.y,bone.y_axis.z,bone.z_axis.x,bone.z_axis.y,bone.z_axis.z))

        #toolData.txtFile.write("{:9} - {:9f},{:9f},{:9f}\n".format(bone.name,bone.vector.x,bone.vector.y,bone.vector.z))

        #rVec = mathutils.Vector((bone.tail[0]-bone.head[0],bone.tail[1]-bone.head[1],bone.tail[2]-bone.head[2]))
        #toolData.txtFile.write("{:9} - {:9f},{:9f}, - {:9f},{:9f} - {:9f},{:9f}\n".format(bone.name,bone.tail[0],bone.head[0],bone.tail[1],bone.head[1],bone.tail[2],bone.head[2]))

        #bone.use_local_location = True
        #bone.matrix_local.identity()
        #bone.matrix.identity()
        #bone.matrix_local[3][0] = bone.head_local[0]
        #bone.matrix_local[3][1] = bone.head_local[1]
        #bone.matrix_local[3][2] = bone.head_local[2]

    
    #toolData.txtFile.write(object.name)
    

    # duplicate the object so it can be used for the shrinkwrap modifier
#    obj.select = True # make sure the object is selected!
#    bpy.ops.object.mode_set(mode='OBJECT')
#    bpy.ops.object.duplicate()
#    target = context.active_object

    # remove all other modifiers from the target
#    for m in range(0, len(target.modifiers)):
#        target.modifiers.remove(target.modifiers[0])

#    context.scene.objects.active = obj
    
#    sw = obj.modifiers.new(type='SHRINKWRAP', name='relax_target')
#    sw.target = target
    
    # run smooth operator to relax the mesh
#    bpy.ops.object.mode_set(mode='EDIT')
#    bpy.ops.mesh.vertices_smooth()
#    bpy.ops.object.mode_set(mode='OBJECT')

    # apply the modifier
#    bpy.ops.object.modifier_apply(modifier='relax_target')
    
    # delete the target object
#    obj.select = False
#    target.select = True
#    bpy.ops.object.delete()
    
    # go back to initial state
#    obj.select = True
#    bpy.ops.object.mode_set(mode='EDIT')

class ZeroJoints(bpy.types.Operator):
    '''Zeros out the joints'''
    bl_idname = 'mesh.relax'
    bl_label = 'Zero Joints'
    bl_options = {'REGISTER', 'UNDO'}

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return (obj and obj.type == 'ARMATURE')

    def execute(self, context):

        # Create the export data
        toolData = ToolData(context)

        # Open a text file for debug info
        toolData.txtFile = open("C:\\zero_joints.txt", "w")

        # Zero out the joints
        zero_joints(toolData)

        toolData.txtFile.flush()
        toolData.txtFile.close()

        return {'FINISHED'}


def menu_func(self, context):
    self.layout.operator(ZeroJoints.bl_idname, text="Zero Joints")

def register():
    bpy.utils.register_module(__name__)
    bpy.types.VIEW3D_MT_armature_specials.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.VIEW3D_MT_armature_specials.remove(menu_func)

if __name__ == "__main__":
    register()
