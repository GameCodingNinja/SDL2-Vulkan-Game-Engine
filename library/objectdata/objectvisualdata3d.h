
/************************************************************************
*    FILE NAME:       ojectvisualdata3d.h
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

#ifndef __object_visual_data_3d_h__
#define __object_visual_data_3d_h__

// Physical component dependency
#include <objectdata/iobjectvisualdata.h>

// Game lib dependencies
#include <common/size.h>
#include <common/color.h>
#include <common/rect.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/scaledframe.h>
#include <common/texture.h>
#include <common/model.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward Declarations
struct XMLNode;

class CObjectVisualData3D : public iObjectVisualData
{
public:

    // Constructor/Destructor
    CObjectVisualData3D();
    virtual ~CObjectVisualData3D();

    // Load thes object data from node
    void loadFromNode( const XMLNode & objectNode );
    
    // Load the mesh data from file
    void loadMeshData( const std::string & group );
    
    // Create the object from data
    void createFromData( const std::string & group );

    // Get the pipeline ID
    const std::string & getPipelineID() const;

    // Get the color
    const CColor & getColor() const override;

    // Whether or not the visual tag was specified
    bool isActive() const override;

    // Get the mesh3d vector
    const CModel & getModel() const override;
    
private:

    CModel m_model;

    // Name of the pipeline id
    std::string m_pipelineID;

    // Initial color of the object
    CColor m_color;

    // model file path
    std::string m_modelFile;
};

#endif
