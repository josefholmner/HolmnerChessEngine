#include "ThirdPartyWrappers/SFMLDrawable.h"

#include "GuiUtilities.h"
#include "ImageData.h"

SFMLDrawable::SFMLDrawable(const ImageData& image)
{
	if (!texture.loadFromMemory(image.data, image.length))
	{
		GuiUtilities::logE("SFMLDrawable texture.loadFromMemory() failed.");
		return;
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
}

void SFMLDrawable::setRelativePosition(const Vec2<float>& relPos, const Vec2<uint32_t>& windowSize)
{
	sprite.setPosition(relPos.x() * (float)windowSize.x(),
		relPos.y() * (float)windowSize.y());
}

void SFMLDrawable::setScale(const Vec2<float>& scale)
{
	sprite.setScale(scale.x(), scale.y());
}

Vec2<float> SFMLDrawable::getScale() const
{
	const auto scale = sprite.getScale();
	return Vec2<float>(scale.x, scale.y);
}
