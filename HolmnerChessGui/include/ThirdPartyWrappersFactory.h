#pragma once

#include <memory>
#include <string>
#include <cstdint>

class Window;
class Drawable;
class Clickable;
class Text;
struct ResourceData;

class ThirdPartyWrappersFactory
{
public:
	static std::unique_ptr<Window> createWindow();

	static std::unique_ptr<Drawable> createDrawable(const ResourceData& image);

	static std::unique_ptr<Clickable> createClickable(const ResourceData& image);

	static std::unique_ptr<Text> createText(const std::string& str,
		const ResourceData& font, uint32_t sizeInPixels);
};
