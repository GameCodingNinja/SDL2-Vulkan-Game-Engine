#!/usr/bin/env python
# -*- coding: utf-8 -*-
#    This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#   To Install, copy file to...
#   1) Linux: usr/lib/gimp/2.0/plug-ins/

from gimpfu import *
import os
import sys

gettext.install("gimp20-python", gimp.locale_directory, unicode=True)

#sys.stderr = open( '/gimpstderr.txt', 'w')
#sys.stdout = open( '/gimpstdout.txt', 'w')
# print example
# print( "%s, of: %d lc: %d, rc: %d, w: %d" % (layer.name, offsetX, leftCropX, rightCropX, layer.width) )

# Globals
X = 0
Y = 1
W = 2
H = 3
N = 4
OF = 5

#************************************************************************
#    desc:  class for recording layer with index
#************************************************************************
class CPair:
    # Init class members
    def __init__(self):
        self.index = -1
        self.w = 0
        self.h = 0
        self.layerWidth = 0
        self.layerHeight = 0
        self.layer = None
        
        
#************************************************************************
#    desc:  Sort layers by height
#************************************************************************
def SortByHeight(item):
    return item.layer.height
    
    
#************************************************************************
#    desc:  Sort by index
#************************************************************************
def SortByIndex(item):
    return item.index


#************************************************************************
#    desc:  Sort by index
#************************************************************************
def ForceEvenOdd(forceEvenOdd, value):
    
    # Force odd or even image sizes if enabled
    if forceEvenOdd == "even":
        if not value % 2 == 0:
            value += 1

    elif forceEvenOdd == "odd":
        if value % 2 == 0:
            value += 1
            
    return value


#************************************************************************
#    desc:  class for recording node information
#************************************************************************
class CNode:
    # Init class members
    def __init__(self):
        self.rect = (0,0,0,0)
        self.layerWidth = 0
        self.layerHeight = 0
        self.index = -1
        self.layer = None
        self.node = None

    def Insert( self, pair ):

        # If we are not a leaf
        if self.node is not None:

            # Try inserting into first node
            result = self.node[0].Insert( pair )

            if result is not None:
                return result;

            # If that didn't work, try the second node
            return self.node[1].Insert( pair )

        else:

            # If already in use
            if self.index > -1:
                return None;

            # Return if doesn't fit                
            if (pair.w > self.rect[W]) or (pair.h > self.rect[H]):
                return None

            # Return if perfect fit                
            if (pair.w == self.rect[W]) and (pair.h == self.rect[H]):
                self.index = pair.index
                self.layer = pair.layer
                self.layerWidth = pair.layerWidth
                self.layerHeight = pair.layerHeight
                return self

            # Split this node and create some kids
            self.node = ( CNode(), CNode() )

            # Decide which way to split
            width = self.rect[W] - pair.w
            height = self.rect[H] - pair.h

            if width > height:
                self.node[0].rect = ( self.rect[X], self.rect[Y], pair.w, self.rect[H] )
                self.node[1].rect = ( self.rect[X] + pair.w, self.rect[Y], self.rect[W] - pair.w, self.rect[H] )
            else:
                self.node[0].rect = ( self.rect[X], self.rect[Y], self.rect[W], pair.h )
                self.node[1].rect = ( self.rect[X], self.rect[Y] + pair.h, self.rect[W], self.rect[H] - pair.h )
                
            # Insert into first child created
            return self.node[0].Insert( pair )
        

#************************************************************************
#    desc:  Entry point for plugin - 
#           This function is called when the OK button is pressed
#************************************************************************
def CreateSpriteSheet(image, simpleSpriteSheet, columns, maxWidth, exportFile, exportPath, exportFilename, padding, cropImage, forceEvenOdd):

    # Define the list of rects
    rectLst = []
    
    # Inform them that they need more then one layer to make a sprite sheet
    if len(image.layers) < 2:
        pdb.gimp_message('You need more then one layer to make a sprite sheet.')
        return
        
    # Inform them that padding needs to be a positive number
    if padding < 0:
        pdb.gimp_message('Please specify "Padding" with a number greater then -1.')
        return None

    # Are all layers the same size?
    if simpleSpriteSheet:
        newImg = CreateSimple( image, columns, rectLst, padding, forceEvenOdd )
    else:
        newImg = CreateComplex( image, maxWidth, rectLst, padding, cropImage, forceEvenOdd )
    
    if newImg is not None:
        newImg.enable_undo()
        newImg.clean_all()
        gimp.Display(newImg)
        
        pdb.gimp_image_undo_thaw( image )
        gimp.displays_flush()
        
        if exportFilename != "":
            if exportFile == "xml":
                CreateXML( exportPath, exportFilename, rectLst, newImg.width, newImg.height )
            elif exportFile == "json":
                CreateJSON( exportPath, exportFilename, rectLst, newImg.width, newImg.height )
        elif exportFile != "none":
            pdb.gimp_message('Export File option has been selected but no file name has been entered. Please specify a name for the Export File.')


