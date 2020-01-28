#        slot/symbol2d.cpp
#        slot/symbolsetview.cpp
#        slot/symbolsetviewmanager.cpp
#        slot/slotmathmanager.cpp
#        slot/slotmath.cpp
#        slot/mathsymbol.cpp
#        slot/paycombo.cpp
#        slot/valuetable.cpp
#        slot/weightedtable.cpp
#        slot/slotgroup.cpp
#        slot/paylineset.cpp
#        slot/slotgroupmodel.cpp
#        slot/slotstripmodel.cpp
#        slot/slotgroupview.cpp
#        slot/reelgroupview.cpp
#        slot/reelstripview.cpp
#        slot/spinprofile.cpp
#        slot/playresult.cpp
#        slot/slotresults.cpp
#        slot/basegamemusic.cpp
#        slot/pay.cpp
#        slot/slotgame.cpp
#        slot/paytableset.cpp
#        slot/stripset.cpp
#        slot/stripstop.cpp
#        slot/betmanager.cpp
#        slot/wheelgroupview.cpp
#        slot/wheelview.cpp
#        slot/simplecycleresults.cpp
#        slot/icycleresults.cpp
#        slot/slotgroupfactory.cpp
#        slot/animatedcycleresults.cpp


# Add the files to the library
add_library(
    library STATIC
        gui/icontrol.cpp
        gui/uibutton.cpp
        gui/menumanager.cpp
        gui/menutree.cpp
        gui/uicontrol.cpp
        gui/uicontrolfactory.cpp
        gui/menu.cpp
        gui/uilabel.cpp
        gui/uisubcontrol.cpp
        gui/uibuttonlist.cpp
        gui/uicheckbox.cpp
        gui/uislider.cpp
        gui/uiscrollbox.cpp
        gui/uimeter.cpp
        gui/uiprogressbar.cpp
        gui/scrollparam.cpp
        objectdata/objectdata2d.cpp
        objectdata/objectdata3d.cpp
        objectdata/objectdatamanager.cpp
        objectdata/objectphysicsdata2d.cpp
        objectdata/objectphysicsdata3d.cpp
        objectdata/objectvisualdata2d.cpp
        objectdata/objectvisualdata3d.cpp
        objectdata/iobjectdata.cpp
        objectdata/iobjectvisualdata.cpp
        objectdata/iobjectphysicsdata.cpp
        script/scriptmanager.cpp
        script/scriptglobals.cpp
        script/scriptcolor.cpp
        script/scriptcamera.cpp
        script/scriptcameramanager.cpp
        script/scriptcomponent.cpp
        script/scriptpoint.cpp
        script/scriptsprite.cpp
        script/scriptsound.cpp
        script/scriptobjectdatamanager.cpp
        script/scriptactionmanager.cpp
        script/scriptsettings.cpp
        script/scriptsize.cpp
        script/scripthighresolutiontimer.cpp
        script/scriptfontmanager.cpp
        script/scriptstrategy.cpp
        script/scriptmenumanager.cpp
        script/scriptscriptmanager.cpp
        script/scriptmenu.cpp
        script/scriptuicontrol.cpp
        script/scriptvisual.cpp
        script/scriptdevice.cpp
        script/scriptphysics2d.cpp
        script/scriptstatcounter.cpp
        system/devicevulkan.cpp
        system/device.cpp
        system/uniformbufferobject.cpp
        system/pushdescriptorset.cpp
        utilities/xmlparsehelper.cpp
        utilities/statcounter.cpp
        utilities/genfunc.cpp
        utilities/settings.cpp
        utilities/highresolutiontimer.cpp
        utilities/timer.cpp
        utilities/xmlParser.cpp
        utilities/mathfunc.cpp
        utilities/threadpool.cpp
        utilities/xmlpreloader.cpp
        utilities/matrix.cpp
        utilities/exceptionhandling.cpp
        managers/soundmanager.cpp
        managers/managerbase.cpp
        managers/fontmanager.cpp
        managers/actionmanager.cpp
        managers/spritesheetmanager.cpp
        managers/cameramanager.cpp
        physics/physicsworldmanager2d.cpp
        physics/physicsworldmanager3d.cpp
        physics/physicsworld2d.cpp
        physics/physicsworld3d.cpp
        physics/physicscomponent2d.cpp
        physics/physicscomponent3d.cpp
        sprite/sprite.cpp
        sprite/spritesheet.cpp
        sprite/spritedata.cpp
        node/node.cpp
        node/spritenodemultilist.cpp
        node/objectnodemultilist.cpp
        node/nodemultilist.cpp
        node/spritenode.cpp
        node/nodedata.cpp
        node/nodedatalist.cpp
        node/nodefactory.cpp
        node/uicontrolnode.cpp
        2d/font.cpp
        2d/visualcomponentquad.cpp
        2d/visualcomponentspritesheet.cpp
        2d/visualcomponentscaledframe.cpp
        2d/visualcomponentfont.cpp
        2d/visualcomponentnull.cpp
        2d/object2d.cpp
        3d/object3d.cpp
        3d/light.cpp
        3d/lightlist.cpp
        3d/visualcomponent3d.cpp
        strategy/sector.cpp
        strategy/actorstrategy.cpp
        strategy/stagestrategy.cpp
        strategy/strategymanager.cpp
        strategy/istrategy.cpp
        strategy/strategyloader.cpp
        common/sound.cpp
        common/playlist.cpp
        common/worldvalue.cpp
        common/camera.cpp
        common/object.cpp
        common/fontdata.cpp
        common/fontproperties.cpp
        common/ivisualcomponent.cpp
        common/visual.cpp
        common/vertex.cpp
        common/dynamicoffset.cpp
        soil/stb_image_aug.c
        soil/SOIL.c
        soil/image_helper.c
        soil/image_DXT.c )

include_directories(
    objectdata
    script
    system
    sprite
    node
    strategy
    utilities
    managers
    physics
    2d
    3d
    gui
    slot
    common
    soil )

