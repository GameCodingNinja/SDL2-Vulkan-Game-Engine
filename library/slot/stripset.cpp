
/************************************************************************
*    FILE NAME:       stripset.cpp
*
*    DESCRIPTION:     Strip set math class
************************************************************************/

// Physical component dependency
#include <slot/stripset.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CStripSet::CStripSet( const std::string & id, const std::vector<int8_t> & evalSymbIndexVec ) :
    m_id(id),
    m_evalSymbIndexVec(evalSymbIndexVec)
{
}


/************************************************************************
*    DESC:  Get the paytable ID
************************************************************************/
const std::string & CStripSet::getId() const
{
    return m_id;
}


/************************************************************************
*    DESC:  Get the evaluation symbol index count
************************************************************************/
const std::vector<int8_t> & CStripSet::getEvalIndexVec() const
{
    return m_evalSymbIndexVec;
}
