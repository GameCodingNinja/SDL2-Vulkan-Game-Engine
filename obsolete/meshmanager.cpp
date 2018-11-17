
/************************************************************************
 *    FILE NAME:       meshmanager.cpp
 *
 *    DESCRIPTION:     mesh manager class singleton
 ************************************************************************/

#if !(defined(__IOS__) || defined(__ANDROID__) || defined(__arm__))
// Glew dependencies (have to be defined first)
#include <GL/glew.h>
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
 *    desc:  Constructor
 ************************************************************************/
CMeshMgr::CMeshMgr()
{
}   // constructor


/************************************************************************
 *    desc:  destructor
 ************************************************************************/
CMeshMgr::~CMeshMgr()
{
    // Free the buffers in all groups
    for( auto & mapMapIter : m_meshBufMapMapVec )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            for( auto & iter : mapIter.second )
            {
                glDeleteBuffers( 1, &iter.m_ibo );
                glDeleteBuffers( 1, &iter.m_vbo );
            }
        }
    }

}	// destructor


/************************************************************************
 *    desc: Load mesh from file
 ************************************************************************/
void CMeshMgr::LoadFromFile(
    const std::string & group,
    const std::string & filePath,
    std::vector<CMesh3D> & meshVec )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_meshBufMapMapVec.find( group );
    if( mapMapIter == m_meshBufMapMapVec.end() )
        mapMapIter = m_meshBufMapMapVec.emplace( group, std::map<const std::string, std::vector < CMesh3D >>() ).first;

    // See if the ID has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, create the mesh buffer and add it to the list
    if( mapIter == mapMapIter->second.end() )
    {
        mapIter = mapMapIter->second.emplace( filePath, std::vector<CMesh3D>() ).first;

        LoadFrom3DM( group, filePath, mapIter->second );
    }

    // Copy the mesh data to the passed in mesh vector
    meshVec.reserve( mapIter->second.size() );
    for( auto & iter : mapIter->second )
        meshVec.emplace_back( iter );

}   // LoadFromFile


/************************************************************************
 *    desc: Load collision mesh from file
 ************************************************************************/
void CMeshMgr::LoadFromFile(
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

        LoadFrom3DM( filePath, mapIter->second );
    }

    // Copy the mesh data over
    collisionMesh = mapIter->second;

}   // LoadFromFile


/************************************************************************
 *    desc: Load 3d mesh file
 ************************************************************************/
void CMeshMgr::LoadFrom3DM(
    const std::string & group,
    const std::string & filePath,
    std::vector<CMesh3D> & meshVec )
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
        LoadFromFile( scpFile.get(), fileHeader, group, filePath, meshVec );
    else
        // Load without textures
        LoadFromFile( scpFile.get(), fileHeader, filePath, meshVec );

}   // LoadFrom3DM


/************************************************************************
 *    desc: Load 3d mesh file with textures
 ************************************************************************/
