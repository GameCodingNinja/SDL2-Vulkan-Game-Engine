
/************************************************************************
 *    FILE NAME:       meshmanager.h
 *
 *    DESCRIPTION:     mesh manager class singleton
 ************************************************************************/

#ifndef __mesh_manager_h__
#define __mesh_manager_h__

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

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
    void LoadFromFile(
        const std::string & group,
        const std::string & filePath,
        std::vector<CMesh3D> & meshVec );

    // Load collision mesh from file
    void LoadFromFile(
        const std::string & group,
        const std::string & filePath,
        CCollisionMesh3D & collisionMesh );

    // Delete mesh buffer group
    void DeleteBufferGroup(const std::string & group);

private:

    // Constructor
    CMeshMgr();

    // Destructor
    ~CMeshMgr();

    // Load 3d mesh file
    void LoadFrom3DM(
        const std::string & group,
        const std::string & filePath,
        std::vector<CMesh3D> & meshVec );
    
    // Load 3d collision mesh file
    void LoadFrom3DM(
        const std::string & filePath,
        CCollisionMesh3D & collisionMesh );

    // Load 3d mesh file with textures
    void LoadFromFile(
        SDL_RWops * pFile,
        const CMeshBinaryFileHeader & fileHeader,
        const std::string & group,
        const std::string & filePath,
        std::vector<CMesh3D> & meshVec );

    // Load 3d mesh file without textures
    void LoadFromFile(
        SDL_RWops * pFile,
        const CMeshBinaryFileHeader & fileHeader,
        const std::string & filePath,
        std::vector<CMesh3D> & meshVec );

    // Do the tag check to insure we are in the correct spot
    void TagCheck( SDL_RWops * file, const std::string & filePath );

private:

    // Map containing a group array of vbo, ibo and texture id's
    std::map< const std::string, std::map< const std::string, std::vector<CMesh3D> > > m_meshBufMapMapVec;

    // Map for collision mesh
    std::map< const std::string, std::map< const std::string, CCollisionMesh3D > > m_collisionMeshBufMapMap;

    // Current vbo ID
    GLuint m_currentVBOID;

    // Current IBO ID
    GLuint m_currentIBOID;

};

#endif  // __mesh_manager_h__


