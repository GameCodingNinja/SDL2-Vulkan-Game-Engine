
/************************************************************************
 *    FILE NAME:       meshmanager.cpp
 *
 *    DESCRIPTION:     mesh manager class singleton
 ************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <managers/meshmanager.h>

// Game lib dependencies
#include <utilities/smartpointers.h>
#include <utilities/exceptionhandling.h>
#include <managers/texturemanager.h>
#include <common/meshbinaryfileheader.h>
#include <common/point.h>
#include <common/normal.h>
#include <common/uv.h>
#include <common/texture.h>
#include <common/vertex3d.h>

// SDL lib dependencies
#include <SDL.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <algorithm>
#include <memory>

/************************************************************************
 *    DESC:  Constructor
 ************************************************************************/
CMeshMgr::CMeshMgr()
{
}


/************************************************************************
 *    DESC:  destructor
 ************************************************************************/
CMeshMgr::~CMeshMgr()
{
    // Free the buffers in all groups
    for( auto & mapMapIter : m_meshBufMapMap )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            for( auto & iter : mapIter.second.getMeshVec() )
            {
                glDeleteBuffers( 1, &iter.m_ibo );
                glDeleteBuffers( 1, &iter.m_vbo );
            }
        }
    }
}


/************************************************************************
 *    DESC: Load mesh from file
 ************************************************************************/
void CMeshMgr::loadFromFile(
    const std::string & group,
    const std::string & filePath )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_meshBufMapMap.find( group );
    if( mapMapIter == m_meshBufMapMap.end() )
        mapMapIter = m_meshBufMapMap.emplace( group, std::map<const std::string, CMesh3D >() ).first;

    // See if the ID has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, create the mesh buffer and add it to the list
    if( mapIter == mapMapIter->second.end() )
    {
        mapIter = mapMapIter->second.emplace( filePath, CMesh3D() ).first;

        loadFrom3DM( group, filePath, mapIter->second );
    }
}


/************************************************************************
 *    DESC: Load collision mesh from file
 ************************************************************************/
void CMeshMgr::loadFromFile(
    const std::string & group,
    const std::string & filePath,
    CCollisionMesh3D & collisionMesh )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_collisionMeshBufMapMap.find( group );
    if( mapMapIter == m_collisionMeshBufMapMap.end() )
        mapMapIter = m_collisionMeshBufMapMap.emplace( group, std::map<const std::string, CCollisionMesh3D>() ).first;

    // See if the ID has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, create the collision mesh buffer and add it to the list
    if( mapIter == mapMapIter->second.end() )
    {
        mapIter = mapMapIter->second.emplace( filePath, CCollisionMesh3D() ).first;

        loadFrom3DM( filePath, mapIter->second );
    }

    // Copy the mesh data over
    collisionMesh = mapIter->second;
}


/************************************************************************
 *    DESC: Load 3d mesh file
 ************************************************************************/