void CMeshMgr::LoadFromFile(
    SDL_RWops * pFile,
    const CMeshBinaryFileHeader & fileHeader,
    const std::string & group,
    const std::string & filePath,
    std::vector<CMesh3D> & meshVec )
{
    // temporary texture vector
    std::vector<CTexture> textureVec;
    textureVec.reserve( fileHeader.text_count );

    // Check to insure we are in the correct spot in the binary file
    TagCheck( pFile, filePath );

    // Load the textures into a temporary vector
    for( int i = 0; i < fileHeader.text_count; ++i )
    {
        // Get the texture and it's type
        CBinaryTexture btext;
        SDL_RWread( pFile, &btext, 1, sizeof( btext ) );

        // Load the texture
        CTexture texture = CTextureMgr::Instance().LoadFor3D( group, btext.path );
        texture.m_type = ETextureType(btext.type);

        textureVec.emplace_back( texture );
    }

    // Allocate local smart pointer arrays and load the unique verts, normals and UV's

    // Load in the verts
    std::unique_ptr < CPoint<float>[] > upVert( new CPoint<float>[fileHeader.vert_count] );
    TagCheck( pFile, filePath );
    SDL_RWread( pFile, upVert.get(), fileHeader.vert_count, sizeof( CPoint<float> ) );

    // Load in the normals
    std::unique_ptr < CNormal<float>[] > upVNormal( new CNormal<float>[fileHeader.vert_norm_count] );
    TagCheck( pFile, filePath );
    SDL_RWread( pFile, upVNormal.get(), fileHeader.vert_norm_count, sizeof( CNormal<float> ) );

    // Check to insure we are in the correct spot in the binary file
    std::unique_ptr < CUV[] > upUV( new CUV[fileHeader.uv_count] );
    TagCheck( pFile, filePath );
    SDL_RWread( pFile, upUV.get(), fileHeader.uv_count, sizeof( CUV ) );

    // Reserve the number of vbo groups
    meshVec.reserve( fileHeader.face_group_count );

    // Read in each face group
    for( int i = 0; i < fileHeader.face_group_count; ++i )
    {
        // Check to insure we are in the correct spot in the binary file
        TagCheck( pFile, filePath );

        // Get the number faces in the group as well as the material index
        CBinaryFaceGroup faceGroup;
        SDL_RWread( pFile, &faceGroup, 1, sizeof( faceGroup ) );

        // Allocate local smart pointer arrays
        std::unique_ptr < uint16_t[] > upTextIndexBuf( new uint16_t[faceGroup.textureCount] );
        std::unique_ptr < CBinaryVertex[] > upVertBuf( new CBinaryVertex[faceGroup.vertexBufCount] );
        std::unique_ptr < uint16_t[] > upIndexBuf( new uint16_t[faceGroup.indexBufCount] );

        // Read in the indexes that are the textures
        SDL_RWread( pFile, upTextIndexBuf.get(), faceGroup.textureCount, sizeof( uint16_t ) );

        // Read in the indexes used to create the VBO
        SDL_RWread( pFile, upVertBuf.get(), faceGroup.vertexBufCount, sizeof( CBinaryVertex ) );

        // Read in the indexes that are the IBO
        SDL_RWread( pFile, upIndexBuf.get(), faceGroup.indexBufCount, sizeof( uint16_t ) );


        // Allocate a temporary buffer for building the VBO
        std::unique_ptr < CVertex3D[] > upVBO( new CVertex3D[faceGroup.vertexBufCount] );

        // Build the VBO
        for( int j = 0; j < faceGroup.vertexBufCount; ++j )
        {
            upVBO[j].vert = upVert[ upVertBuf[j].vert ];
            upVBO[j].norm = upVNormal[ upVertBuf[j].norm ];
            upVBO[j].uv = upUV[ upVertBuf[j].uv ];
        }

        // Add a new entry into the vector
        meshVec.emplace_back();

        // Create the VBO
        glGenBuffers( 1, &meshVec.back().m_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, meshVec.back().m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CVertex3D)*faceGroup.vertexBufCount, upVBO.get(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        // Create the IBO - It's saved in the binary file as needed. Don't need to build it.
        glGenBuffers( 1, &meshVec.back().m_ibo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, meshVec.back().m_ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * faceGroup.indexBufCount, upIndexBuf.get(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        // Save the number of indexes in the IBO buffer - Will need this for the render call
        meshVec.back().m_iboCount = faceGroup.indexBufCount;

        // Reserve texture space
        meshVec.back().m_textureVec.reserve(faceGroup.textureCount);

        // Copy over the texture data
        for( int j = 0; j < faceGroup.textureCount; ++j )
        {
            int textIndex = upTextIndexBuf[j];
            meshVec.back().m_textureVec.emplace_back( textureVec.at(textIndex) );
        }
    }
}   // LoadFromFile


/************************************************************************
 *    desc: Load 3d mesh file without textures
 ************************************************************************/
void CMeshMgr::LoadFromFile(
    SDL_RWops * pFile,
    const CMeshBinaryFileHeader & fileHeader,
    const std::string & filePath,
    std::vector<CMesh3D> & meshVec )
{
    // Allocate local smart pointer arrays and load the unique verts and normals

    // Load in the verts
    std::unique_ptr < CPoint<float>[] > upVert( new CPoint<float>[fileHeader.vert_count] );
    TagCheck( pFile, filePath );
    SDL_RWread( pFile, upVert.get(), fileHeader.vert_count, sizeof( CPoint<float> ) );

    // Load in the normals
    std::unique_ptr < CNormal<float>[] > upVNormal( new CNormal<float>[fileHeader.vert_norm_count] );
    TagCheck( pFile, filePath );
    SDL_RWread( pFile, upVNormal.get(), fileHeader.vert_norm_count, sizeof( CNormal<float> ) );

    // Reserve the number of vbo groups
    meshVec.reserve( fileHeader.face_group_count );

    // Read in each face group
    for( int i = 0; i < fileHeader.face_group_count; ++i )
    {
        // Check to insure we are in the correct spot in the binary file
        TagCheck( pFile, filePath );

        // Get the number faces in the group as well as the material index
        CBinaryFaceGroup faceGroup;
        SDL_RWread( pFile, &faceGroup, 1, sizeof( faceGroup ) );

        // Allocate local smart pointer arrays
        std::unique_ptr<CBinaryVertexNoTxt[]> upVertBuf( new CBinaryVertexNoTxt[faceGroup.vertexBufCount] );
        std::unique_ptr<uint16_t[]> upIndexBuf( new uint16_t[faceGroup.indexBufCount] );

        // Read in the indexes used to create the VBO
        SDL_RWread( pFile, upVertBuf.get(), faceGroup.vertexBufCount, sizeof( CBinaryVertexNoTxt ) );

        // Read in the indexes that are the IBO
        SDL_RWread( pFile, upIndexBuf.get(), faceGroup.indexBufCount, sizeof( uint16_t ) );

        // Allocate a temporary buffer for building the VBO
        std::unique_ptr<CVertex3D_no_txt[]> upVBO( new CVertex3D_no_txt[faceGroup.vertexBufCount] );

        // Build the VBO
        for( int j = 0; j < faceGroup.vertexBufCount; ++j )
        {
            upVBO[j].vert = upVert[ upVertBuf[j].vert ];
            upVBO[j].norm = upVNormal[ upVertBuf[j].norm ];
        }

        // Add a new entry into the vector
        meshVec.emplace_back();

        // Create the VBO
        glGenBuffers( 1, &meshVec.back().m_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, meshVec.back().m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CVertex3D_no_txt)*faceGroup.vertexBufCount, upVBO.get(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        // Create the IBO - It's saved in the binary file as needed. Don't need to build it.
        glGenBuffers( 1, &meshVec.back().m_ibo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, meshVec.back().m_ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * faceGroup.indexBufCount, upIndexBuf.get(), GL_STATIC_DRAW );

        // unbind the buffer
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        // Save the number of indexes in the IBO buffer - Will need this for the render call
        meshVec.back().m_iboCount = faceGroup.indexBufCount;
    }
}   // LoadFromFile


/************************************************************************
 *    desc: Load 3d collision mesh file
 ************************************************************************/
void CMeshMgr::LoadFrom3DM(
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
    TagCheck( scpFile.get(), filePath );
    SDL_RWread( scpFile.get(), upVert.get(), fileHeader.vert_count, sizeof( CPoint<float> ) );


    // Check to insure we are in the correct spot in the binary file
    TagCheck( scpFile.get(), filePath );

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
        
}   // LoadFrom3DM


/************************************************************************
 *    desc: Do the tag check to insure we are in the correct spot
 ************************************************************************/
void CMeshMgr::TagCheck( SDL_RWops * file, const std::string & filePath )
{
    int tag_check;

    // Read in new tag
    SDL_RWread( file, &tag_check, 1, sizeof( tag_check ) );

    if( tag_check != TAG_CHECK )
        throw NExcept::CCriticalException( "Visual Mesh Load Error!",
            boost::str( boost::format( "Tag check mismatch (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );

}   // TagCheck


/************************************************************************
 *    desc:  Delete mesh buffer group
 ************************************************************************/
void CMeshMgr::DeleteBufferGroup( const std::string & group )
{
    auto mapMapIter = m_meshBufMapMapVec.find( group );
    if( mapMapIter != m_meshBufMapMapVec.end() )
    {
        // Delete all the buffers in this group
        for( auto & mapIter : mapMapIter->second )
        {
            for( auto & iter : mapIter.second )
            {
                glDeleteBuffers( 1, &iter.m_ibo );
                glDeleteBuffers( 1, &iter.m_vbo );
            }
        }

        // Erase this group
        m_meshBufMapMapVec.erase( mapMapIter );
    }

}   // DeleteBufferGroup
