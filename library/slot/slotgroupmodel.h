
/************************************************************************
*    FILE NAME:       slotgroupmodel.h
*
*    DESCRIPTION:     Model class for the slot group
************************************************************************/

#ifndef __slot_group_model_h__
#define __slot_group_model_h__

// Game lib dependencies
#include <slot/slotstripmodel.h>

// Standard lib dependencies
#include <deque>
#include <random>
#include <string>
#include <vector>

// Forward declaration(s)
class CSlotMath;
class CMathSymbol;
class CPlayResult;
class CPaylineSet;
class CPayCombo;
struct XMLNode;

class CSlotGroupModel
{
public:

    // Constructor
    CSlotGroupModel( const CSlotMath & rSlotMath, CPlayResult & rPlayResult );

    // Destructor
    virtual ~CSlotGroupModel();
    
    // Create the model strips
    void create( const std::string & rStripSetId, const std::string & paytableSetId );
    
    // Generate the reel stops
    void generateStops();
    
    // Evaluate the reels
    void evaluate();
    
    // Get the strips
    const CSlotStripModel & getStrip( int index ) const;
    
    // Get the number of math strips
    size_t getCount() const;
    
private:
    
    // Generate the evaluation symbols
    void generateEvalSymbs();
    
    // Evaluate the line pays
    void evaluateLinePays( const std::string & paytable );
    
    // Evaluate the scatter pays
    void evaluateScatters( const std::string & paytable );
    
    // Add line pay to slot result
    void addLinePay(
        const CPayCombo & rPayCombo,
        const int payline,
        const std::vector<std::vector<int8_t>> & rPaylineSetVecVec );
    
private:
    
    // slot math reference
    const CSlotMath & m_rSlotMath;
    
    // play result reference
    CPlayResult & m_rPlayResult;
    
    // Payline set
    const CPaylineSet & m_rPaylineSet;
    
    // Deque of math strip models
    std::deque<CSlotStripModel> m_slotStripModelDeq;
    
    // Evaluation symbol vectors
    std::vector<std::vector<const CMathSymbol *>> m_evalMathSymbs;
    
    // Random number generation based on the Mersenne Twister algorithm
    std::mt19937 m_rng;
    
    // Paytable Set Id
    std::string m_paytableSetId;

};

#endif  // __slot_group_model_h__


