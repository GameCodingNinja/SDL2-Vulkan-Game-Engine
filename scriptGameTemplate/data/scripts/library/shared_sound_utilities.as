
/************************************************************************
*    FILE NAME:       shared_sound_utilities.as
*
*    DESCRIPTION:     General purpose shared utility scripts
*                     The keyword "shared" allows for only one copy to
*                     loaded and used among all modules
************************************************************************/

namespace NSoundUtils
{
    /************************************************************************
    *    DESC:  Volume to the final volume
    ************************************************************************/
    shared void VolumeTo( float time, int final, CSound & sound )
    {
        float current = sound.getVolume();
        float inc = (float(final) - current) / time;
        int lastVolume = int(current);

        do
        {
            time -= HighResTimer.getElapsedTime();

            if( time < 0 )
            {
                sound.setVolume( int(final) );
            }
            else
            {
                current += inc * HighResTimer.getElapsedTime();

                if( lastVolume != int(current) )
                {
                    sound.setVolume( int(current) );
                    lastVolume = int(current);
                }
            }

            Suspend();
        }
        while( time > 0 );
    }
}
