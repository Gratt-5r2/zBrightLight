// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  enum zTSoftLightMode {
    SoftLight_Disabled,
    SoftLight_Default,
    SoftLight_Exponental
  };

  int Opt_PluginEnabled = True;
  float Opt_Brightness = 0.85f;
  int Opt_AutoBrightness = True;
  int Opt_NightMode = 1;
  int Opt_SoftGI = 2;
  int Opt_GameWorldOnly = 0;

  void ApplyGamepadOptions();
}