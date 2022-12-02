#pragma once

#include "Framebuffer.h"
#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float4.h"


class MenuViewport
{
public:
	MenuViewport();

	~MenuViewport();

	void Draw(Framebuffer* framebuffer, int currentOperation);

	inline float4 GetBounds() { return bounds; }
	inline const bool GetState() const { return selected; }

private:
	float2 sizeViewport;
	float4 bounds;
	bool active;

	bool selected;
};