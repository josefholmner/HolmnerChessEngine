#include "ThirdPartyWrappersFactory.h"

#include "ImageData.h"

#include "ThirdPartyWrappers/SFMLWindow.h"
#include "ThirdPartyWrappers/SFMLDrawable.h"
#include "ThirdPartyWrappers/SFMLClickable.h"

std::unique_ptr<Window> ThirdPartyWrappersFactory::createWindow()
{
	return std::make_unique<SFMLWindow>();
}

std::unique_ptr<Drawable> ThirdPartyWrappersFactory::createDrawable(const ImageData& image)
{
	return std::make_unique<SFMLDrawable>(image);
}

std::unique_ptr<Clickable> ThirdPartyWrappersFactory::createClickable(const ImageData& image)
{
	return std::make_unique<SFMLClickable>(image);
}