void CMeshMgr::loadFrom3DM(
    const std::string & group,
    const std::string & filePath,
    CMesh3D & mesh3d )
{
    // Open file for reading
    NSmart::scoped_SDL_filehandle_ptr<SDL_RWops> scpFile( SDL_RWFromFile( filePath.c_str(), "rb" ) );
    if( scpFile.isNull() )
        throw NExcept::CCriticalException( "File Load Error!",
            boost::str( boost::format( "Error Loading file (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );

    // Read in the file header
    CMeshBinaryFileHeader fileHeader;
    SDL_RWread( scpFile.get(), &fileHeader, 1, sizeof( fileHeader ) );

    // Check to make sure we're loading in the right kind of file
    if( fileHeader.file_header != MESH_FILE_HEADER )
        throw NExcept::CCriticalException( "Visual Mesh Load Error!",
            boost::str( boost::format( "File header mismatch (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );

    if( fileHeader.text_count > 0 )
        // Load with textures
        loadFromFile( scpFile.get(), fileHeader, group, filePath, mesh3d );
    else
        // Load without textures
        loadFromFile( scpFile.get(), fileHeader, filePath, mesh3d );
}


/************************************************************************
 *    DESC: Load 3d mesh file with textures
 ************************************************************************/
void CMeshMgr::loadFromFile(
    SDL_RWops * pFile,
    const CMeshBinaryFileHeader & fileHeader,
    const std::string & group,
    const std::string & filePath,
    CMesh3D & mesh3d )
{
    // Check to insure we are in the correct spot in the binary file
    tagCheck( pFile, filePath );

    // Setup the texture file path vector
    auto & textureVec = mesh3d.getTextureVec();
    textureVec.reserve( fileHeader.text_count);

    // Load the textures into a temporary vector
    for( int i = 0; i < fileHeader.text_count; ++i )
    {
        // Get the texture and it's type
        CBinaryTexture btext;
        SDL_RWread( pFile, &btext, 1, sizeof( btext ) );

        // Load the texture
        CTextureMgr::Instance().loadImageFor3D( group, btext.path );

        // Record texture info for later
        textureVec.emplace_back();
        textureVec.back().m_textFilePath = btext.path;
        textureVec.back().m_type = ETextureType(btext.type);
    }

    // Allocate smart pointer arrays and load the unique verts, normals and UV's

    // Load in the verts
    tagCheck( pFile, filePath );
    std::unique_ptr < CPoint<float>[] > upVert( new CPoint<float>[fileHeader.vert_count] );
    SDL_RWread( pFile, upVert.get(), fileHeader.vert_count, sizeof( CPoint<float> ) );

    // Load in the normals
    tagCheck( pFile, filePath );
    std::unique_ptr < CNormal<float>[] > upVNormal( new CNormal<float>[fileHeader.vert_norm_count] );
    SDL_RWread( pFile, upVNormal.get(), fileHeader.vert_norm_count, sizeof( CNormal<float> ) );

    // Load in the UV
    tagCheck( pFile, filePath );
    std::unique_ptr < CUV[] > upUV( new CUV[fileHeader.uv_count] );
    SDL_RWread( pFile, upUV.get(), fileHeader.uv_count, sizeof( CUV ) );

    // Reserve the number of vbo groups
    mesh3d.reserve( fileHeader.face_group_count );

    // Read in each face group
    for( int i = 0; i < fileHeader.face_group_count; ++i )
    {
        // Add a new mesh entry into the vector
        mesh3d.emplace_back();
        auto & mesh = mesh3d.back();

        // Check to insure we are in the correct spot in the binary file
        tagCheck( pFile, filePath );

        // Get the number faces in the group as well as the material index
        SDL_RWread( pFile, &mesh.m_faceGroup, 1, sizeof( CBinaryFaceGroup ) );

        // Allocate a temporary buffers for building the VBO
        mesh.allocateBuffers();
        std::unique_ptr < CBinaryVertex[] > upVertBuf( new CBinaryVertex[mesh.m_faceGroup.vertexBufCount] );

        // Read in the indexes that are the textures
        SDL_RWread( pFile, mesh.m_spTextIndexBuf.get(), mesh.m_faceGroup.textureCount, sizeof( uint16_t ) );

        // Read in the indexes used to create the VBO
        SDL_RWread( pFile, upVertBuf.get(), mesh.m_faceGroup.vertexBufCount, sizeof( CBinaryVertex ) );

        // Read in the indexes that are the IBO
        SDL_RWread( pFile, mesh.m_spIndexBuf.get(), mesh.m_faceGroup.indexBufCount, sizeof( uint16_t ) );

        // Build the VBO
        for( int j = 0; j < mesh.m_faceGroup.vertexBufCount; ++j )
        {
            mesh.m_spVBO.get()[j].vert = upVert[ upVertBuf[j].vert ];
            mesh.m_spVBO.get()[j].norm = upVNormal[ upVertBuf[j].norm ];
            mesh.m_spVBO.get()[j].uv = upUV[ upVertBuf[j].uv ];
        }

        // Save the number of indexes in the IBO buffer - Will need this for the render call
        mesh.m_iboCount = mesh.m_faceGroup.indexBufCount;
    }
}


/************************************************************************
 *    DESC: Create from data
 ************************************************************************/
void CMeshMgr::createFromData(
    const std::string & group,
    const std::string & filePath,
    CMesh3D & mesh3d )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_meshBufMapMap.find( group );
    if( mapMapIter == m_meshBufMapMap.end() )
        mapMapIter = m_meshBufMapMap.emplace( group, std::map<const std::string, CMesh3D >() ).first;

    // See if the ID has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, there's a problem
    if( mapIter == mapMapIter->second.end() )
        throw NExcept::CCriticalException("Create Mesh Error!",
                boost::str( boost::format("Error creating mesh (%s)(%s).\n\n%s\nLine: %s")
                    % filePath % group % __FUNCTION__ % __LINE__ ));

    // See if this mesh needs to be created
    if( mapIter->second.allowCreate() )
    {
        createFromData( group, mapIter->second );

        // Copy the mesh data to the passed in mesh vector
        mesh3d.reserve( mapIter->second.size() );
        for( auto & iter : mapIter->second.getMeshVec() )
            mesh3d.emplace_back( iter );
    }
}


/************************************************************************
 *    DESC: Create from the data
 ************************************************************************/
void CMeshMgr::createFromData(
    const std::string & group,
    CMesh3D & mesh3d )
{
    // Get the texture vector
    auto & textureVec = mesh3d.getTextureVec();

    // Create the textures from data
    for( auto & iter : textureVec )
    {
        // Load the texture
        CTexture texture = CTextureMgr::Instance().createTextureFor3D( group, iter.m_textFilePath );

        iter.m_id = texture.m_id;
        iter.m_size = texture.m_size;
        iter.m_channels = texture.m_channels;
    }

    // Get the mesh vector
    auto & meshVec = mesh3d.getMeshVec();

    // Read in each face group
    for( auto & iter : meshVec )
    {
        // Create the VBO
        glGenBuffers( 1, &iter.m_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, iter.m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CVertex3D)*iter.m_faceGroup.vertexBufCount, iter.m_spVBO.get(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        // Create the IBO - It's saved in the binary file as needed. Don't need to build it.
        glGenBuffers( 1, &iter.m_ibo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iter.m_ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * iter.m_faceGroup.indexBufCount, iter.m_spIndexBuf.get(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        // Reserve texture space
        iter.m_textureVec.reserve(iter.m_faceGroup.textureCount);

        // Copy over the texture data
        for( int j = 0; j < iter.m_faceGroup.textureCount; ++j )
        {
            int textIndex = iter.m_spTextIndexBuf.get()[j];
            iter.m_textureVec.emplace_back( textureVec.at(textIndex) );
        }
    }

    // Clear out the allocated buffers
    mesh3d.clearBuffers();
}


/************************************************************************
 *    DESC: Load 3d mesh file without textures
 ************************************************************************/
void CMeshMgr::loadFromFile(
    SDL_RWops * pFile,
    const CMeshBinaryFileHeader & fileHeader,
    const std::string & filePath,
    CMesh3D & mesh3d )
{
    // Allocate smart pointer arrays and load the unique verts and normals

    // Load in the verts
    tagCheck( pFile, filePath );
    std::unique_ptr < CPoint<float>[] > upVert( new CPoint<float>[fileHeader.vert_count] );
    SDL_RWread( pFile, upVert.get(), fileHeader.vert_count, sizeof( CPoint<float> ) );

    // Load in the normals
    tagCheck( pFile, filePath );
    std::unique_ptr < CNormal<float>[] > upVNormal( new CNormal<float>[fileHeader.vert_norm_count] );
    SDL_RWread( pFile, upVNormal.get(), fileHeader.vert_norm_count, sizeof( CNormal<float> ) );

    // Reserve the number of vbo groups
    mesh3d.reserve( fileHeader.face_group_count );

    // Read in each face group
    for( int i = 0; i < fileHeader.face_group_count; ++i )
    {
        // Add a new mesh entry into the vector
        mesh3d.emplace_back();
        auto & mesh = mesh3d.back();

        // Check to insure we are in the correct spot in the binary file
        tagCheck( pFile, filePath );

        // Get the number faces in the group as well as the material index
        SDL_RWread( pFile, &mesh.m_faceGroup, 1, sizeof( CBinaryFaceGroup ) );

        // Allocate a temporary buffers for building the VBO
        mesh.allocateBuffers();

        std::unique_ptr<CBinaryVertexNoTxt[]> upVertBuf( new CBinaryVertexNoTxt[mesh.m_faceGroup.vertexBufCount] );

        // Read in the indexes used to create the VBO
        SDL_RWread( pFile, upVertBuf.get(), mesh.m_faceGroup.vertexBufCount, sizeof( CBinaryVertexNoTxt ) );

        // Read in the indexes that are the IBO
        SDL_RWread( pFile, mesh.m_spIndexBuf.get(), mesh.m_faceGroup.indexBufCount, sizeof( uint16_t ) );

        // Build the VBO
        for( int j = 0; j < mesh.m_faceGroup.vertexBufCount; ++j )
        {
            mesh.m_spVBONoTxt.get()[j].vert = upVert[ upVertBuf[j].vert ];
            mesh.m_spVBONoTxt.get()[j].norm = upVNormal[ upVertBuf[j].norm ];
        }

        // Create the VBO
        glGenBuffers( 1, &mesh3d.back().m_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, mesh3d.back().m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CVertex3D_no_txt)*mesh.m_faceGroup.vertexBufCount, mesh.m_spVBONoTxt.get(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        // Create the IBO - It's saved in the binary file as needed. Don't need to build it.
        glGenBuffers( 1, &mesh3d.back().m_ibo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh3d.back().m_ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * mesh.m_faceGroup.indexBufCount, mesh.m_spIndexBuf.get(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        // Save the number of indexes in the IBO buffer - Will need this for the render call
        mesh3d.back().m_iboCount = mesh.m_faceGroup.indexBufCount;;
    }
}


/************************************************************************
 *    DESC: Load 3d collision mesh file
 ************************************************************************/
void CMeshMgr::loadFrom3DM(
    const std::string & filePath,
    CCollisionMesh3D & collisionMesh )
{
    // Open file for reading
    NSmart::scoped_SDL_filehandle_ptr<SDL_RWops> scpFile( SDL_RWFromFile( filePath.c_str(), "rb" ) );
    if( scpFile.isNull() )
        throw NExcept::CCriticalException( "File Load Error!",
            boost::str( boost::format( "Error Loading file (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );

    // Read in the file header
    CMeshBinaryFileHeader fileHeader;
    SDL_RWread( scpFile.get(), &fileHeader, 1, sizeof( fileHeader ) );

    // Check to make sure we're loading in the right kind of file
    if( fileHeader.file_header != MESH_FILE_HEADER )
        throw NExcept::CCriticalException( "Visual Mesh Load Error!",
            boost::str( boost::format( "File header mismatch (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );


    // Allocate local smart pointer arrays and load the unique verts and normals

    // Load in the verts
    std::unique_ptr<CPoint<float>[]> upVert( new CPoint<float>[fileHeader.vert_count] );
    tagCheck( scpFile.get(), filePath );
    SDL_RWread( scpFile.get(), upVert.get(), fileHeader.vert_count, sizeof( CPoint<float> ) );


    // Check to insure we are in the correct spot in the binary file
    tagCheck( scpFile.get(), filePath );

    // Get the number faces in the group as well as the material index
    CBinaryFaceGroup faceGroup;
    SDL_RWread( scpFile.get(), &faceGroup, 1, sizeof( faceGroup ) );

    // Allocate local smart pointer arrays
    std::unique_ptr<uint16_t[]> upVertBuf( new uint16_t[faceGroup.vertexBufCount] );
    collisionMesh.ibo.reset( new uint16_t[faceGroup.indexBufCount], std::default_delete<uint16_t[]>() );

    // Read in the indexes used to create the VBO
    SDL_RWread( scpFile.get(), upVertBuf.get(), faceGroup.vertexBufCount, sizeof( uint16_t ) );

    // Read in the indexes that are the IBO
    SDL_RWread( scpFile.get(), collisionMesh.ibo.get(), faceGroup.indexBufCount, sizeof( uint16_t ) );

    // Allocate buffer for building the VBO
    collisionMesh.vbo.reset( new CPoint<float>[faceGroup.vertexBufCount], std::default_delete<CPoint<float>[]>() );

    // Build the VBO
    for( int i = 0; i < faceGroup.vertexBufCount; ++i )
        collisionMesh.vbo.get()[i] = upVert[ upVertBuf[i] ];

    // Save the number of indexes in the IBO buffer - Will need this for the render call
    collisionMesh.m_iboCount = faceGroup.indexBufCount;
}


/************************************************************************
 *    DESC: Do the tag check to insure we are in the correct spot
 ************************************************************************/
void CMeshMgr::tagCheck( SDL_RWops * file, const std::string & filePath )
{
    int tag_check;

    // Read in new tag
    SDL_RWread( file, &tag_check, 1, sizeof( tag_check ) );

    if( tag_check != TAG_CHECK )
        throw NExcept::CCriticalException( "Visual Mesh Load Error!",
            boost::str( boost::format( "Tag check mismatch (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );
}


/************************************************************************
 *    DESC:  Delete mesh buffer group
 ************************************************************************/
void CMeshMgr::deleteBufferGroup( const std::string & group )
{
    auto mapMapIter = m_meshBufMapMap.find( group );
    if( mapMapIter != m_meshBufMapMap.end() )
    {
        // Delete all the buffers in this group
        for( auto & mapIter : mapMapIter->second )
        {
            for( auto & iter : mapIter.second.getMeshVec() )
            {
                glDeleteBuffers( 1, &iter.m_ibo );
                glDeleteBuffers( 1, &iter.m_vbo );
            }
        }

        // Erase this group
        m_meshBufMapMap.erase( mapMapIter );
    }
}
