#include "Text.h"

Text::Text(ID3D10Device* device)
{
	mDevice = device;
	D3DX10CreateFont(mDevice, 40, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &mFont );
	D3DX10CreateSprite(mDevice,512,&mFontSprite);
	D3DX10CreateFont(mDevice, 40, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times New Roman", &mFontTimes );
	D3DX10CreateSprite(mDevice,512,&mFontSpriteTimes);
	D3DX10CreateFont(mDevice, 200, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &mFontDead);
	D3DX10CreateSprite(mDevice,512,&mFontSpriteDead);
	D3DX10CreateFont(mDevice, 210, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &mFontOutline);
	D3DX10CreateSprite(mDevice,512,&mFontSpriteOutline);

	mNumber = 0;
	mTextLength = 1;
}

void Text::DrawInt(int number, RECT rect, D3DXCOLOR color)
{
	if(number >= 0 && number < 10)
		mTextLength = 1;
	if(number >= 10 && number < 100)
		mTextLength = 2;
	else if(number >= 100 && number < 1000)
		mTextLength = 3;
	else if(number >= 1000 && number < 10000)
		mTextLength = 4;
	else if(number >= 10000 && number < 100000)
		mTextLength = 5;

	_itow_s(number, mIstr, 10);

	mFontSprite->Begin( D3DX10_SPRITE_SORT_TEXTURE );

	mFont->DrawTextW(mFontSprite, mIstr, mTextLength, &rect, DT_LEFT, color);

	mFontSprite->End();
}

void Text::DrawString(const char* text, RECT rect, D3DXCOLOR color, int textLength, int type)
{
	if(type == 1)
	{
		mFontSpriteTimes->Begin( D3DX10_SPRITE_SORT_TEXTURE );

		mFontTimes->DrawText(mFontSpriteTimes, text, textLength, &rect, DT_CENTER, color);
		mFontSpriteTimes->End();
	}
	else if(type == 2)
	{
		mFontSpriteOutline->Begin( D3DX10_SPRITE_SORT_TEXTURE );
		
		RECT rec = rect;
		rec.top = rec.top - 5;
		D3DXCOLOR clr(0,0.3f,0,1);
		mFontOutline->DrawText(mFontSpriteOutline, text, textLength, &rec, DT_CENTER, clr);
		mFontSpriteOutline->End();

		mFontSpriteDead->Begin( D3DX10_SPRITE_SORT_TEXTURE );

		mFontDead->DrawText(mFontSpriteDead, text, textLength, &rect, DT_CENTER, color);
		mFontSpriteDead->End();
	}
	else
	{
		mFontSprite->Begin( D3DX10_SPRITE_SORT_TEXTURE );

		mFont->DrawText(mFontSprite, text, textLength, &rect, DT_CENTER, color);
		mFontSprite->End();
	}
}

void Text::DrawLevelHUD(int timeLeft, int points, int puzzlesLeft)
{
	RECT rc;
	rc.left = SCREEN_WIDTH - 80;
	rc.right = SCREEN_WIDTH; 
	rc.top = SCREEN_HEIGHT - 45;
	rc.bottom = SCREEN_HEIGHT;

	DrawInt(timeLeft, rc, D3DXCOLOR(1,1,1,1));

	rc.left = 350;
	rc.right = 620;

	for(int i = 0; i < puzzlesLeft; i++)
	{
		rc.left = 450 + i * 30;

		DrawString("I", rc, D3DXCOLOR(1,1,1,1), 1, 1);
	}

	rc.left = 90;
	rc.right = 250; 
	rc.top = SCREEN_HEIGHT - 45;
	rc.bottom = SCREEN_HEIGHT;

	DrawInt(points, rc, D3DXCOLOR(1,1,1,1));
}

void Text::DrawCenter(const char* text, int length)
{
	RECT rc;
	rc.left = 0;
	rc.right = SCREEN_WIDTH; 
	rc.top = SCREEN_HEIGHT / 2 - 150;
	rc.bottom = SCREEN_HEIGHT;

	DrawString(text, rc, D3DXCOLOR(0,0.4f,0,1), length, 2);
}

void Text::DrawCompleted()
{
	RECT rc;
	rc.left = 0;
	rc.right = SCREEN_WIDTH; 
	rc.top = SCREEN_HEIGHT / 2 - 300;
	rc.bottom = SCREEN_HEIGHT;

	DrawString("Level", rc, D3DXCOLOR(0,0.4f,0,1), 5, 2);

	rc.top = SCREEN_HEIGHT / 2 - 100;

	DrawString("completed!", rc, D3DXCOLOR(0,0.4f,0,1), 10, 2);
}