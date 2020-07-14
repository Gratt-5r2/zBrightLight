// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

  HOOK Ivk_zCRnd_D3D_SetTextureStageState  PATCH( &zCRnd_D3D::SetTextureStageState, &zCRnd_D3D::SetTextureStageState_Union );
  HOOK Hook_oCViewDialogItem_Blit          PATCH( &oCViewDialogItem::Blit,          &zCViewBase::oCViewDialogItem_Blit_Union );
  HOOK Hook_zCViewDraw_Blit                PATCH( &zCViewDraw::Blit,                &zCViewBase::zCViewDraw_Blit_Union );
  HOOK Hook_zCViewFX_Blit                  PATCH( &zCViewFX::Blit,                  &zCViewBase::zCViewFX_Blit_Union );
  HOOK Hook_zCViewPrint_Blit               PATCH( &zCViewPrint::Blit,               &zCViewBase::zCViewPrint_Blit_Union );
  HOOK Hook_zCView_Blit                    PATCH( &zCView::Blit,                    &zCViewBase::zCView_Blit_Union );
  HOOK Hook_zCViewDialogChoice_BlitText    PATCH( &zCViewDialogChoice::BlitText,    &zCViewBase::zCViewDialogChoice_BlitText );
  HOOK Hook_zCViewDraw_BlitTexture         PATCH( &zCViewDraw::BlitTexture,         &zCViewBase::zCViewDraw_BlitTexture );
  HOOK Hook_zCViewPrint_BlitText           PATCH( &zCViewPrint::BlitText,           &zCViewBase::zCViewPrint_BlitText );
  HOOK Hook_zCViewPrint_BlitTextLine       PATCH( &zCViewPrint::BlitTextLine,       &zCViewBase::zCViewPrint_BlitTextLine );
  HOOK Hook_zCViewPrint_BlitTextCharacters PATCH( &zCViewPrint::BlitTextCharacters, &zCViewBase::zCViewPrint_BlitTextCharacters );
  HOOK Hook_zCView_BlitText                PATCH( &zCView::BlitText,                &zCViewBase::zCView_BlitText );



  
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



  int zCRnd_D3D::SetTextureStageState_Union( unsigned long stage, zTRnd_TextureStageState state, unsigned long value ) {
    ulong d3dTop = s_D3DStageLocked ?
      GetD3DTOP( zERenderObject::Interface ) :
      GetCurrentRenderObject();

    int ok = THISCALL( Ivk_zCRnd_D3D_SetTextureStageState )(stage, state, value);
    if( d3dTop != Invalid && !zKeyPressed( KEY_NUMPAD5 ) )
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