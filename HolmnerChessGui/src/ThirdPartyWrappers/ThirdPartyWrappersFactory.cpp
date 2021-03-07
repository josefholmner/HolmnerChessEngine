#include "ThirdPartyWrappersFactory.h"

#include "ThirdPartyWrappers/SFMLWindow.h"

std::unique_ptr<Window> ThirdPartyWrappersFactory::createWindow()
{
	return std::make_unique<SFMLWindow>();
}
