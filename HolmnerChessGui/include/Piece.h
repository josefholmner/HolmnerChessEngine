#pragma once

#include "Vec2.h"

#include <memory>
#include <cstdint>

class Drawable;
class Window;
struct ImageData;

class Piece
{
public:
	Piece() = default;
	~Piece() = default;

	enum class Type
	{
		None,
		WKing,
		WQueen,
		WRook,
		WBishop,
		WKnight,
		WPawn,
		BKing,
		BQueen,
		BRook,
		BBishop,
		BKnight,
		BPawn
	};

	void init(Type type, const Vec2<float>& scale);

	void draw(Window& window, const Vec2<int32_t>& mousePos);

	// Also updates the drawable according to the type.
	void setType(Type inType);
	Type getType() const { return type; }

	void setNormalizedPosition(const Vec2<float>& normPos, const Window& window);
	Vec2<float> getNormalizedPosition(const Window& window) const;

	void setIsMouseDragged(bool inIsMouseDragged);
	bool getIsMouseDragged() { return isMouseDragged; }

private:
	Type type = Type::None;
	bool isMouseDragged = false;
	std::unique_ptr<Drawable> drawable;
};
