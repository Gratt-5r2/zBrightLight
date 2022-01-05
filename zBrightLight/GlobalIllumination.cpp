// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static Array<zCVisual*> HighSoftModelList;
  static Array<zCVisual*> MidSoftModelList;
  static Array<zCVisual*> LowSoftModelList;
  static float HightGlobalIllumination = 0.6f;
  static float MidGlobalIllumination   = 0.5f;
  static float LowGlobalIllumination   = 0.43f;

  static void ResetSoftLightModelLists() {
    HighSoftModelList.Clear();
    MidSoftModelList.Clear();
    LowSoftModelList.Clear();
  }

  static Array<string> ReadStringArray( const string& section, const string& parameter, const string& default = "" ) {
    string source = zoptions->ReadString( Z section, Z parameter, default );
    Array<string> array = source.Split( "," );
    for( uint i = 0; i < array.GetNum(); i++ )
      array[i].Shrink().Upper();
    
    return array;
  }

  static void CheckSoftModelInList( zCVisual* visual, Array<zCVisual*>& list ) {
    if( list.HasEqualSorted( visual ) )
      return;

    list.InsertSorted( visual );
  }

  static void CheckSoftModel( zCVisual* visual ) {
    if( !visual )
      return;

    static Array<string> highSoftFilter = ReadStringArray( "zBrightLight", "highSoftLightModels", "*grass*" );
    static Array<string> midSoftFilter  = ReadStringArray( "zBrightLight", "midSoftLightModels",  "*fern*, *farn*, *weed*, *clover*, *plant*, *lupine*" );
    static Array<string> lowSoftFilter  = ReadStringArray( "zBrightLight", "lowSoftLightModels",  "*bush*, *webs*" );

    string visualName = visual->GetVisualName().Upper();
    if( visualName.IsEmpty() || visualName.EndWith( ".TGA" ) )
      return;

    for( uint i = 0; i < highSoftFilter.GetNum(); i++ )
      if( visualName.CompareMasked( highSoftFilter[i] ) )
        return CheckSoftModelInList( visual, HighSoftModelList );

    for( uint i = 0; i < midSoftFilter.GetNum(); i++ )
      if( visualName.CompareMasked( midSoftFilter[i] ) )
        return CheckSoftModelInList( visual, MidSoftModelList );

    for( uint i = 0; i < lowSoftFilter.GetNum(); i++ )
      if( visualName.CompareMasked( lowSoftFilter[i] ) )
        return CheckSoftModelInList( visual, LowSoftModelList );
  }

  HOOK Hook_zCVob_SetVisual PATCH_IF( &zCVob::SetVisual, &zCVob::SetVisual_Union, false );

  void zCVob::SetVisual_Union( zCVisual* visual ) {
    THISCALL( Hook_zCVob_SetVisual )(visual);
    CheckSoftModel( visual );
  }

  static zVEC3 GetFloorColorVec3( zCVob* vob ) {
    zCOLOR floorLight = vob->GetLightColorStat();
    if( Opt_SoftGI == SoftLight_Exponental ) {
      zVEC3 light( floorLight.r, floorLight.g, floorLight.b );
      float length = light.Length();
      float multiplier = SmoothIncrease( length, 255, 2 );
      return light.Normalize() * multiplier;
    }
    else
      return zVEC3( floorLight.r, floorLight.g, floorLight.b );
  }

  static bool IsOutDoorSector( const zTRenderContext& renderContext ) {
    zCPolygon* groundPoly = renderContext.vob->groundPoly;
    return !groundPoly || !groundPoly->flags.sectorPoly || renderContext.world->GetBspTree()->bspTreeMode == zBSP_MODE_OUTDOOR;
  }

  static bool VobIsInSoftList( zCVob* vob, Array<zCVisual*>& list ) {
    if( !vob || !vob->visual )
      return false;

    return list.HasEqualSorted( vob->visual );
  }

  HOOK Hook_zCRenderLightContainer_CollectLights_StatLights PATCH_IF( &zCRenderLightContainer::CollectLights_StatLights, &zCRenderLightContainer::CollectLights_StatLights_Union, false );

  void zCRenderLightContainer::CollectLights_StatLights_Union( const zTRenderContext& renderContext ) {
    THISCALL( Hook_zCRenderLightContainer_CollectLights_StatLights )(renderContext);
    if( Opt_SoftGI == SoftLight_Disabled )
      return;

    bool outdoor = IsOutDoorSector( renderContext );
    if( !outdoor )
      return;

    float giIntensity = 0.0f;
         if( renderContext.vob->type == zVOB_TYPE_ITEM )               giIntensity = MidGlobalIllumination;
    else if( VobIsInSoftList( renderContext.vob, HighSoftModelList ) ) giIntensity = HightGlobalIllumination;
    else if( VobIsInSoftList( renderContext.vob, MidSoftModelList  ) ) giIntensity = MidGlobalIllumination;
    else if( VobIsInSoftList( renderContext.vob, LowSoftModelList  ) ) giIntensity = LowGlobalIllumination;
    else
      return;

    zCRenderLight& pmLight = lightList[numLights++];
    zVEC3 floorColor       = GetFloorColorVec3( renderContext.vob );
    pmLight.lightType      = zLIGHT_TYPE_AMBIENT;
    pmLight.colorDiffuse   = floorColor * giIntensity;
  };

  static void PluginHooksEnabled_SoftGI() {
    Hook_zCVob_SetVisual.Commit();
    Hook_zCRenderLightContainer_CollectLights_StatLights.Commit();
  }

  static void PluginHooksDisabled_SoftGI() {
    Hook_zCVob_SetVisual.Detach();
    Hook_zCRenderLightContainer_CollectLights_StatLights.Detach();
  }
}