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

	void handleCastelingFlagsMakeMove(const Move& move, BoardState& board)
	{
		if (move.prohibitsWKcastling)
		{
			board.getCastleAvailability()['K'] = false;
		}
		if (move.prohibitsWQcastling)
		{
			board.getCastleAvailability()['Q'] = false;
		}
		if (move.prohibitsBKcastling)
		{
			board.getCastleAvailability()['k'] = false;
		}
		if (move.prohibitsBQcastling)
		{
			board.getCastleAvailability()['q'] = false;
		}
	}

	void handleCastelingFlagsUnmakeMove(const Move& move, BoardState& board)
	{
		
		if (move.prohibitsWKcastling)
		{
			board.getCastleAvailability()['K'] = true;
		}
		if (move.prohibitsWQcastling)
		{
			board.getCastleAvailability()['Q'] = true;
		}
		if (move.prohibitsBKcastling)
		{
			board.getCastleAvailability()['k'] = true;
		}
		if (move.prohibitsBQcastling)
		{
			board.getCastleAvailability()['q'] = true;
		}
	}

	bool isEnPassantSqValid(const BoardState& board)
	{
		const Square eSq = board.getEnPassantSquare();
		if (eSq == squares::none)
		{
			return true;
		}

		const Rank eRank = ranks::toRank(eSq);
		if (eRank == 2)
		{
			if (board.getPiece(eSq + 8) == pieces::wP && board.getTurn() == pieces::Color::BLACK)
			{
				return true;
			}
		}
		else if (eRank == 5)
		{
			if (board.getPiece(eSq - 8) == pieces::bP && board.getTurn() == pieces::Color::WHITE)
			{
				return true;
			}
		}

		return false;
	}

	bool isPieceCountValid(const BoardState& board)
	{
		int32_t wPs = 0, wNs = 0, wBs = 0, wRs = 0, wQs = 0, wKs = 0;
		int32_t bPs = 0, bNs = 0, bBs = 0, bRs = 0, bQs = 0, bKs = 0;
		for (const auto piece : board.getPieces())
		{
			switch (piece)
			{
				case pieces::wP:
					wPs++;
					break;
				case pieces::wN:
					wNs++;
					break;
				case pieces::wB:
					wBs++;
					break;
				case pieces::wR:
					wRs++;
					break;
				case pieces::wQ:
					wQs++;
					break;
				case pieces::wK:
					wKs++;
					break;
				case pieces::bP:
					bPs++;
					break;
				case pieces::bN:
					bNs++;
					break;
				case pieces::bB:
					bBs++;
					break;
				case pieces::bR:
					bRs++;
					break;
				case pieces::bQ:
					bQs++;
					break;
				case pieces::bK:
					bKs++;
					break;
				case pieces::none:
					// Do nothing.
					break;
				default:
					return false; // Unknown piece.
			}
		}

		if (wKs == 1 && bKs == 1 && wQs <= 9 && bQs <= 9 && wRs <= 10 && bRs <= 10 && wBs <= 10 && bBs <= 10
			&& wNs <= 10 && bNs <= 10 && wPs <= 8 && bPs <= 8)
		{
			return true;
		}

		return false;
	}

	bool isCastlinAvailabilityValid(const BoardState& board)
	{
		using namespace pieces;
		const auto& cas = board.getCastleAvailability();
		const auto& psc = board.getPieces();
		if (cas.find('K')->second)
		{
			if (psc[squares::e1] != wK || psc[squares::h1] != wR)
			{
				return false;
			}
		}
		if (cas.find('Q')->second)
		{
			if (psc[squares::e1] != wK || psc[squares::a1] != wR)
			{
				return false;
			}
		}
		if (cas.find('k')->second)
		{
			if (psc[squares::e8] != bK || psc[squares::h8] != bR)
			{
				return false;
			}
		}
		if (cas.find('q')->second)
		{
			if (psc[squares::e8] != bK || psc[squares::a8] != bR)
			{
				return false;
			}
		}

		return true;
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

	if (!isValid())
	{
		EngineUtilities::logE("initFromFEN failed: Invalid FEN.");
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
			break;
		case wK:
		case bK:
			makeKingMove(move);
			break;
		default:
			makeNonSpecializedMove(move);
			break;
	}

	//Finally, switch side to play.
	turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void BoardState::unmakeMove(const Move& move)
{
	using namespace pieces;

	switch (move.movingPiece)
	{
		case wP:
		case bP:
			unmakePawnMove(move);
			break;
		case wK:
		case bK:
			unmakeKingMove(move);
			break;
		default:
			unmakeNonSpecializedMove(move);
			break;
	}

	//Finally, switch side to play.
	turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
}

bool BoardState::isValid() const
{
	return isPieceCountValid(*this) && isEnPassantSqValid(*this) && isCastlinAvailabilityValid(*this);
}

void BoardState::makePawnMove(const Move& move)
{
	if (move.pawnPromotionPiece != pieces::none)
	{
		pieces[move.fromSquare] = pieces::none;
		if (move.capturedPiece != pieces::none)
		{
			pieces[move.capturedSquare] = pieces::none;
		}

		pieces[move.toSquare] = move.pawnPromotionPiece;
		handleCastelingFlagsMakeMove(move, *this);
		enPassantSquare = move.enPassantCreatedSquare;
	}
	else
	{
		makeNonSpecializedMove(move);
	}
}

void BoardState::unmakePawnMove(const Move& move)
{
	unmakeNonSpecializedMove(move);
}

void BoardState::makeKingMove(const Move& move)
{
	if (turn == pieces::Color::WHITE)
	{
		// Perform casteling.
		if (move.fromSquare == squares::e1)
		{
			if (move.toSquare == squares::g1)
			{
				assert(pieces[squares::h1] == pieces::wR);
				pieces[squares::h1] = pieces::none;
				pieces[squares::f1] = pieces::wR;
			}
			else if (move.toSquare == squares::c1)
			{
				assert(pieces[squares::a1] == pieces::wR);
				pieces[squares::a1] = pieces::none;
				pieces[squares::d1] = pieces::wR;
			}
		}
	}
	else
	{
		// Perform casteling.
		if (move.fromSquare == squares::e8)
		{
			if (move.toSquare == squares::g8)
			{
				assert(pieces[squares::h8] == pieces::bR);
				pieces[squares::h8] = pieces::none;
				pieces[squares::f8] = pieces::bR;
			}
			else if (move.toSquare == squares::c8)
			{
				assert(pieces[squares::a8] == pieces::bR);
				pieces[squares::a8] = pieces::none;
				pieces[squares::d8] = pieces::bR;
			}
		}
	}

	makeNonSpecializedMove(move);
}

void BoardState::unmakeKingMove(const Move& move)
{
	// At this point turn has switched sides, therefore the color check may seem wrong but is correct.
	if (turn == pieces::Color::BLACK)
	{
		// Unmake casteling.
		if (move.fromSquare == squares::e1)
		{
			if (move.toSquare == squares::g1)
			{
				assert(pieces[squares::h1] == pieces::none);
				pieces[squares::h1] = pieces::wR;
				pieces[squares::f1] = pieces::none;
			}
			else if (move.toSquare == squares::c1)
			{
				assert(pieces[squares::a1] == pieces::none);
				pieces[squares::a1] = pieces::wR;
				pieces[squares::d1] = pieces::none;
			}
		}
	}
	else
	{
		// Unmake casteling.
		if (move.fromSquare == squares::e8)
		{
			if (move.toSquare == squares::g8)
			{
				assert(pieces[squares::h8] == pieces::none);
				pieces[squares::h8] = pieces::bR;
				pieces[squares::f8] = pieces::none;
			}
			else if (move.toSquare == squares::c8)
			{
				assert(pieces[squares::a8] == pieces::none);
				pieces[squares::a8] = pieces::bR;
				pieces[squares::d8] = pieces::none;
			}
		}
	}

	unmakeNonSpecializedMove(move);
}

void BoardState::makeNonSpecializedMove(const Move& move)
{
	pieces[move.fromSquare] = pieces::none;
	if (move.capturedPiece != pieces::none)
	{
		pieces[move.capturedSquare] = pieces::none;
	}

	pieces[move.toSquare] = move.movingPiece;
	enPassantSquare = move.enPassantCreatedSquare;
	handleCastelingFlagsMakeMove(move, *this);
}

void BoardState::unmakeNonSpecializedMove(const Move& move)
{
	pieces[move.toSquare] = pieces::none;
	if (move.capturedPiece != pieces::none)
	{
		pieces[move.capturedSquare] = move.capturedPiece;
	}

	pieces[move.fromSquare] = move.movingPiece;
	enPassantSquare = move.previousEnPassantSquare;
	handleCastelingFlagsUnmakeMove(move, *this);
}

