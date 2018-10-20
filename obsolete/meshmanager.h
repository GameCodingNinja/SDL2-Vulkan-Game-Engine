
/************************************************************************
 *    FILE NAME:       meshmanager.h
 *
 *    DESCRIPTION:     mesh manager class singleton
 ************************************************************************/

#ifndef __mesh_manager_h__
#define __mesh_manager_h__

// Game lib dependencies
#include <common/mesh3d.h>
#include <common/collisionmesh3d.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
struct SDL_RWops;
class CMeshBinaryFileHeader;

class CMeshMgr
{
public:

    // Get the instance of the singleton class
    static CMeshMgr & Instance()
    {
        static CMeshMgr meshMgr;
        return meshMgr;
    }

    // Load mesh from file
    void loadFromFile(
        const std::string & group,
        const std::string & filePath );

    // Load collision mesh from file
    void loadFromFile(
        const std::string & group,
        const std::string & filePath,
        CCollisionMesh3D & collisionMesh );
    
    void createFromData(
        const std::string & group,
        const std::string & filePath,
        CMesh3D & mesh3d );
    
    void createFromData(
        const std::string & group,
        CMesh3D & mesh3d );
    
    // Delete mesh buffer group
    void deleteBufferGroup(const std::string & group);

private:

    // Constructor
    CMeshMgr();

    // Destructor
    ~CMeshMgr();

    // Load 3d mesh file
    void loadFrom3DM(
        const std::string & group,
        const std::string & filePath,
        CMesh3D & mesh3d );
    
    // Load 3d collision mesh file
    void loadFrom3DM(
        const std::string & filePath,
        CCollisionMesh3D & collisionMesh );

    // Load 3d mesh file with textures
    void loadFromFile(
        SDL_RWops * pFile,
        const CMeshBinaryFileHeader & fileHeader,
        const std::string & group,
        const std::string & filePath,
        CMesh3D & mesh3d );

    // Load 3d mesh file without textures
    void loadFromFile(
        SDL_RWops * pFile,
        const CMeshBinaryFileHeader & fileHeader,
        const std::string & filePath,
        CMesh3D & mesh3d );

    // Do the tag check to insure we are in the correct spot
    void tagCheck( SDL_RWops * file, const std::string & filePath );

private:

    // Map containing a group array of vbo, ibo and texture id's
    std::map< const std::string, std::map< const std::string, CMesh3D > > m_meshBufMapMap;

    // Map for collision mesh
    std::map< const std::string, std::map< const std::string, CCollisionMesh3D > > m_collisionMeshBufMapMap;

    // Current vbo ID
    uint32_t m_currentVBOID;

    // Current IBO ID
    uint32_t m_currentIBOID;

};

#endif  // __mesh_manager_h__


