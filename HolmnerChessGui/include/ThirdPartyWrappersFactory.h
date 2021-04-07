#pragma once

#include <memory>

class Window;
class Drawable;
struct ImageData;

class ThirdPartyWrappersFactory
{
public:
	static std::unique_ptr<Window> createWindow();

	static std::unique_ptr<Drawable> createDrawable(const ImageData& image);
};
