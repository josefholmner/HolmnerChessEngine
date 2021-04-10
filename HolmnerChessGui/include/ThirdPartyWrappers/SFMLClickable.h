#pragma once

#include "Clickable.h"
#include "SFMLDrawable.h"

class SFMLClickable : public Clickable
{
public:
	SFMLClickable(const ImageData& image);

	bool wasClicked(const Vec2<int32_t>& mousePos, const Window& window) override;

	Drawable& getDrawable() override;

private:
	SFMLDrawable drawable;
};
