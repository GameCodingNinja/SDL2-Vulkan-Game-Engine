
/************************************************************************
*    FILE NAME:       shared_sprite_utilities.as
*
*    DESCRIPTION:     General purpose shared utility scripts
*                     The keyword "shared" allows for only one copy to
*                     loaded and used among all modules
************************************************************************/

namespace NVisualUtils
{
    /************************************************************************
    *    DESC:  Color to the final color in time
    ************************************************************************/
    shared void ColorTo( float time, CColor &in final, CVisual &in visual )
    {
        CColor current = visual.getAdditiveColor();
        CColor inc = (final - current) / time;

        do
        {
            time -= HighResTimer.getElapsedTime();

            if( time < 0 )
            {
                visual.setAdditiveColor( final );
            }
            else
            {
                current += inc * HighResTimer.getElapsedTime();
                visual.setAdditiveColor( current );
            }

            Suspend();
        }
        while( time > 0 );
    }
}