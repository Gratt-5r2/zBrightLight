// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  void ApplyGamepadOptions() {
    int   pluginEnabled          =  zoptions->ReadBool( "ZBRIGHTLIGHT", "Enabled",    Opt_PluginEnabled );
    float brightness             =  zoptions->ReadReal( "ZBRIGHTLIGHT", "Brightness", Opt_Brightness    );
    int   nightMode              =  zoptions->ReadInt(  "ZBRIGHTLIGHT", "NightMode",  Opt_NightMode     );
    int   softGI                 =  zoptions->ReadInt(  "ZBRIGHTLIGHT", "SoftGI",     Opt_SoftGI        );
    int   gameWorldOnly          = !zoptions->ReadInt(  "ZBRIGHTLIGHT", "Inventory", !Opt_GameWorldOnly );

    // Safe hooks enabled/disabled
    static bool systemEnabled = false;
    if( !systemEnabled && pluginEnabled ) {
      PluginHooksEnabled();
      PluginHooksEnabled_SoftGI();
      systemEnabled = true;
    }
    else if( systemEnabled && !pluginEnabled ) {
      PluginHooksDisabled();
      PluginHooksDisabled_SoftGI();
      systemEnabled = false;
    }

    // Safe values changing
    bool valuesChanged =
      pluginEnabled != Opt_PluginEnabled ||
      brightness    != Opt_Brightness ||
      nightMode     != Opt_NightMode ||
      softGI        != Opt_SoftGI ||
      gameWorldOnly != Opt_GameWorldOnly;

    if( !valuesChanged )
      return;

    Opt_PluginEnabled = pluginEnabled;
    Opt_Brightness    = brightness;
    Opt_NightMode     = nightMode;
    Opt_SoftGI        = softGI;
    Opt_GameWorldOnly = gameWorldOnly;

    oCWorld* world = ogame->GetGameWorld();
    if( world != Null ) {
      auto outdoor = world->GetActiveSkyControler()->CastTo<zCSkyControler_Outdoor>();
      if( outdoor != Null && outdoor->initDone == True )
        outdoor->Interpolate();
    }
  }

  HOOK Hook_CGameManager_ApplySomeSettings PATCH( &CGameManager::ApplySomeSettings, &CGameManager::ApplySomeSettings_Union );

  void CGameManager::ApplySomeSettings_Union() {
    ApplyGamepadOptions();
    THISCALL( Hook_CGameManager_ApplySomeSettings )();
  }
}