#pragma once

#include <memory>

class Window;
class Drawable;
class Clickable;
struct ImageData;

class ThirdPartyWrappersFactory
{
public:
	static std::unique_ptr<Window> createWindow();

	static std::unique_ptr<Drawable> createDrawable(const ImageData& image);

	static std::unique_ptr<Clickable> createClickable(const ImageData& image);
};