#************************************************************************
#    desc:  Create a simple sprite sheet based on columns and rows
#************************************************************************
def CreateSimple( image, columns, rectLst, padding, forceEvenOdd ):

    if columns < 1:
        pdb.gimp_message('Please specify a "Number of Columns" that is greater then zero.')
        return None
        
    # copy over only the visible layers
    layers = []
    for layer in image.layers:
        if pdb.gimp_item_get_visible(layer):
            layers.append( layer )
            
    layerCount = len(layers)
        
    # First layer width and height will be used to define the size of a simple sprite sheet
    layerWidth = layers[-1].width
    layerHeight = layers[-1].height
    
    # Force odd or even image sizes if enabled
    layerWidth = ForceEvenOdd( forceEvenOdd, layerWidth )
    layerHeight = ForceEvenOdd( forceEvenOdd, layerHeight )
    
    paddedWidth = layerWidth + (padding * 2)
    paddedHeight = layerHeight + (padding * 2)
    layerWidthDiv2 = layerWidth / 2
    layerHeightDiv2 = layerHeight / 2
    
    # Calculate the number of rows
    rows = layerCount / columns
    if not layerCount % columns == 0:
        rows += 1
    
    newImageWidth = (paddedWidth * columns)
    newImageHeight = (paddedHeight * rows)
    newImageWidthDiv2 = newImageWidth / 2
    newImageHeightDiv2 = newImageHeight / 2
    
    # Create the image
    newImg = gimp.Image(newImageWidth, newImageHeight, RGB)
    newImg.disable_undo()

    newLayer = gimp.Layer(newImg, "sprite sheet", newImageWidth, newImageHeight, RGBA_IMAGE, 100, NORMAL_MODE)
    newImg.add_layer(newLayer, 0)
    #layers[counter].name
    delta = 1.0 / layerCount
    percentComplete = 0.0
    counter = layerCount - 1
    for y in range(0, rows):
        for x in range(0, columns):
            
            # Get the name of the file with the extension stripped out
            nameMinusExt = os.path.splitext(os.path.basename(layers[counter].name))[0]
            
            # Add the rect of the image
            rect = ((x * paddedWidth) + padding, (y * paddedHeight) + padding, layerWidth, layerHeight, nameMinusExt, (0,0))
            rectLst.append( rect )
            
            # Copy the image layer to the new layer
            pdb.gimp_edit_copy( layers[counter] )
            floatingLayer = pdb.gimp_edit_paste( newLayer, False )
    
            # Calculate the offset
            offsetX = -(newImageWidthDiv2 - layerWidthDiv2)
            offsetY = -(newImageHeightDiv2 - layerHeightDiv2)
            
            # Correct any offset issues due to the division of images with odd number width and heights
            dif = offsetX + floatingLayer.offsets[0]
            if dif != 0:
                offsetX += dif * -1
                
            dif = offsetY + floatingLayer.offsets[1]
            if dif != 0:
                offsetY += dif * -1
    
            # Position the new layer into it's spot
            pdb.gimp_layer_translate( floatingLayer, offsetX + rect[X], offsetY + rect[Y] )
            pdb.gimp_floating_sel_anchor( floatingLayer )
            
            # Increment the progress bar
            percentComplete += delta
            pdb.gimp_progress_update( percentComplete )
            
            counter -= 1
            
            if counter == -1:
                break
                
    return newImg
    
    
