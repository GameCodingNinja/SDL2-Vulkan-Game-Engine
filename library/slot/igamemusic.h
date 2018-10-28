
/************************************************************************
*    FILE NAME:       igamemusic.h
*
*    DESCRIPTION:     Interface for handling game music
************************************************************************/

#ifndef __i_game_music_h__
#define __i_game_music_h__

class iGameMusic
{
public:
    
    // Handle update checks
    virtual void update() = 0;
    
    // Start the music
    virtual void startMusic() = 0;
    
    // Start the music timeout
    virtual void setTimeOut() = 0;
};

#endif
