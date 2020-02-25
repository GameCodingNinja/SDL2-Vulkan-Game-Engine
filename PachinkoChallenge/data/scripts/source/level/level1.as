
//
//  FILE NAME:  level1.as
//
//  DESC:       Scripts specifically for the level 1 state
//


//
//  AI Update script
//
void Level1_BallAI( CSprite & sprite )
{
    while(true)
    {
        if( sprite.getPos().y > 1700.f )
        {
            StrategyMgr.getStrategy("_level_1_actor_").destroy(sprite.getId());
            break;
        }
        else if(
            (sprite.getPos().y > 1600.f) && 
            (abs(sprite.getPos().x) < 720) && 
            !sprite.getParameters().isSet(NLevelDefs::ESS_BANG_UP_AWARD) )
        {
            // Set the state so as to not enter this if again
            sprite.getParameters().add(NLevelDefs::ESS_BANG_UP_AWARD);

            // Dispatch message to bang this one up
            DispatchEvent(NLevelDefs::ELE_BANG_UP_AWARD);
        }

        Suspend();
    }
}

//
//  AI Update script
//
void Level_PegOff( CSprite & sprite )
{
    Hold( 200 );

    sprite.setFrame(0);
}