#************************************************************************
#    desc:  Create a complex sprite sheet based on layers of different sizes
#************************************************************************
def CreateComplex( image, maxWidth, rectLst, padding, cropImage, forceEvenOdd ):
    
    if maxWidth < 1:
        pdb.gimp_message('Please specify a "Pack Width" that is greater then zero.')
        return None
        
    # Duplicate Image. It makes the code simpler
    localImage = pdb.gimp_image_duplicate( image )
    
    # copy over only the visible layers
    layers = []
    for layer in localImage.layers:
        if pdb.gimp_item_get_visible(layer):
            layers.append( layer )
        
    layerCount = len(layers)
        
    # Increment the interval if image cropping
    interval = 2
    if cropImage:
        interval += 1
        
    # Init local variables for progress bar
    delta = 1.0 / (layerCount * interval)
    percentComplete = 0.0
    
    # Offset List
    offsetList = None
    
    # Crop all the images
    if cropImage:
        offsetList = []
        
        for layer in layers:
            # Save the old size of the offsets and the image 
            offset = layer.offsets
            oldWidth = layer.width
            oldHeight = layer.height
            
            # crop it
            pdb.gimp_image_set_active_layer( localImage, layer )
            pdb.plug_in_autocrop_layer( localImage, layer )
            
            # Get the left side crop
            leftCropX = layer.offsets[0] - offset[0]
            leftCropY = layer.offsets[1] - offset[1]
            
            # Get the new width
            newWidth = ForceEvenOdd( forceEvenOdd, layer.width )
            newHeight = ForceEvenOdd( forceEvenOdd, layer.height )
                    
            # Calc the crop of the right side of the image
            rightCropX = (oldWidth - newWidth - leftCropX)
            rightCropY = (oldHeight - newHeight - leftCropY)
            
            # Get the new center offsets
            offsetX = leftCropX - rightCropX
            offsetY = rightCropY - leftCropY
            
            # save them to be written to file
            offsetList.append( (offsetX / 2, offsetY / 2) )
            
            # Increment the progress bar
            percentComplete += delta
            pdb.gimp_progress_update( percentComplete )

    # Create a list of layers that holds it's original index
    sortLst = []
    counter = 0
    for layer in layers:
        sort = CPair()
        sort.index = counter
        
        # Force odd or even image sizes if enabled
        sort.layerWidth = ForceEvenOdd( forceEvenOdd, layer.width )
        sort.layerHeight = ForceEvenOdd( forceEvenOdd, layer.height )
                
        sort.w = sort.layerWidth + (padding * 2)
        sort.h = sort.layerHeight + (padding * 2)
        sort.layer = layer
        counter += 1
        sortLst.append(sort)
        
    # Sort the list
    sortLst.sort( reverse=True, key=SortByHeight )

    # Create the head node
    head = CNode()
    # Init with the max width and an arbitrary large height to allow for growth
    head.rect = ( 0, 0, maxWidth, 0xFFFFFFF )
    
    finalWidth = 0
    finalHeight = 0
    finalNodeLst = []
    
    for sort in sortLst:
        node = head.Insert( sort )
        
        if node is not None:
            
            finalNodeLst.append( node )
            
            if node.rect[X] + node.rect[W] > finalWidth:
                finalWidth = node.rect[X] + node.rect[W]
                
            if node.rect[Y] + node.rect[H] > finalHeight:
                finalHeight = node.rect[Y] + node.rect[H]
                
            # Increment the progress bar
            percentComplete += delta
            pdb.gimp_progress_update( percentComplete )
            
        else:
            pdb.gimp_message('One of your images is too big to fit within the "Pack Width" specified (%d). Increase the "Pack Width" size or reduce the image that is too big.' % maxWidth)
            return None
    
    # Create the image   
    newImg = gimp.Image(finalWidth, finalHeight, RGB)
    newImg.disable_undo()
    
    newLayer = gimp.Layer(newImg, "sprite sheet", finalWidth, finalHeight, RGBA_IMAGE, 100, NORMAL_MODE)
    newImg.add_layer(newLayer, 0)
    
    newImageWidthDiv2 = finalWidth / 2
    newImageHeightDiv2 = finalHeight / 2
        
    # Sort the final node list by index
    finalNodeLst.sort( reverse=True, key=SortByIndex )

    for node in finalNodeLst:
        
        offset = (0,0)
        if offsetList is not None:
            offset = offsetList[node.index]
            
        # Get the name of the file with the extension stripped out
        nameMinusExt = os.path.splitext(os.path.basename(node.layer.name))[0]
            
        # Add the rect of the image
        rect = (node.rect[X] + padding, node.rect[Y] + padding, node.layerWidth, node.layerHeight, nameMinusExt, offset)
        rectLst.append( rect )
        
        # Copy the image layer to the new layer
        pdb.gimp_edit_copy( node.layer )
        floatingLayer = pdb.gimp_edit_paste( newLayer, False )

        # Calculate the offset to top left (0,0))
        offsetX = -(newImageWidthDiv2 - (floatingLayer.width / 2))
        offsetY = -(newImageHeightDiv2 - (floatingLayer.height / 2))
        
        # Correct any offset issues due to the division of negative numbers
        dif = offsetX + floatingLayer.offsets[0]
        if dif != 0:
            offsetX += dif * -1
            
        dif = offsetY + floatingLayer.offsets[1]
        if dif != 0:
            offsetY += dif * -1
        
        # Position the new layer into it's spot
        pdb.gimp_layer_translate( floatingLayer, offsetX + rect[X], offsetY + rect[Y] )
        pdb.gimp_floating_sel_anchor( floatingLayer )
        
        # Increment the progress bar
        percentComplete += delta
        pdb.gimp_progress_update( percentComplete )
        
    # Delete the duplicate image
    pdb.gimp_image_delete( localImage )
    
    return newImg

    
