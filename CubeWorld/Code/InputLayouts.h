#ifndef INPUTLAYOUTS_H
#define INPUTLAYOUTS_H

#include "stdafx.h"

namespace InputLayout
{
	extern ID3D10InputLayout* mSky;
	extern ID3D10InputLayout* mModel;
	extern ID3D10InputLayout* mHud;
	extern ID3D10InputLayout* mParticleFire;
	extern ID3D10InputLayout* mParticleMove;

	void InitAll(ID3D10Device* device);
};

#endif