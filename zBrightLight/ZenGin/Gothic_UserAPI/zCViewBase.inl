// Supported with union (c) 2020 Union team

// User API for zCViewBase
// Add your methods here

void __fastcall oCViewDialogItem_Blit_Union();
void __fastcall zCViewDraw_Blit_Union();
void __fastcall zCViewFX_Blit_Union();
void __fastcall zCViewPrint_Blit_Union();
void zCView_Blit_Union();

void __fastcall zCViewDialogChoice_BlitText();
void __fastcall zCViewDraw_BlitTexture();
void __fastcall zCViewPrint_BlitText();
void __fastcall zCViewPrint_BlitTextLine( zCViewText2* line );
void __fastcall zCViewPrint_BlitTextCharacters( zCViewText2* line, zCFont* font, zCOLOR& color );
void zCView_BlitText();