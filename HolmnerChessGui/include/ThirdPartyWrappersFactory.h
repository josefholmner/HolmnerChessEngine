#pragma once

#include <memory>

class Window;


class ThirdPartyWrappersFactory
{
public:
	static std::unique_ptr<Window> createWindow();
};
