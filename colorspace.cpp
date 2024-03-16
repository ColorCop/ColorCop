
#include "stdafx.h"
#include "colorspace.h"


class colorspace
{
protected:


public:

	// Red
	BYTE GetRed() { return byRed; }
	void SetRed(BYTE r) { byRed = r; }

	// Green
	BYTE GetGreen() { return byGreen; }
	void SetGreen(BYTE g) { byGreen = g;}
	
	// Blue
	BYTE GetBlue() { return byBlue; }
	void SetBlue(BYTE b) { byBlue = b; }

	// Cyan
	BYTE GetCyan() { return byCyan; }
	void SetCyan(BYTE c) { byCyan = c; }

	// Magenta
	BYTE GetMagenta() { return byCyan; }
	void SetMagenta(BYTE c) { byCyan = c; }

	// Yellow
	BYTE GetYellow() { return byCyan; }
	void SetYellow(BYTE c) { byCyan = c; }

	// blacK
	BYTE GetBlack() { return byCyan; }
	void SetBlack(BYTE c) { byCyan = c; }

	// Hue
	BYTE GetHue() { return byCyan; }
	void SetHue(BYTE c) { byCyan = c; }

	// Saturation
	BYTE GetSaturation() { return byCyan; }
	void SetSaturation(BYTE c) { byCyan = c; }

	// Brightness
	BYTE GetBrightness() { return byCyan; }
	void SetBrightness(BYTE c) { byCyan = c; }

	// ctor
	colorspace() 
	{


	}


///////////////////////////////////////////////////////////////////////////////
//
// The algorithms for these routines were taken from:
//     http://www.neuro.sfc.keio.ac.jp/~aly/polygon/info/color-space-faq.html
//
// RGB --> CMYK                              CMYK --> RGB
// ---------------------------------------   --------------------------------------------
// Black   = minimum(1-Red,1-Green,1-Blue)   Red   = 1-minimum(1,Cyan*(1-Black)+Black)
// Cyan    = (1-Red-Black)/(1-Black)         Green = 1-minimum(1,Magenta*(1-Black)+Black)
// Magenta = (1-Green-Black)/(1-Black)       Blue  = 1-minimum(1,Yellow*(1-Black)+Black)
// Yellow  = (1-Blue-Black)/(1-Black)
//


///////////////////////////////////////////////////////////////////////////////
// RGB2CMYK
void RGB2CMYK(BYTE r, BYTE g, BYTE b, BYTE& c, BYTE& m, BYTE& y, BYTE& k)
{
	double R, G, B;
	R = (double) r;
	G = (double) g;
	B = (double) b;

	R = 1.0 - (R / 255.0);
	G = 1.0 - (G / 255.0);
	B = 1.0 - (B / 255.0);

	double C, M, Y, K;
	if (R < G)
		K = R;
	else
		K = G;
	if (B < K)
		K = B;

	C = (R - K)/(1.0 - K);
	M = (G - K)/(1.0 - K);
	Y = (B - K)/(1.0 - K);

	C = (C * 100) + 0.5;
	M = (M * 100) + 0.5;
	Y = (Y * 100) + 0.5;
	K = (K * 100) + 0.5;

	c = (BYTE) C;
	m = (BYTE) M;
	y = (BYTE) Y;
	k = (BYTE) K;
}

///////////////////////////////////////////////////////////////////////////////
// CMYK2RGB
COLORREF CMYK2RGB(BYTE c, BYTE m, BYTE y, BYTE k)
{
	BYTE r, g, b;
	COLORREF rgb;

	double R, G, B;
	double C, M, Y, K;

	C = (double) c;
	M = (double) m;
	Y = (double) y;
	K = (double) k;

	C = C / 255.0;
	M = M / 255.0;
	Y = Y / 255.0;
	K = K / 255.0;

	R = C * (1.0 - K) + K;
	G = M * (1.0 - K) + K;
	B = Y * (1.0 - K) + K;

	R = (1.0 - R) * 255.0 + 0.5;
	G = (1.0 - G) * 255.0 + 0.5;
	B = (1.0 - B) * 255.0 + 0.5;

	r = (BYTE) R;
	g = (BYTE) G;
	b = (BYTE) B;

	rgb = RGB(r,g,b);

	return rgb;
}



};
