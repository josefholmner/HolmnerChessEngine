#pragma once

#include <cstdint>
#include <string>

typedef int32_t Piece;
namespace pieces
{
	constexpr Piece none = 0;
	constexpr Piece wK = 1;
	constexpr Piece wQ = 2;
	constexpr Piece wR = 3;
	constexpr Piece wB = 4;
	constexpr Piece wN = 5;
	constexpr Piece wP = 6;

	constexpr Piece bK = 7;
	constexpr Piece bQ = 8;
	constexpr Piece bR = 9;
	constexpr Piece bB = 10;
	constexpr Piece bN = 11;
	constexpr Piece bP = 12;

	std::string pieceToStr(Piece piece);
}

typedef int32_t Square;
namespace squares
{
	constexpr int32_t num = 64;

	constexpr Square a1 = 0;
	constexpr Square a2 = 1;
	constexpr Square a3 = 2;
	constexpr Square a4 = 3;
	constexpr Square a5 = 4;
	constexpr Square a6 = 5;
	constexpr Square a7 = 6;
	constexpr Square a8 = 7;

	constexpr Square b1 = 8;
	constexpr Square b2 = 9;
	constexpr Square b3 = 10;
	constexpr Square b4 = 11;
	constexpr Square b5 = 12;
	constexpr Square b6 = 13;
	constexpr Square b7 = 14;
	constexpr Square b8 = 15;

	constexpr Square c1 = 16;
	constexpr Square c2 = 17;
	constexpr Square c3 = 18;
	constexpr Square c4 = 19;
	constexpr Square c5 = 20;
	constexpr Square c6 = 21;
	constexpr Square c7 = 22;
	constexpr Square c8 = 23;

	constexpr Square d1 = 24;
	constexpr Square d2 = 25;
	constexpr Square d3 = 26;
	constexpr Square d4 = 27;
	constexpr Square d5 = 28;
	constexpr Square d6 = 29;
	constexpr Square d7 = 30;
	constexpr Square d8 = 31;

	constexpr Square e1 = 32;
	constexpr Square e2 = 33;
	constexpr Square e3 = 34;
	constexpr Square e4 = 35;
	constexpr Square e5 = 36;
	constexpr Square e6 = 37;
	constexpr Square e7 = 38;
	constexpr Square e8 = 39;

	constexpr Square f1 = 40;
	constexpr Square f2 = 41;
	constexpr Square f3 = 42;
	constexpr Square f4 = 43;
	constexpr Square f5 = 44;
	constexpr Square f6 = 45;
	constexpr Square f7 = 46;
	constexpr Square f8 = 47;

	constexpr Square g1 = 48;
	constexpr Square g2 = 49;
	constexpr Square g3 = 50;
	constexpr Square g4 = 51;
	constexpr Square g5 = 52;
	constexpr Square g6 = 53;
	constexpr Square g7 = 54;
	constexpr Square g8 = 55;

	constexpr Square h1 = 56;
	constexpr Square h2 = 57;
	constexpr Square h3 = 58;
	constexpr Square h4 = 59;
	constexpr Square h5 = 60;
	constexpr Square h6 = 61;
	constexpr Square h7 = 62;
	constexpr Square h8 = 63;
}
