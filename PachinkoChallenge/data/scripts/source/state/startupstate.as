
//
//  FILE NAME:  startupstate.as
//
//  DESC:       Start up game state
//

final class CStartUpState : CCommonState
{
    //
    //  Constructor
    //
    CStartUpState()
    {
        super( NStateDefs::EGS_STARTUP, NStateDefs::EGS_TITLE_SCREEN );
    }
    
    //
    //  Destroy the state
    //
    void destroy() override
    {
        Device.deleteCommandPoolGroup( "(startup)" );
        ObjectDataMgr.freeGroup( "(startup)" );
        StrategyMgr.deleteStrategy( "_startup_" );
    }

    //
    //  Init the state
    //
    void init() override
    {
        // Init the additive color before any rendering is started
        CVisual visual;
        visual.setAdditiveColor(0,0,0,1);

        // Load the data lists
        ObjectDataMgr.loadListTable( "data/objects/2d/objectDataList/dataTable.lst" );
        StrategyMgr.loadListTable( "data/objects/strategy/strategyTable.lst" );
        
        // Load group specific assets
        ObjectDataMgr.loadGroup( "(startup)" );
        
        // Add the command buffers to the menu manager
        MenuMgr.setCommandBuffer( "(menu)" );
        
        // Create the needed strategy
        StrategyMgr.loadStrategy( "data/objects/strategy/state/startup.loader" );
        
        // Do the fade in
        Spawn("State_StartUpFadeIn");
    }
    
    //
    //  Handle the events
    //
    void handleEvent( const CEvent & event ) override
    {
        if( event.type > NEvents::SDL_USEREVENT )
        {
            if( event.type == NStateDefs::ESE_FADE_IN_COMPLETE )
                SpawnByThread("LoadStartUpAssets");
            
            else if( event.type == NStateDefs::ESE_THREAD_LOAD_COMPLETE )
                Spawn("State_FadeOut");
            
            else if( event.type == NStateDefs::ESE_FADE_OUT_COMPLETE )
                mChangeState = true;
        }
    }
};

//
//  Load the assets
//
void LoadStartUpAssets()
{
    // Load the list tables
    MenuMgr.loadListTable( "data/objects/2d/menu/menuListTable.lst" );
    SoundMgr.loadListTable( "data/sound/soundListTable.lst" );
    PhysicsWorldManager2D.loadListTable( "data/objects/2d/physics/physicsListTable.lst" );

    // Load the menu scripts
    ScriptMgr.loadGroup( "(menu)" );

    // Load in any fonts
    FontMgr.load( "data/textures/fonts/font.lst" );

    // Load the action manager - Must be loaded before memu system
    ActionMgr.load( "data/settings/controllerMapping.cfg" );

    // Load the menu action list
    MenuMgr.loadMenuAction( "data/objects/2d/menu/menu_action.list" );

    // Load the start up animation group
    ObjectDataMgr.loadGroup( "(menu)" );

    // Load the menu sounds
    SoundMgr.loadGroup("(menu)");

    // Load the menu group
    MenuMgr.loadGroup("(menu)");

    // Load the next states assets
    LoadTitleScreenAssets();
}
    