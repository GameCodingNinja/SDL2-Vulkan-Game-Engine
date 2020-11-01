
/************************************************************************
*    FILE NAME:       objectphysicsdata2d.cpp
*
*    DESCRIPTION:     Class containing the 2D object's physics data
************************************************************************/

// Physical component dependency
#include <objectdata/objectphysicsdata2d.h>

// Game lib dependencies
#include <common/color.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectPhysicsData2D::CObjectPhysicsData2D() :
    m_bodyType(b2BodyType(defs_NULL_BODY_TYPE)),
    m_linearDamping(0),
    m_angularDamping(0),
    m_fixedRotation(false),
    m_bullet(false),
    m_sleepingAllowed(true)
{
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CObjectPhysicsData2D::~CObjectPhysicsData2D()
{
}


/************************************************************************
*    DESC:  Load thes object data from node
************************************************************************/
void CObjectPhysicsData2D::loadFromNode( const XMLNode & objectNode )
{
    const XMLNode physicsNode = objectNode.getChildNode( "physics" );

    // Check if the object has any physics data
    if( !physicsNode.isEmpty() )
    {
        if( physicsNode.isAttributeSet( "world" ) )
            m_world = physicsNode.getAttribute( "world" );

        // The body of the physics sprite used for physics
        const XMLNode bodyNode = physicsNode.getChildNode( "body" );
        if( !bodyNode.isEmpty() )
        {
            // Get the body type - default is static
            if( bodyNode.isAttributeSet( "type" ) )
            {
                std::string bodyType = bodyNode.getAttribute( "type" );

                if( bodyType == "static" )
                    m_bodyType = b2_staticBody;

                else if( bodyType == "dynamic" )
                    m_bodyType = b2_dynamicBody;

                else if( bodyType == "kinematic" )
                    m_bodyType = b2_kinematicBody;
            }

            // The damping is the constant decceleration of movement
            if( bodyNode.isAttributeSet("linearDamping") )
                m_linearDamping = std::atof( bodyNode.getAttribute( "linearDamping" ) );

            // The angular damping is the constant decceleration of rotation
            if( bodyNode.isAttributeSet("angularDamping") )
                m_angularDamping = std::atof( bodyNode.getAttribute( "angularDamping" ) );

            // Whether the rotation due to physicss is fixed
            if( bodyNode.isAttributeSet("fixedRotation") )
                m_fixedRotation = (std::strcmp( bodyNode.getAttribute("fixedRotation"), "true") == 0);

            // Is this body a bullet
            if( bodyNode.isAttributeSet("bullet") )
                m_bullet = (std::strcmp( bodyNode.getAttribute("bullet"), "true") == 0);

            // Is sleeping allowed
            if( bodyNode.isAttributeSet("sleepingAllowed") )
                m_sleepingAllowed = (std::strcmp( bodyNode.getAttribute("sleepingAllowed"), "true") == 0);
        }

        // The body of the physics sprite used for physics
        const XMLNode fixtureLstNode = physicsNode.getChildNode( "fixtureList" );
        if( !fixtureLstNode.isEmpty() )
        {
            // Allocate all the needed fixtures
            m_fixtureVec.resize( fixtureLstNode.nChildNode() );

            int counter(0);
            for( auto & iter : m_fixtureVec )
            {
                const XMLNode fixtureNode = fixtureLstNode.getChildNode( "fixture", counter++ );

                // Get the fixture shape
                if( fixtureNode.isAttributeSet( "shape" ) )
                {
                    std::string shape = fixtureNode.getAttribute( "shape" );

                    if( shape == "circle" )
                        iter.m_shape = b2Shape::e_circle;

                    else if( shape == "edge" )
                        iter.m_shape = b2Shape::e_edge;

                    else if( shape == "polygon" )
                        iter.m_shape = b2Shape::e_polygon;

                    else if( shape == "chain" )
                        iter.m_shape = b2Shape::e_chain;
                }

                // The friction is how much drag this object has on another object
                if( fixtureNode.isAttributeSet("friction") )
                    iter.m_friction = std::atof( fixtureNode.getAttribute( "friction" ) );

                // The density is how much the object resists movement
                if( fixtureNode.isAttributeSet("density") )
                    iter.m_density = std::atof( fixtureNode.getAttribute( "density" ) );

                // The restitution is the percentage of velocity retained upon physics
                if( fixtureNode.isAttributeSet("restitution") )
                    iter.m_restitution = std::atof( fixtureNode.getAttribute( "restitution" ) );

                // Radius if shape is a circle
                if( fixtureNode.isAttributeSet("radius") )
                    iter.m_radius = std::atof( fixtureNode.getAttribute( "radius" ) );

                // Is chain shape a loop?
                if( fixtureNode.isAttributeSet("chainLoop") )
                    iter.m_chainLoop = (std::strcmp(fixtureNode.getAttribute( "chainLoop" ), "true") == 0);

                // Is fixture a sensor?
                if( fixtureNode.isAttributeSet("sensor") )
                    iter.m_sensor = (std::strcmp(fixtureNode.getAttribute( "sensor" ), "true") == 0);

                // See if there is a vert list
                const XMLNode vertLstNode = fixtureNode.getChildNode("vertList");
                if( !vertLstNode.isEmpty() )
                {
                    iter.m_vertVec.reserve( vertLstNode.nChildNode() );

                    for( int i = 0; i < vertLstNode.nChildNode(); ++i )
                    {
                        const XMLNode vertNode = vertLstNode.getChildNode( i );

                        iter.m_vertVec.emplace_back(
                            std::atof( vertNode.getAttribute("x") ),
                            std::atof( vertNode.getAttribute("y") ) );
                    }
                }

                // See if the filter is defined
                const XMLNode filterNode = fixtureNode.getChildNode("collisionFilter");
                if( !filterNode.isEmpty() )
                {
                    if( filterNode.isAttributeSet("categoryBits") )
                        iter.m_filter.categoryBits = std::atoi( filterNode.getAttribute("categoryBits") );

                    if( filterNode.isAttributeSet("maskBits") )
                        iter.m_filter.maskBits = std::atoi( filterNode.getAttribute("maskBits") );

                    if( filterNode.isAttributeSet("groupIndex") )
                        iter.m_filter.groupIndex = std::atoi( filterNode.getAttribute("groupIndex") );
                }

                // The size mod is how much the mesh size should be adjusted on each side
                const XMLNode sizeModNode = fixtureNode.getChildNode( "sizeMod" );
                if( !sizeModNode.isEmpty() )
                {
                    if( sizeModNode.isAttributeSet("top") )
                        iter.m_topMod = std::atof( sizeModNode.getAttribute( "top" ) );

                    if( sizeModNode.isAttributeSet("bottom") )
                        iter.m_bottomMod = std::atof( sizeModNode.getAttribute( "bottom" ) );

                    if( sizeModNode.isAttributeSet("left") )
                        iter.m_leftMod = std::atof( sizeModNode.getAttribute( "left" ) );

                    if( sizeModNode.isAttributeSet("right") )
                        iter.m_rightMod = std::atof( sizeModNode.getAttribute( "right" ) );
                }
            }
        }

        // Put out a warning if the physics world is not defined
        if( m_world.empty() && m_bodyType != defs_NULL_BODY_TYPE && !m_fixtureVec.empty() )
            NGenFunc::PostDebugMsg( "WARNING: Physics world not defined even though body type and fixture is." );
    }
}


/************************************************************************
*    DESC:  Get the name of the physics world
************************************************************************/
const std::string & CObjectPhysicsData2D::getWorld() const
{
    return m_world;
}


/************************************************************************
*    DESC:  Get the type of physics body
************************************************************************/
b2BodyType CObjectPhysicsData2D::getBodyType() const
{
    return m_bodyType;
}


/************************************************************************
*    DESC:  Get the linear damping
************************************************************************/
float CObjectPhysicsData2D::getLinearDamping() const
{
    return m_linearDamping;
}


/************************************************************************
*    DESC:  Get the angular damping
************************************************************************/
float CObjectPhysicsData2D::getAngularDamping() const
{
    return m_angularDamping;
}


/************************************************************************
*    DESC:  Is the rotation fixed
************************************************************************/
bool CObjectPhysicsData2D::isRotationFixed() const
{
    return m_fixedRotation;
}


/************************************************************************
*    DESC:  Get the fixture data
************************************************************************/
const std::vector<CFixture> & CObjectPhysicsData2D::getFixtureVec() const
{
    return m_fixtureVec;
}


/************************************************************************
*    DESC:  Specify if physics is active
************************************************************************/
bool CObjectPhysicsData2D::isActive() const
{
    return (!m_world.empty() && (m_bodyType != b2BodyType(defs_NULL_BODY_TYPE)));
}


/************************************************************************
*    DESC:  Is this body a bullet
************************************************************************/
bool CObjectPhysicsData2D::isBullet() const
{
    return m_bullet;
}