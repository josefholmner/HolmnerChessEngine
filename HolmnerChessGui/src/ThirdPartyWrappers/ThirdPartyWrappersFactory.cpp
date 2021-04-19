#include "ThirdPartyWrappersFactory.h"

#include "ResourceData.h"

#include "ThirdPartyWrappers/SFMLWindow.h"
#include "ThirdPartyWrappers/SFMLDrawable.h"
#include "ThirdPartyWrappers/SFMLClickable.h"
#include "ThirdPartyWrappers/SFMLText.h"

std::unique_ptr<Window> ThirdPartyWrappersFactory::createWindow()
{
	return std::make_unique<SFMLWindow>();
}

std::unique_ptr<Drawable> ThirdPartyWrappersFactory::createDrawable(const ResourceData& image)
{
	return std::make_unique<SFMLDrawable>(image);
}

std::unique_ptr<Clickable> ThirdPartyWrappersFactory::createClickable(const ResourceData& image)
{
	return std::make_unique<SFMLClickable>(image);
}

std::unique_ptr<Text> ThirdPartyWrappersFactory::createText(const std::string& str,
	const ResourceData& font, uint32_t sizeInPixels)
{
	return std::make_unique<SFMLText>(str, font, sizeInPixels);
}
