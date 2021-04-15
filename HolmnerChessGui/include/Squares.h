#pragma once

#include <cstdint>
#include <string>

typedef int32_t Square;
namespace squares
{
	static constexpr Square num = 64;

	static constexpr Square none = -1;
	static constexpr Square a1 = 0;
	static constexpr Square b1 = 1;
	static constexpr Square c1 = 2;
	static constexpr Square d1 = 3;
	static constexpr Square e1 = 4;
	static constexpr Square f1 = 5;
	static constexpr Square g1 = 6;
	static constexpr Square h1 = 7;

	static constexpr Square a2 = 8;
	static constexpr Square b2 = 9;
	static constexpr Square c2 = 10;
	static constexpr Square d2 = 11;
	static constexpr Square e2 = 12;
	static constexpr Square f2 = 13;
	static constexpr Square g2 = 14;
	static constexpr Square h2 = 15;

	static constexpr Square a3 = 16;
	static constexpr Square b3 = 17;
	static constexpr Square c3 = 18;
	static constexpr Square d3 = 19;
	static constexpr Square e3 = 20;
	static constexpr Square f3 = 21;
	static constexpr Square g3 = 22;
	static constexpr Square h3 = 23;

	static constexpr Square a4 = 24;
	static constexpr Square b4 = 25;
	static constexpr Square c4 = 26;
	static constexpr Square d4 = 27;
	static constexpr Square e4 = 28;
	static constexpr Square f4 = 29;
	static constexpr Square g4 = 30;
	static constexpr Square h4 = 31;

	static constexpr Square a5 = 32;
	static constexpr Square b5 = 33;
	static constexpr Square c5 = 34;
	static constexpr Square d5 = 35;
	static constexpr Square e5 = 36;
	static constexpr Square f5 = 37;
	static constexpr Square g5 = 38;
	static constexpr Square h5 = 39;

	static constexpr Square a6 = 40;
	static constexpr Square b6 = 41;
	static constexpr Square c6 = 42;
	static constexpr Square d6 = 43;
	static constexpr Square e6 = 44;
	static constexpr Square f6 = 45;
	static constexpr Square g6 = 46;
	static constexpr Square h6 = 47;

	static constexpr Square a7 = 48;
	static constexpr Square b7 = 49;
	static constexpr Square c7 = 50;
	static constexpr Square d7 = 51;
	static constexpr Square e7 = 52;
	static constexpr Square f7 = 53;
	static constexpr Square g7 = 54;
	static constexpr Square h7 = 55;

	static constexpr Square a8 = 56;
	static constexpr Square b8 = 57;
	static constexpr Square c8 = 58;
	static constexpr Square d8 = 59;
	static constexpr Square e8 = 60;
	static constexpr Square f8 = 61;
	static constexpr Square g8 = 62;
	static constexpr Square h8 = 63;

	std::string toString(Square sq);
	Square fromString(const std::string& sq);
}
