
bl_info = {
    "name": "Export waffles Level(.xml)",
    "author": "Rosenorn",
    "version": (1, 0),
    "blender": (2, 5, 9),
    "api": 37702,
    "location": "File > Export > waffles Level (.xml)",
    "description": "Export level (.xml)",
    "warning": "",
    "wiki_url": "http://wiki.blender.org/index.php/Extensions:2.5/Py/"\
        "Scripts/Import-Export/PC2_Pointcache_export",
    "tracker_url": "https://projects.blender.org/tracker/index.php?"\
        "func=detail&aid=24703",
    "category": "Import-Export"}

import os
import bpy
from bpy.props import *
import mathutils, math, struct
from os import remove
import time
from ctypes import *

# This is how to raise an exception
# raise RuntimeError('This export only supports one armature set')

#************************************************************************
#    desc:  Container for the exporter settings
#************************************************************************
class ExportData:
    def __init__(self, context):
        self.context = context
        self.xmlFile = None



#************************************************************************
#    desc:  export object positions
# 
#    param: exportData 
#************************************************************************
def export_level(exportData):
    dummy = None






#************************************************************************
#    desc:  Export Operator
#************************************************************************
class Export_waffles_Level(bpy.types.Operator):
    '''Exports scene as waffles Level .xml file.'''
    bl_idname = "export_scene.xml"
    bl_label = "Export waffles Level (.xml)"

    filepath = StringProperty(subtype='FILE_PATH')

    def execute(self, context):

        # Create the export data
        exportData = ExportData(context)

        filepath_xml = bpy.path.ensure_ext(self.filepath, ".xml")

        # Open the files for writing
        exportData.xmlFile = open(filepath_xml, "w")

        # Call the functions that export the data
        export_level(exportData)

        # Flush and close the files
        exportData.xmlFile.flush()
        exportData.xmlFile.close()
              
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
    self.layout.operator(Export_RabidSquirrel_Level.bl_idname, text="Export waffles level (.xml)")

def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func)
    
if __name__ == "__main__":
    register()
