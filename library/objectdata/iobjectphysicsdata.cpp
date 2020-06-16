
/************************************************************************
*    FILE NAME:       iobjectphysicsdata.h
*
*    DESCRIPTION:     Object Physics Data interface class
************************************************************************/

// Physical component dependency
#include <objectdata/iobjectphysicsdata.h>

std::string iObjectPhysicsData::m_null_string;
std::vector<CFixture> iObjectPhysicsData::m_null_fixture_vec;
CPoint<float> iObjectPhysicsData::m_null_point;

// Constructor / Destructor
iObjectPhysicsData::iObjectPhysicsData()
{}

iObjectPhysicsData::~iObjectPhysicsData()
{}