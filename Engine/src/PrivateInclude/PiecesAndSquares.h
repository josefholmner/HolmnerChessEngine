#pragma once

#include <cstdint>
#include <string>

// TODO replace all instances of 0, 7 or similar with constants, e.g.
// rank::max or similar thought the code base.

typedef int32_t Piece;
namespace pieces
{
	constexpr Piece none = -1;
	constexpr Piece wK = 0;
	constexpr Piece wQ = 1;
	constexpr Piece wR = 2;
	constexpr Piece wB = 3;
	constexpr Piece wN = 4;
	constexpr Piece wP = 5;

	constexpr Piece bK = 6;
	constexpr Piece bQ = 7;
	constexpr Piece bR = 8;
	constexpr Piece bB = 9;
	constexpr Piece bN = 10;
	constexpr Piece bP = 11;

	std::string pieceToStr(Piece piece);

	enum class Color
	{
		WHITE,
		BLACK
	};
}

typedef int32_t Square;
namespace squares
{
	constexpr int32_t num = 64;

	constexpr Square none = -1;
	constexpr Square a1 = 0;
	constexpr Square b1 = 1;
	constexpr Square c1 = 2;
	constexpr Square d1 = 3;
	constexpr Square e1 = 4;
	constexpr Square f1 = 5;
	constexpr Square g1 = 6;
	constexpr Square h1 = 7;

	constexpr Square a2 = 8;
	constexpr Square b2 = 9;
	constexpr Square c2 = 10;
	constexpr Square d2 = 11;
	constexpr Square e2 = 12;
	constexpr Square f2 = 13;
	constexpr Square g2 = 14;
	constexpr Square h2 = 15;

	constexpr Square a3 = 16;
	constexpr Square b3 = 17;
	constexpr Square c3 = 18;
	constexpr Square d3 = 19;
	constexpr Square e3 = 20;
	constexpr Square f3 = 21;
	constexpr Square g3 = 22;
	constexpr Square h3 = 23;

	constexpr Square a4 = 24;
	constexpr Square b4 = 25;
	constexpr Square c4 = 26;
	constexpr Square d4 = 27;
	constexpr Square e4 = 28;
	constexpr Square f4 = 29;
	constexpr Square g4 = 30;
	constexpr Square h4 = 31;

	constexpr Square a5 = 32;
	constexpr Square b5 = 33;
	constexpr Square c5 = 34;
	constexpr Square d5 = 35;
	constexpr Square e5 = 36;
	constexpr Square f5 = 37;
	constexpr Square g5 = 38;
	constexpr Square h5 = 39;

	constexpr Square a6 = 40;
	constexpr Square b6 = 41;
	constexpr Square c6 = 42;
	constexpr Square d6 = 43;
	constexpr Square e6 = 44;
	constexpr Square f6 = 45;
	constexpr Square g6 = 46;
	constexpr Square h6 = 47;

	constexpr Square a7 = 48;
	constexpr Square b7 = 49;
	constexpr Square c7 = 50;
	constexpr Square d7 = 51;
	constexpr Square e7 = 52;
	constexpr Square f7 = 53;
	constexpr Square g7 = 54;
	constexpr Square h7 = 55;

	constexpr Square a8 = 56;
	constexpr Square b8 = 57;
	constexpr Square c8 = 58;
	constexpr Square d8 = 59;
	constexpr Square e8 = 60;
	constexpr Square f8 = 61;
	constexpr Square g8 = 62;
	constexpr Square h8 = 63;
}

typedef int32_t Rank;
namespace ranks
{
	Rank toRank(Square sq);
}

typedef int32_t File;
namespace files
{
	File toFile(Square sq);
}
