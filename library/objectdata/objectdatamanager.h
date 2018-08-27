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

// Forward declaration(s)
class CObjectData2D;
class CObjectData3D;
class CSpriteData;

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
    const CObjectData2D & getData2D( const std::string & group, const std::string & name ) const;
    const CObjectData2D & getData2D( const CSpriteData & spriteData ) const;
    const CObjectData3D & getData3D( const std::string & group, const std::string & name ) const;

    // Load all of the meshes and materials of a specific data group
    void loadGroup2D( const std::string & group );
    void loadGroup3D( const std::string & group );

    // Free all of the meshes and materials of a specific data group
    void freeGroup2D( const std::string & group );
    void freeGroup3D( const std::string & group );
    
    // Is data part of 2d/3d
    bool isData2D( const std::string & group, const std::string & name ) const;
    bool isData3D( const std::string & group, const std::string & name ) const;

private:

    CObjectDataMgr();
    virtual ~CObjectDataMgr();

    // Load all object information from an xml
    void load2D( const std::string & group, const std::string & filePath );
    void load3D( const std::string & group, const std::string & filePath );

private:

    // Map in a map of all the objects' data
    std::map<const std::string, std::map<const std::string, CObjectData2D> > m_objectData2DMapMap;
    std::map<const std::string, std::map<const std::string, CObjectData3D> > m_objectData3DMapMap;

};

#endif  // __object_data_list_2d_h__
