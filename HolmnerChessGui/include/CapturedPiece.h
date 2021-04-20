#pragma once

#include "Piece.h"

#include <cstdint>

class CapturedPiece : public Piece
{
public:
	CapturedPiece() = default;
	CapturedPiece(const CapturedPiece& other);

	void init(Type type, const Vec2<float>& inScale);

	bool operator<(const CapturedPiece& other) const;
	CapturedPiece& operator=(const CapturedPiece& other);

protected:
	int32_t getOrderValue() const {	return orderValue; }

private:
	int32_t orderValue = 0;
};