#************************************************************************
#    desc:  Create an XML of the UV offsets
#************************************************************************
def CreateXML( exportPath, exportFilename, rectLst, width, height ):
    
    # Open the file for writing
    txtFile = open(os.path.join(exportPath, exportFilename), "w")
    
    txtFile.write('<?xml version="1.0"?>\n<spriteSheet width="{}" height="{}">\n\n'.format( width, height ) )
    
    # Write out the pixel and UV offsets
    for rect in rectLst:
        txtFile.write('    <rect x1="{:4}" y1="{:4}" x2="{:4}" y2="{:4}" cx="{:4}" cy="{:4}" name="{}"/>\n'.format(rect[X], rect[Y], rect[W], rect[H], rect[OF][0], rect[OF][1], rect[N]))
    
    txtFile.write("\n</spriteSheet>")
    
    # Flush and close the files
    txtFile.flush()
    txtFile.close()
    

#************************************************************************
#    desc:  Create a JSON of the UV offsets
#************************************************************************
def CreateJSON( exportPath, exportFilename, rectLst, width, height ):
    
    # Open the file for writing
    txtFile = open(os.path.join(exportPath, exportFilename), "w")
    
    # Need the number of elements to determine when the last one is reached
    length = len(rectLst)
    counter = 0
    
    txtFile.write('{\n  "spriteSheet": { ')
    txtFile.write('"width": "{}", "height": "{}",\n    "rect": [\n'.format( width, height ) )
    
    for rect in rectLst:
        txtFile.write('      {\n')
        txtFile.write('        "x1": "{}",\n        "y1": "{}",\n        "x2": "{}",\n        "y2": "{}",\n        "cx": "{}",\n        "cy": "{}",\n        "name": "{}"\n'.format(rect[0], rect[1], rect[2], rect[3], rect[OF][0], rect[OF][1], rect[4]))
        
        # Need to know when the last element is reached so as to not include a comma
        # JSON format really picky
        counter += 1
        if counter == length:
            txtFile.write('      }\n')
        else:
            txtFile.write('      },\n')

    txtFile.write("    ]\n  }\n}")
    
    # Flush and close the files
    txtFile.flush()
    txtFile.close()


#************************************************************************
#    desc:  Register call when plugin is initialized
#************************************************************************
register(
    "sprite-sheets-by-howie",
    N_('Sprite Sheet Creator by Howie (hrosenorn@gmail.com)\n\nFrom the "File" menu choose "New.." and make an image big enough to hold the largest image. Be sure to delete the "Background" layer (or toggle the visiblity) that is automatically created otherwise it will be added to your sprite sheet as a big empty hole. From the "File" menu choose "Open As Layers..." all the image files you want to make a sprite sheet from and then choose a "Simple" or "Complex" sprite sheet.\n\n1) For simple sprite sheets of images of the same size, choose the Simple option and set the desired columns. Segment size based on last layer in Layers Dialog.\n\n2) For complex sprite sheets of images of varing sizes (for efficient packing), choose the Complex option and set the pack width. The height will be generated based on the space needed to fit all images.\n\nTurn off a layers visibility to exclude it from the sprite sheet.\n\nExperiment with the above options to achieve the desired results.'),
    "Create complex sprite sheets simple and easy.",
    "Howard C Rosenorn",
    "Howard C Rosenorn",
    "2016",
    N_("Sprite Sheets by _Howie..."),
    "*",
    [
        (PF_IMAGE, "image",  "Input image", None),
        (PF_RADIO, "simpleSpriteSheet", "Simple or Complex Sprite Sheet", True,
          (("Simple: Grid based on columns and rows of the same size.", True), ("Complex: Grouping of images of different sizes for space efficiency.", False))),
        (PF_INT, "columns", "Number of colums: (Simple only feature)", 1),
        (PF_INT, "maxWidth", "Pack width: (Complex only feature)", 1024),
        (PF_RADIO, "exportFile", "Create an export file? File with x, y, width and height offsets.", "none",
          (("XML", "xml"), ("JSON", "json"), ("None", "none"))),
        (PF_DIRNAME, "exportPath", "Save Export files here.", os.getcwd()),
        (PF_STRING, "exportFilename", "Export Filename",  ""),
        (PF_INT, "padding", "Padding: The amount of transparent pixels around each image.", 0),
        (PF_BOOL,   "cropImage", "Crop Images: (Complex only feature)", False),
        (PF_RADIO, "forceEvenOdd", "Force even or odd image sizes.", "none",
          (("Even", "even"), ("Odd", "odd"), ("None", "none"))),
    ],
    [],
    CreateSpriteSheet,
    menu="<Image>/Filters/Combine",
    domain=("gimp20-python", gimp.locale_directory))

main ()

