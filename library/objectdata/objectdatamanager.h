/************************************************************************
*    FILE NAME:       objectdatamanager.h
*
*    DESCRIPTION:     Singlton that holds a map of all 2D/3D object data used 
*					  for later loading
************************************************************************/

#ifndef __object_data_manager_h__
#define __object_data_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CObjectData2D;
class CObjectData3D;
class CSpriteData;
class iObjectData;
struct XMLNode;

class CObjectDataMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CObjectDataMgr & Instance()
    {
        static CObjectDataMgr objectDataMgr;
        return objectDataMgr;
    }
    
    // Get a specific object's data
    const iObjectData & getData( const std::string & group, const std::string & name ) const;

    // Load all of the meshes and materials of a specific data group
    void loadGroup( const std::string & group );

    // Free all of the meshes and materials of a specific data group
    void freeGroup( const std::string & group );
    
    // Free only the data of a specific group
    void freeDataGroup( const std::string & group );

private:

    CObjectDataMgr();
    virtual ~CObjectDataMgr();

    // Load all object information from an xml
    void load( const std::string & group, const std::string & filePath );
    void load2D( const std::string & group, const XMLNode & mainNode );
    void load3D( const std::string & group, const XMLNode & mainNode );

    // Create the group's VBO, IBO, textures, etc
    void createFromData( const std::string & group );

private:
    
    // Map in a map of all the objects' data
    std::map<const std::string, std::map<const std::string, std::unique_ptr<iObjectData>> > m_objectDataMapMap;
};

#endif
