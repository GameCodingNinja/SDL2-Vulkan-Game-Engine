
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
            StrategyMgr.getStrategy("_level_1_").destroy(sprite.getId());
            break;
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