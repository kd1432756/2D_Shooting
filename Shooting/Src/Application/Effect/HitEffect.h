#pragma once

#include "BaseEffect.h"

class HitEffect : public BaseEffect
{
public:

	HitEffect() {}
	virtual ~HitEffect() {}

	void Init();
	void Update();
	void Draw();
	void Release();

private:

};