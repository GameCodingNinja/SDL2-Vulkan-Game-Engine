
//
//  FILE NAME:  statemessage.as
//
//  DESC:       State message class
//

class CStateMessage
{
    private NStateDefs::EGameState mLoadState = NStateDefs::EGS_NULL;
    private NStateDefs::EGameState mUnloadState = NStateDefs::EGS_NULL;

    NStateDefs::EGameState getLoadState()
    { return mLoadState; }

    NStateDefs::EGameState getUnloadState()
    { return mUnloadState; }

    void setMsg( const NStateDefs::EGameState loadState, const NStateDefs::EGameState unloadState )
    {
        mLoadState = loadState;
        mUnloadState = unloadState;
    }
}
