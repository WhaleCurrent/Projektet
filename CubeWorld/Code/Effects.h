#ifndef EFFECTS_H
#define EFFECTS_H

#include "stdafx.h"

namespace Effects
{
	extern ID3D10Effect* mModelFX;
	extern ID3D10Effect* mSkyFX;
	extern ID3D10Effect* mHudFX;
	extern ID3D10Effect* mFireFX;
	extern ID3D10Effect* mMoveFX;

 
	void InitAll(ID3D10Device* device);
};

#endif