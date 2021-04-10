// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  void ApplyGamepadOptions() {
    float pluginEnabled = zoptions->ReadBool( "ZBRIGHTLIGHT", "Enabled",    Opt_PluginEnabled );
    float brightness    = zoptions->ReadReal( "ZBRIGHTLIGHT", "Brightness", Opt_Brightness    );
    float nightMode     = zoptions->ReadInt(  "ZBRIGHTLIGHT", "NightMode",  Opt_NightMode     );

    // Safe hooks enabled/disabled
    static bool systemEnabled = false;
    if( !systemEnabled && pluginEnabled ) {
      PluginHooksEnabled();
      systemEnabled = true;
    }
    else if( systemEnabled && !pluginEnabled ) {
      PluginHooksDisabled();
      systemEnabled = false;
    }

    // Safe values changing
    bool valuesChanged =
      pluginEnabled != Opt_PluginEnabled ||
      brightness    != Opt_Brightness ||
      nightMode     != Opt_NightMode;

    if( !valuesChanged )
      return;

    Opt_PluginEnabled = pluginEnabled;
    Opt_Brightness    = brightness;
    Opt_NightMode     = nightMode;

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