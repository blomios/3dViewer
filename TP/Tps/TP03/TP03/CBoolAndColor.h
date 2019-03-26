#pragma once
#include "Color.h"

using namespace std;
class CBoolAndColor
{
private:
	CColor col;
	bool mode;
public:
	CBoolAndColor(CColor c, bool b);
	void setColorHSV(float fH, float fS, float fV) { col.SetHSV(fH, fS, fV); }
	void setMode() { mode = !mode; }
	CColor getColor() { return col; }
	bool getMode() { return mode; }
};