#include "PrivateInclude/BoardState.h"

#include "PrivateInclude/EngineUtilities.h"
#include "Common/CommonUtilities.h"

#include <iostream>

namespace
{
	std::optional<Square> fromString(const std::string& str)
	{
		if (str == "-")
		{
			return squares::none;
		}

		if (str.size() != 2)
		{
			EngineUtilities::logE("Invalid square in FEN: " + str);
			return {};
		}

		Rank rank = static_cast<Rank>(str[1] - '1');
		File file = static_cast<File>(str[0] - 'a');

		if (rank < 0 || rank > 7 || file < 0 || file > 7)
		{
			EngineUtilities::logE("Invalid square in FEN: " + str);
			return {};
		}

		return file + 8 * rank;
	}

	std::optional<Square> addPiece(char c, Square cursor, std::array<Piece, squares::num>& pieces)
	{
		if (cursor > squares::num)
		{
			EngineUtilities::logE("Invalid FEN, it represents more than 64 squares.");
			return {};
		}

		switch (c)
		{
		case 'r':
			pieces[cursor++] = pieces::bR;
			return cursor;
		case 'n':
			pieces[cursor++] = pieces::bN;
			return cursor;
		case 'b':
			pieces[cursor++] = pieces::bB;
			return cursor;
		case 'k':
			pieces[cursor++] = pieces::bK;
			return cursor;
		case 'q':
			pieces[cursor++] = pieces::bQ;
			return cursor;
		case 'p':
			pieces[cursor++] = pieces::bP;
			return cursor;
		case 'R':
			pieces[cursor++] = pieces::wR;
			return cursor;
		case 'N':
			pieces[cursor++] = pieces::wN;
			return cursor;
		case 'B':
			pieces[cursor++] = pieces::wB;
			return cursor;
		case 'K':
			pieces[cursor++] = pieces::wK;
			return cursor;
		case 'Q':
			pieces[cursor++] = pieces::wQ;
			return cursor;
		case 'P':
			pieces[cursor++] = pieces::wP;
			return cursor;
		default:
			if (c > '0')
			{
				pieces[cursor++] = pieces::none;
				return addPiece(c - 1, cursor, pieces);
			}

			return cursor;
		}
	}

	std::optional<std::array<Piece, squares::num>> getPiecesFromFEN(const std::string& str)
	{
		const std::vector<std::string>& ranks = CommonUtilities::splitString(str, '/');
		if (ranks.size() != 8)
		{
			EngineUtilities::logE("Wrong number of ranks in FEN.");
			return {};
		}

		std::array<Piece, squares::num> pieces;
		Square cursor = squares::a1;
		for (Rank rank = 7; rank >= 0; rank--)
		{
			for (const char& c : ranks[rank])
			{
				if (auto newCursor = addPiece(c, cursor, pieces))
				{
					cursor = *newCursor;
				}
				else
				{
					// Logging done in addPiece.
					return {};
				}
			}
		}

		if (cursor != squares::num)
		{
			EngineUtilities::logE("Too few squares in FEN.");
			return {};
		}

		return pieces;
	}

	std::optional<pieces::Color> getTurnFromFEN(const std::string& str)
	{
		if (str == "w")
		{
			return pieces::Color::WHITE;
		}
		else if (str == "b")
		{
			return pieces::Color::BLACK;
		}

		EngineUtilities::logE("Invalid character for representing side to play in FEN");
		return {};
	}

