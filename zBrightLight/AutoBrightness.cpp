// Supported with union (c) 2020 Union team
// Union SOURCE file
#include <gdiplus.h>

namespace GOTHIC_ENGINE {

  float SmoothIncrease( const float& value, const float& max, const uint& strength = 1 ) {
    static float limit = PI / 2.0f;
    float multiplier = limit / max;
    float result = value;

    for( uint i = 0; i < strength; i++ )
      result = (cos( result * multiplier + PI ) + 1.0f) * max;

    return result;
  }

// TODO
#if 0
  zCOLOR GetPixelDC( HDC hdc, const int& x, const int& y ) {
    return GetPixel( hdc, x, y );
  }

  int GetAverageLight( HDC hdc ) {
#if 0
    float strength = 0;
    int count      = 0;
    int sx         = zrenderer->vid_xdim;
    int sy         = zrenderer->vid_ydim;
    int midx       = sx / 2;
    int midy       = sy / 2;
    int wx         = sx / 2;
    int wy         = sy / 2;
    int stepx      = wx / 4;
    int stepy      = wy / 4;
    int startx     = midx - wx / 2;
    int starty     = midy - wy / 2;

    for( int y = 0; y < wy; y += stepy ) {
      for( int x = 0; x < wx; x += stepx ) {
        int px = startx + x;
        int py = starty + y;
        zCOLOR color = GetPixelDC( hdc, px, py );
        int brightness = (color.r + color.g + color.b) / 3;
        strength += SmoothIncrease( brightness, 255.0f );
        count++;
      }
    }

    return strength / count;
#else

    int sx = zrenderer->vid_xdim;
    int sy = zrenderer->vid_ydim;
    zCOLOR color = GetPixelDC( hdc, sx / 2, sy / 3 );
    return (color.r + color.g + color.b) / 3;
#endif
  }

  int& playerLightInt = *(int*)0x0089EBB4;

  int GetScreenBrightness() {
    zCRnd_D3D* d3d = (zCRnd_D3D*)zrenderer;// dynamic_cast<zCRnd_D3D*>(zrenderer);
    if( d3d == Null )
      return 255;

    HDC hdc = Null;
    d3d->xd3d_pfrontBuffer->GetDC( &hdc );
    int light = GetAverageLight( hdc );
    d3d->xd3d_pfrontBuffer->ReleaseDC( hdc );
    return light;
  }

  // dword_99ADA4
  DWORD& dword_99ADA4 = *(DWORD*)0x0099ADA4;

	void UpdateBrightness() {
    return;
    // pass

		if( IndoorMode )
			return;

    playerLightInt = 50;

    int brightness = GetScreenBrightness();
    cmd << brightness << "    " << Opt_Brightness << endl;

    if( brightness > 10 ) {
      Opt_Brightness -= ztimer->frameTimeFloat * 0.0001f;
      if( Opt_Brightness < 0.0f )
        Opt_Brightness = 0.0f;
    }
    else if( Opt_Brightness < 1.0f ) {
      Opt_Brightness += ztimer->frameTimeFloat * 0.0001f;
      if( Opt_Brightness > 1.0f )
        Opt_Brightness = 1.0f;
    }
	}
#endif






	HOOK Hook_zCSkyControler_Outdoor_CalcPolyLightCLUT PATCH_IF( &zCSkyControler_Outdoor::CalcPolyLightCLUT, &zCSkyControler_Outdoor::CalcPolyLightCLUT_Union, false );

  zVEC3 SmoothIncreaseVector( zVEC3 vector ) {
    float minLight  = 000.0f;
    float maxLight  = 400.0f;
    float diffLight = maxLight - minLight;

    float legnth = vector.Length();
    float light = legnth - minLight;
    if( light < 0.0f )
      light = 0.0f;

    vector.Normalize();
    float multiplier = SmoothIncrease( min( light, diffLight ), diffLight, Opt_NightMode );
    return vector * multiplier;
  }

	void zCSkyControler_Outdoor::CalcPolyLightCLUT_Union( const zVEC3& col0_src, const zVEC3& col1_src ) {
    zVEC3 col0;
    zVEC3 col1;
    if( Opt_NightMode > 0 ) {
      col0 = SmoothIncreaseVector( col0_src ) * Opt_Brightness;
      col1 = SmoothIncreaseVector( col1_src ) * Opt_Brightness;
    }
    else {
      col0 = col0_src * Opt_Brightness;
      col1 = col1_src * Opt_Brightness;
    }

		THISCALL( Hook_zCSkyControler_Outdoor_CalcPolyLightCLUT )(col0, col1);
	}
}