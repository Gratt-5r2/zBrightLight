// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Hook_zCRnd_D3D_SetTextureStageState PATCH_IF( &zCRnd_D3D::SetTextureStageState, &zCRnd_D3D::SetTextureStageState_Union,      false );
  HOOK Hook_oCViewDialogItem_Blit          PATCH_IF( &oCViewDialogItem::Blit,          &zCViewBase::oCViewDialogItem_Blit_Union,    false );
  HOOK Hook_zCViewDraw_Blit                PATCH_IF( &zCViewDraw::Blit,                &zCViewBase::zCViewDraw_Blit_Union,          false );
  HOOK Hook_zCViewFX_Blit                  PATCH_IF( &zCViewFX::Blit,                  &zCViewBase::zCViewFX_Blit_Union,            false );
  HOOK Hook_zCViewPrint_Blit               PATCH_IF( &zCViewPrint::Blit,               &zCViewBase::zCViewPrint_Blit_Union,         false );
  HOOK Hook_zCView_Blit                    PATCH_IF( &zCView::Blit,                    &zCViewBase::zCView_Blit_Union,              false );
  HOOK Hook_zCViewDialogChoice_BlitText    PATCH_IF( &zCViewDialogChoice::BlitText,    &zCViewBase::zCViewDialogChoice_BlitText,    false );
  HOOK Hook_zCViewDraw_BlitTexture         PATCH_IF( &zCViewDraw::BlitTexture,         &zCViewBase::zCViewDraw_BlitTexture,         false );
  HOOK Hook_zCViewPrint_BlitText           PATCH_IF( &zCViewPrint::BlitText,           &zCViewBase::zCViewPrint_BlitText,           false );
  HOOK Hook_zCViewPrint_BlitTextLine       PATCH_IF( &zCViewPrint::BlitTextLine,       &zCViewBase::zCViewPrint_BlitTextLine,       false );
  HOOK Hook_zCViewPrint_BlitTextCharacters PATCH_IF( &zCViewPrint::BlitTextCharacters, &zCViewBase::zCViewPrint_BlitTextCharacters, false );
  HOOK Hook_zCView_BlitText                PATCH_IF( &zCView::BlitText,                &zCViewBase::zCView_BlitText,                false );

  static void PluginHooksEnabled() {
    Hook_zCRnd_D3D_SetTextureStageState.Commit();
    Hook_oCViewDialogItem_Blit.Commit();
    Hook_zCViewDraw_Blit.Commit();
    Hook_zCViewFX_Blit.Commit();
    Hook_zCViewPrint_Blit.Commit();
    Hook_zCView_Blit.Commit();
    Hook_zCViewDialogChoice_BlitText.Commit();
    Hook_zCViewDraw_BlitTexture.Commit();
    Hook_zCViewPrint_BlitText.Commit();
    Hook_zCViewPrint_BlitTextLine.Commit();
    Hook_zCViewPrint_BlitTextCharacters.Commit();
    Hook_zCView_BlitText.Commit();
    Hook_zCSkyControler_Outdoor_CalcPolyLightCLUT.Commit();
  }

  static void PluginHooksDisabled() {
    Hook_zCRnd_D3D_SetTextureStageState.Detach();
    Hook_oCViewDialogItem_Blit.Detach();
    Hook_zCViewDraw_Blit.Detach();
    Hook_zCViewFX_Blit.Detach();
    Hook_zCViewPrint_Blit.Detach();
    Hook_zCView_Blit.Detach();
    Hook_zCViewDialogChoice_BlitText.Detach();
    Hook_zCViewDraw_BlitTexture.Detach();
    Hook_zCViewPrint_BlitText.Detach();
    Hook_zCViewPrint_BlitTextLine.Detach();
    Hook_zCViewPrint_BlitTextCharacters.Detach();
    Hook_zCView_BlitText.Detach();
    Hook_zCSkyControler_Outdoor_CalcPolyLightCLUT.Detach();
  }



  
  inline ulong GetCurrentRenderObject() {
    uint address = (uint)_ReturnAddress();

    if( address == D3DTOP_Table[zERenderObject::Indoor] )
      return GetD3DTOP( zERenderObject::Indoor );

    if( address == D3DTOP_Table[zERenderObject::Outdoor] )
      return GetD3DTOP( zERenderObject::Outdoor );

    if( address == D3DTOP_Table[zERenderObject::Barrier] )
      return GetD3DTOP( zERenderObject::Barrier );

    if( address == D3DTOP_Table[zERenderObject::ScreenText] )
      return GetD3DTOP( zERenderObject::ScreenText );

    if( address == D3DTOP_Table[zERenderObject::Interface] )
      return GetD3DTOP( zERenderObject::Interface );

    return Invalid;
  }



  bool IsIndoorWorld() {
    return
      ogame == Null ||
      ogame->GetGameWorld() == Null ||
      ogame->GetGameWorld()->bspTree.bspTreeMode == zBSP_MODE_INDOOR;
  }



  int zCRnd_D3D::SetTextureStageState_Union( unsigned long stage, zTRnd_TextureStageState state, unsigned long value ) {
    ulong d3dTop = s_D3DStageLocked ?
      GetD3DTOP( zERenderObject::Interface ) :
      GetCurrentRenderObject();

    int ok = THISCALL( Hook_zCRnd_D3D_SetTextureStageState )(stage, state, value);
    if( d3dTop != Invalid && !IndoorMode )
      xd3d_pd3dDevice7->SetTextureStageState( 0, D3DTSS_COLOROP, d3dTop );

    return ok;
  }



  // Lock the interface bright invencity

  void zCViewBase::oCViewDialogItem_Blit_Union() {
    s_D3DStageLocked++;
    THISCALL( Hook_oCViewDialogItem_Blit )();
    s_D3DStageLocked--;
  }

  void zCViewBase::zCViewDraw_Blit_Union() {
    s_D3DStageLocked++;
    THISCALL( Hook_zCViewDraw_Blit )();
    s_D3DStageLocked--;
  }

  void zCViewBase::zCViewFX_Blit_Union() {
    s_D3DStageLocked++;
    THISCALL( Hook_zCViewFX_Blit )();
    s_D3DStageLocked--;
  }

  void zCViewBase::zCViewPrint_Blit_Union() {
    s_D3DStageLocked++;
    THISCALL( Hook_zCViewPrint_Blit )();
    s_D3DStageLocked--;
  }

  void zCViewBase::zCView_Blit_Union() {
    s_D3DStageLocked++;
    THISCALL( Hook_zCView_Blit )();
    s_D3DStageLocked--;
  }

  void zCViewBase::zCViewDialogChoice_BlitText() {
    s_D3DStageLocked++;
    THISCALL( Hook_zCViewDialogChoice_BlitText )();
    s_D3DStageLocked--;
  }

  void zCViewBase::zCViewDraw_BlitTexture() {
    s_D3DStageLocked++;
    THISCALL( Hook_zCViewDraw_BlitTexture )();
    s_D3DStageLocked--;
  }

  void zCViewBase::zCViewPrint_BlitText() {
    s_D3DStageLocked++;
    THISCALL( Hook_zCViewPrint_BlitText )();
    s_D3DStageLocked--;
  }

  void zCViewBase::zCViewPrint_BlitTextLine( zCViewText2* line ) {
    s_D3DStageLocked++;
    THISCALL( Hook_zCViewPrint_BlitTextLine )(line);
    s_D3DStageLocked--;
  }

  void zCViewBase::zCViewPrint_BlitTextCharacters( zCViewText2* line, zCFont* font, zCOLOR& color ) {
    s_D3DStageLocked++;
    THISCALL( Hook_zCViewPrint_BlitTextCharacters )(line, font, color);
    s_D3DStageLocked--;
  }

  void zCViewBase::zCView_BlitText() {
    s_D3DStageLocked++;
    THISCALL( Hook_zCView_BlitText )();
    s_D3DStageLocked--;
  }
}