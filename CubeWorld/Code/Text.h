#ifndef TEXT_H
#define TEXT_H

#include "stdafx.h"

class Text
{
private:
	LPD3DX10FONT mFont;
	LPD3DX10SPRITE mFontSprite;
	LPD3DX10FONT mFontTimes;
	LPD3DX10SPRITE mFontSpriteTimes;
	LPD3DX10FONT mFontDead;
	LPD3DX10SPRITE mFontSpriteDead;
	LPD3DX10FONT mFontOutline;
	LPD3DX10SPRITE mFontSpriteOutline;

	wchar_t mIstr[32];
	int mTextLength;
	int mNumber;
	ID3D10Device* mDevice;

public:
	Text(ID3D10Device* device);
	void DrawInt(int text, RECT rect, D3DXCOLOR color);
	void DrawString(const char* text, RECT rect, D3DXCOLOR color, int textLength, int type = 0);
	void DrawLevelSelect(int levelLe, int levelNo);
	void DrawLevelHUD(int timeLeft, int points, int puzzlesLeft);
	void DrawCenter(const char* text, int length);
	void DrawCompleted();
};

#endif


