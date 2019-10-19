
/************************************************************************
*    FILE NAME:       shared_sprite_utilities.as
*
*    DESCRIPTION:     General purpose shared utility scripts
*                     The keyword "shared" allows for only one copy to
*                     loaded and used among all modules
************************************************************************/

namespace NSpriteUtils
{
    /************************************************************************
    *    DESC:  Color to the final color in time
    ************************************************************************/
    shared void ColorTo( float time, CColor &in final, CSprite & sprite )
    {
        CColor current = sprite.getColor();
        CColor inc = (final - current) / time;

        do
        {
            time -= HighResTimer.getElapsedTime();

            if( time < 0 )
            {
                sprite.setColor( final );
            }
            else
            {
                current += inc * HighResTimer.getElapsedTime();
                sprite.setColor( current );
            }

            Suspend();
        }
        while( time > 0 );
    }


    /************************************************************************
    *    DESC:  Alpha to the final alpha
    ************************************************************************/
    shared void AlphaTo( float time, float final, CSprite & sprite )
    {
        // Convert from RGB value to normalized value
        if( final > 1.5 )
            final *= 0.00390625f;

        float current = sprite.getAlpha();
        float inc = (final - current) / time;

        do
        {
            time -= HighResTimer.getElapsedTime();

            if( time < 0 )
            {
                sprite.setAlpha( final );
            }
            else
            {
                current += inc * HighResTimer.getElapsedTime();
                sprite.setAlpha( current );
            }

            Suspend();
        }
        while( time > 0 );
    }


    /************************************************************************
    *    DESC:  Play the animation
    ************************************************************************/
    shared void Play( int fps, CSprite & sprite, bool loop = false )
    {
        float time = 1000.0 / fps;
        uint frameCount = sprite.getFrameCount();

        do
        {
            for( uint i = 0; i < frameCount; ++i )
            {
                sprite.setFrame( i );

                Hold( time );
            }
        }
        while( loop );
    }
}