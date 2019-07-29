
/************************************************************************
*    FILE NAME:       shared_utilities.as
*
*    DESCRIPTION:     General purpose shared utility scripts
*                     The keyword "shared" allows for only one copy to
*                     loaded and used among all modules
************************************************************************/

/************************************************************************
*    DESC:  Hold the script execution in time
************************************************************************/
shared void Hold( float time )
{
    do
    {
        time -= HighResTimer.getElapsedTime();

        Suspend();
    }
    while( time > 0 );
}
