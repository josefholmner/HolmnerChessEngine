#pragma once

#include <cstdint>

using Square = uint8_t;
using Piece = uint8_t;
using Rank = uint8_t;
using File = uint8_t;
using SmallVal = uint8_t;
using Side = uint64_t;
using Val = uint64_t;
using BitBoard = uint64_t;

static constexpr Val BIT = 1;
static constexpr Side WHITE = ~1;
static constexpr Side BLACK = 1;
static constexpr Rank NUM_RANKS = 8;
static constexpr File NUM_FILES = 8;

static constexpr Val CAN_CASTLE_WK = 1;
static constexpr Val CAN_CASTLE_WQ = 1 << 1;
static constexpr Val CAN_CASTLE_BK = 1 << 2;
static constexpr Val CAN_CASTLE_BQ = 1 << 3;
static constexpr Val CAN_CASTLE_WKWQBKBQ = 
  CAN_CASTLE_WK | CAN_CASTLE_WQ | CAN_CASTLE_BK | CAN_CASTLE_BQ;

static constexpr Square A1 = 0;
static constexpr Square B1 = 1;
static constexpr Square C1 = 2;
static constexpr Square D1 = 3;
static constexpr Square E1 = 4;
static constexpr Square F1 = 5;
static constexpr Square G1 = 6;
static constexpr Square H1 = 7;

static constexpr Square A2 = 8;
static constexpr Square B2 = 9;
static constexpr Square C2 = 10;
static constexpr Square D2 = 11;
static constexpr Square E2 = 12;
static constexpr Square F2 = 13;
static constexpr Square G2 = 14;
static constexpr Square H2 = 15;

static constexpr Square A3 = 16;
static constexpr Square B3 = 17;
static constexpr Square C3 = 18;
static constexpr Square D3 = 19;
static constexpr Square E3 = 20;
static constexpr Square F3 = 21;
static constexpr Square G3 = 22;
static constexpr Square H3 = 23;

static constexpr Square A4 = 24;
static constexpr Square B4 = 25;
static constexpr Square C4 = 26;
static constexpr Square D4 = 27;
static constexpr Square E4 = 28;
static constexpr Square F4 = 29;
static constexpr Square G4 = 30;
static constexpr Square H4 = 31;

static constexpr Square A5 = 32;
static constexpr Square B5 = 33;
static constexpr Square C5 = 34;
static constexpr Square D5 = 35;
static constexpr Square E5 = 36;
static constexpr Square F5 = 37;
static constexpr Square G5 = 38;
static constexpr Square H5 = 39;

static constexpr Square A6 = 40;
static constexpr Square B6 = 41;
static constexpr Square C6 = 42;
static constexpr Square D6 = 43;
static constexpr Square E6 = 44;
static constexpr Square F6 = 45;
static constexpr Square G6 = 46;
static constexpr Square H6 = 47;

static constexpr Square A7 = 48;
static constexpr Square B7 = 49;
static constexpr Square C7 = 50;
static constexpr Square D7 = 51;
static constexpr Square E7 = 52;
static constexpr Square F7 = 53;
static constexpr Square G7 = 54;
static constexpr Square H7 = 55;

static constexpr Square A8 = 56;
static constexpr Square B8 = 57;
static constexpr Square C8 = 58;
static constexpr Square D8 = 59;
static constexpr Square E8 = 60;
static constexpr Square F8 = 61;
static constexpr Square G8 = 62;
static constexpr Square H8 = 63;

static constexpr Square NO_SQUARE = 64;