	std::optional<Square> getEnPassantSquarefromStr(const std::string& str,
		std::array<Piece, squares::num> pieces)
	{
		if (str == "-")
		{
			return squares::none;
		}

		const auto eSq = fromString(str);
		if (!eSq)
		{
			EngineUtilities::logE("En passant square in FEN is invalid: " + str);
			return {};
		}

		// Ensure the en passant square is at a valid rank.
		const Rank eSqRank = ranks::toRank(*eSq);
		if (eSqRank != 2 && eSqRank != 5)
		{
			EngineUtilities::logE("En passant square in FEN is invalid: " + str);
			return {};
		}

		// Ensure there is actually a pawn in front of the en passant square.
		if (eSqRank == 2 && pieces[(size_t)*eSq + (size_t)8] != pieces::wP)
		{
			EngineUtilities::logE("No pawn in front of en passant square. Invalid FEN.");
			return {};
		}

		if (eSqRank == 5 && pieces[(size_t)*eSq - (size_t)8] != pieces::bP)
		{
			EngineUtilities::logE("No pawn in front of en passant square. Invalid FEN.");
			return {};
		}

		return *eSq;
	}

	std::optional<std::unordered_map<char, bool>> getCastleAvailabilityFromFEN(const std::string& str)
	{
		if (str.size() < 1)
		{
			EngineUtilities::logE("Invalid castling availability in FEN: " + str);
			return {};
		}


		std::unordered_map<char, bool> castle{{'K', false}, { 'Q', false }, { 'k', false }, { 'q', false }};
		if (str[0] == '-')
		{
			return castle;
		}

		for (const char c : str)
		{
			switch (c)
			{
				case 'Q':
					castle[c] = true;
					break;
				case 'K':
					castle[c] = true;
					break;
				case 'q':
					castle[c] = true;
					break;
				case 'k':
					castle[c] = true;
					break;
				default:
					EngineUtilities::logE("Invalid char in castling availability in FEN: " + c);
					return {};
			}
		}

		return castle;
	}
}

bool BoardState::initFromFEN(const std::string& FEN)
{
	const std::vector<std::string> splitFEN = CommonUtilities::splitString(FEN, ' ');

	// All FENs should be exactly 6 segments long.
	if (splitFEN.size() != 6)
	{
		EngineUtilities::logE("initFromFEN failed: Invalid FEN.");
		return false;
	}

	// Set pieces.
	if (const auto pi = getPiecesFromFEN(splitFEN[0]))
	{
		pieces = *pi;
	}
	else
	{
		return false;
	}

	// Set turn.
	if (const auto t = getTurnFromFEN(splitFEN[1]))
	{
		turn = *t;
	}
	else
	{
		return false;
	}

	// Set castling availability.
	if (const auto castle = getCastleAvailabilityFromFEN(splitFEN[2]))
	{
		casleAvailability = *castle;
	}
	else
	{
		return false;
	}

	// Set en passant square.
	if (auto eSq = getEnPassantSquarefromStr(splitFEN[3], pieces))
	{
		enPassantSquare = *eSq;
	}
	else
	{
		return false;
	}

	return true;
}

void BoardState::printBoard() const
{
	for (Rank rank = 7; rank >= 0; rank--)
	{
		for (File file = 0; file < 8; file++)
		{
			const int32_t index = file + rank * 8;
			std::cout << "[" << pieces::pieceToStr(pieces[index]) << "] ";
		}

		std::cout << std::endl;
	}
}

void BoardState::makeMove(const Move& move)
{
	using namespace pieces;

	switch (move.movingPiece)
	{
		case wP:
		case bP:
			makePawnMove(move);
			return;
		case wR:
		case bR:
			makeRookMove(move);
			return;
		case wK:
		case bK:
			makeKingMove(move);
			return;
	}
}

void BoardState::unmakeMove(const Move& move)
{
	using namespace pieces;

	switch (move.movingPiece)
	{
	case wP:
	case bP:
		unmakePawnMove(move);
		return;
	case wR:
	case bR:
		unmakeRookMove(move);
		return;
	case wK:
	case bK:
		unmakeKingMove(move);
		return;
	}
}

void BoardState::makePawnMove(const Move& move)
{
}

void BoardState::unmakePawnMove(const Move& move)
{
}

void BoardState::makeRookMove(const Move& move)
{
}

void BoardState::unmakeRookMove(const Move& move)
{
}

void BoardState::makeKingMove(const Move& move)
{
}

void BoardState::unmakeKingMove(const Move& move)
{
}
