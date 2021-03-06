#include "PrivateInclude/BoardState.h"

#include "PrivateInclude/EngineUtilities.h"
#include "PrivateInclude/Move.h"
#include "Common/CommonUtilities.h"

#include <iostream>
#include <cassert>
#include <algorithm>

namespace
{
	static constexpr Hash64 transpositionTableMaxLength = 5000000;

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

		if (rank < ranks::rank1 || rank > ranks::rank8 || file < files::fileA || file > files::fileH)
		{
			EngineUtilities::logE("Invalid square in FEN: " + str);
			return {};
		}

		return file + files::num * rank;
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
		for (Rank rank = ranks::rank8; rank >= ranks::rank1; rank--)
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

	Square findWhiteKingSquare(const BoardState& board)
	{
		const auto& pieces = board.getPieces();
		const auto itr = std::find(pieces.begin(), pieces.end(), pieces::wK);
		if(itr == pieces.end())
		{
			return pieces::none;
		}

		return std::distance(pieces.begin(), itr);
	}

	Square findBlackKingSquare(const BoardState& board)
	{
		const auto& pieces = board.getPieces();
		const auto itr = std::find(pieces.begin(), pieces.end(), pieces::bK);
		if (itr == pieces.end())
		{
			return pieces::none;
		}

		return std::distance(pieces.begin(), itr);
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
		int8_t wPs = 0, wNs = 0, wBs = 0, wRs = 0, wQs = 0, wKs = 0;
		int8_t bPs = 0, bNs = 0, bBs = 0, bRs = 0, bQs = 0, bKs = 0;
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

	bool isKingSquaresValid(const BoardState& board)
	{
		const Square wKsq = board.getWhiteKingSquare();
		const Square bKsq = board.getBlackKingSquare();
		return EngineUtilities::isNonNoneSquare(wKsq) && EngineUtilities::isNonNoneSquare(bKsq) &&
			board.getPiece(wKsq) == pieces::wK && board.getPiece(bKsq) == pieces::bK;
	}

	int16_t getHashIndex(Square sq, Piece piece)
	{
		return sq * pieces::num + piece;
	}
}

BoardState::BoardState(bool reserveTranspositionTable)
{
#ifdef NDEBUG
	transpositionTable.reserve(transpositionTableMaxLength);
#endif
}

bool BoardState::operator==(const BoardState& other) const
{
#ifndef NDEBUG
	// Debug build.
	return (pieces == other.getPieces() && casleAvailability == other.getCastleAvailability() &&
		enPassantSquare == other.getEnPassantSquare() && turn == other.getTurn() &&
		hash == other.getHash());
#else
	return hash == other.getHash();
#endif
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

	// Set king squares.
	wKingSq = findWhiteKingSquare(*this);
	bKingSq = findBlackKingSquare(*this);

	if (!isValid())
	{
		EngineUtilities::logE("initFromFEN failed: Invalid FEN.");
		return false;
	}

	// Update the hash according to this board state.
	hash = generateHash();

	return true;
}

std::string BoardState::toFEN() const
{
	std::string FEN = "";

	// Pieces.
	for (Rank rank = ranks::rank8; rank >= ranks::rank1; rank--)
	{
		int emptyCount = 0;
		for (File file = files::fileA; file <= files::fileH; file++)
		{
			const Square sq = file + rank * ranks::num;
			if (pieces[sq] != pieces::none)
			{
				if (emptyCount > 0)
				{
					FEN += static_cast<char>('0' + emptyCount);
					emptyCount = 0;
				}

				FEN += pieces::pieceToStr(pieces[sq]);
			}
			else if(file == files::fileH)
			{
				emptyCount++;
				FEN += static_cast<char>('0' + emptyCount);
			}
			else
			{
				emptyCount++;
			}
		}

		if (rank != ranks::rank1)
		{
			FEN += "/";
		}
	}

	FEN += " ";

	// Turn.
	FEN += turn == pieces::Color::WHITE ? "w" : "b";
	FEN += " ";

	// Castling availability.
	bool noCastling = true;
	for (const auto entry : casleAvailability)
	{
		if (entry.second)
		{
			FEN += entry.first;
			noCastling = false;
		}
	}

	if (noCastling)
	{
		FEN += "-";
	}

	FEN += " ";

	// En passant square.
	FEN += enPassantSquare == squares::none ? "-" : squares::squareToStr(enPassantSquare);
	FEN += " ";

	// Halfmove and fullmove count (not implemented).
	FEN += "0 1";
	return FEN;
}

void BoardState::printBoard() const
{
	for (Rank rank = ranks::rank8; rank >= ranks::rank1; rank--)
	{
		for (File file = files::fileA; file <= files::fileH; file++)
		{
			const Square sq = file + rank * files::num;
			std::cout << "[" << pieces::pieceToStr(pieces[sq]) << "] ";
		}

		std::cout << std::endl;
	}
}

void BoardState::makeMove(const Move& move)
{
	using namespace pieces;
	using namespace hashValues;

	// Handle castling availability.
	if (move.prohibitsWKcastling)
	{
		assert(casleAvailability['K']);
		casleAvailability['K'] = false;
		hash ^= values[castlingIndexStart];
	}
	if (move.prohibitsWQcastling)
	{
		assert(casleAvailability['Q']);
		casleAvailability['Q'] = false;
		hash ^= values[castlingIndexStart + 1];
	}
	if (move.prohibitsBKcastling)
	{
		assert(casleAvailability['k']);
		casleAvailability['k'] = false;
		hash ^= values[castlingIndexStart + 2];
	}
	if (move.prohibitsBQcastling)
	{
		assert(casleAvailability['q']);
		casleAvailability['q'] = false;
		hash ^= values[castlingIndexStart + 3];
	}

	// Must be set for all moves since any move will remove previous en passant square.
	enPassantSquare = move.enPassantCreatedSquare;
	if (move.previousEnPassantSquare != squares::none)
	{
		hash ^= values[(size_t)enPassantSqIndexStart + move.previousEnPassantSquare];
	}

	if (move.enPassantCreatedSquare != squares::none)
	{
		hash ^= values[(size_t)enPassantSqIndexStart + move.enPassantCreatedSquare];
	}

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

	// Finally, switch side to play.
	turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
	hash ^= values[whiteToPlayIndex];
	assert(hash == generateHash());
}

void BoardState::unmakeMove(const Move& move)
{
	using namespace pieces;
	using namespace hashValues;

	if (move.prohibitsWKcastling)
	{
		assert(!casleAvailability['K']);
		casleAvailability['K'] = true;
		hash ^= values[castlingIndexStart];
	}
	if (move.prohibitsWQcastling)
	{
		assert(!casleAvailability['Q']);
		casleAvailability['Q'] = true;
		hash ^= values[castlingIndexStart + 1];
	}
	if (move.prohibitsBKcastling)
	{
		assert(!casleAvailability['k']);
		casleAvailability['k'] = true;
		hash ^= values[castlingIndexStart + 2];
	}
	if (move.prohibitsBQcastling)
	{
		assert(!casleAvailability['q']);
		casleAvailability['q'] = true;
		hash ^= values[castlingIndexStart + 3];
	}

	// Must be set for all moves since any move will remove previous en passant square.
	enPassantSquare = move.previousEnPassantSquare;
	if (move.enPassantCreatedSquare != squares::none)
	{
		hash ^= values[(size_t)enPassantSqIndexStart + move.enPassantCreatedSquare];
	}

	if (move.previousEnPassantSquare != squares::none)
	{
		hash ^= values[(size_t)enPassantSqIndexStart + move.previousEnPassantSquare];
	}

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
	hash ^= values[whiteToPlayIndex];
	assert(hash == generateHash());
}

bool BoardState::isValid() const
{
	return isPieceCountValid(*this) && isEnPassantSqValid(*this) &&
		isCastlinAvailabilityValid(*this) && isKingSquaresValid(*this);
}

Hash64 BoardState::generateHash() const
{
	using namespace hashValues;
	Hash64 hash = 0;

	// Hash pieces on the board.
	for (Square sq = 0; sq < squares::num; sq++)
	{
		assert(EngineUtilities::isValidSquare(sq));
		if (pieces[sq] != pieces::none)
		{
			hash ^= values[getHashIndex(sq, pieces[sq])];
		}
	}

	// Hash white to play.
	if (turn == pieces::Color::WHITE)
	{
		hash ^= values[whiteToPlayIndex];
	}

	// Hash castling possible.
	if (casleAvailability.find('K')->second) hash ^= values[castlingIndexStart];
	if (casleAvailability.find('Q')->second) hash ^= values[castlingIndexStart + 1];
	if (casleAvailability.find('k')->second) hash ^= values[castlingIndexStart + 2];
	if (casleAvailability.find('q')->second) hash ^= values[castlingIndexStart + 3];

	// Hash en passant square.
	if (enPassantSquare != squares::none)
	{
		hash ^= values[(size_t)enPassantSqIndexStart + enPassantSquare];
	}

	return hash;
}

void BoardState::addTranspositionElement(const searchHelpers::tp::Element& elem)
{
	if (transpositionTable.size() >= transpositionTableMaxLength)
	{
		return;
	}

	transpositionTable[hash] = elem;
}

const searchHelpers::tp::Element* BoardState::findTranspositionElement() const
{
	const auto it = transpositionTable.find(hash);
	if (it == transpositionTable.end())
	{
		return nullptr;
	}

	return &it->second;
}

void BoardState::makePawnMove(const Move& move)
{
	using namespace hashValues;

	if (move.pawnPromotionPiece != pieces::none)
	{
		pieces[move.fromSquare] = pieces::none;
		hash ^= values[getHashIndex(move.fromSquare, move.movingPiece)];
		if (move.capturedPiece != pieces::none)
		{
			pieces[move.capturedSquare] = pieces::none;
			hash ^= values[getHashIndex(move.capturedSquare, move.capturedPiece)];
		}

		pieces[move.toSquare] = move.pawnPromotionPiece;
		hash ^= values[getHashIndex(move.toSquare, move.pawnPromotionPiece)];
	}
	else
	{
		makeNonSpecializedMove(move);
	}
}

void BoardState::unmakePawnMove(const Move& move)
{
	using namespace hashValues;

	// In case of pawn promotion, the hashing must be handled specifically.
	if (move.pawnPromotionPiece != pieces::none)
	{
		hash ^= values[getHashIndex(move.toSquare, move.pawnPromotionPiece)];
		if (move.capturedPiece != pieces::none)
		{
			hash ^= values[getHashIndex(move.capturedSquare, move.capturedPiece)];
		}
		hash ^= values[getHashIndex(move.fromSquare, move.movingPiece)];

		unmakeNonSpecializedMove(move, false);
	}
	else
	{
		unmakeNonSpecializedMove(move);
	}
}

void BoardState::makeKingMove(const Move& move)
{
	if (turn == pieces::Color::WHITE)
	{
		wKingSq = move.toSquare;
		makeWhiteRookMoveIfCastling(move);
	}
	else
	{
		bKingSq = move.toSquare;
		makeBlackRookMoveIfCastling(move);
	}

	makeNonSpecializedMove(move);
}

void BoardState::unmakeKingMove(const Move& move)
{
	// At this point turn has switched sides, therefore the color check may seem wrong but is correct.
	if (turn == pieces::Color::BLACK)
	{
		wKingSq = move.fromSquare;
		unmakeWhiteRookMoveIfCastling(move);
	}
	else
	{
		bKingSq = move.fromSquare;
		unmakeBlackRookMoveIfCastling(move);
	}

	unmakeNonSpecializedMove(move);
}

void BoardState::makeWhiteRookMoveIfCastling(const Move& move)
{
	using namespace hashValues;
	assert(move.movingPiece == pieces::wK);

	// Perform casteling (rook only).
	if (move.fromSquare == squares::e1)
	{
		if (move.toSquare == squares::g1)
		{
			assert(pieces[squares::h1] == pieces::wR);
			pieces[squares::h1] = pieces::none;
			pieces[squares::f1] = pieces::wR;
			hash ^= values[getHashIndex(squares::h1, pieces::wR)];
			hash ^= values[getHashIndex(squares::f1, pieces::wR)];
		}
		else if (move.toSquare == squares::c1)
		{
			assert(pieces[squares::a1] == pieces::wR);
			pieces[squares::a1] = pieces::none;
			pieces[squares::d1] = pieces::wR;
			hash ^= values[getHashIndex(squares::a1, pieces::wR)];
			hash ^= values[getHashIndex(squares::d1, pieces::wR)];
		}
	}
}

void BoardState::makeBlackRookMoveIfCastling(const Move& move)
{
	using namespace hashValues;
	assert(move.movingPiece == pieces::bK);

	// Perform casteling (rook only).
	if (move.fromSquare == squares::e8)
	{
		if (move.toSquare == squares::g8)
		{
			assert(pieces[squares::h8] == pieces::bR);
			pieces[squares::h8] = pieces::none;
			pieces[squares::f8] = pieces::bR;
			hash ^= values[getHashIndex(squares::h8, pieces::bR)];
			hash ^= values[getHashIndex(squares::f8, pieces::bR)];
		}
		else if (move.toSquare == squares::c8)
		{
			assert(pieces[squares::a8] == pieces::bR);
			pieces[squares::a8] = pieces::none;
			pieces[squares::d8] = pieces::bR;
			hash ^= values[getHashIndex(squares::a8, pieces::bR)];
			hash ^= values[getHashIndex(squares::d8, pieces::bR)];
		}
	}
}

void BoardState::unmakeWhiteRookMoveIfCastling(const Move& move)
{
	using namespace hashValues;
	assert(move.movingPiece == pieces::wK);

	// Unmake casteling (rook only).
	if (move.fromSquare == squares::e1)
	{
		if (move.toSquare == squares::g1)
		{
			assert(pieces[squares::h1] == pieces::none);
			pieces[squares::h1] = pieces::wR;
			pieces[squares::f1] = pieces::none;
			hash ^= values[getHashIndex(squares::h1, pieces::wR)];
			hash ^= values[getHashIndex(squares::f1, pieces::wR)];
		}
		else if (move.toSquare == squares::c1)
		{
			assert(pieces[squares::a1] == pieces::none);
			pieces[squares::a1] = pieces::wR;
			pieces[squares::d1] = pieces::none;
			hash ^= values[getHashIndex(squares::a1, pieces::wR)];
			hash ^= values[getHashIndex(squares::d1, pieces::wR)];
		}
	}
}

void BoardState::unmakeBlackRookMoveIfCastling(const Move& move)
{
	using namespace hashValues;
	assert(move.movingPiece == pieces::bK);

	// Unmake casteling (rook only).
	if (move.fromSquare == squares::e8)
	{
		if (move.toSquare == squares::g8)
		{
			assert(pieces[squares::h8] == pieces::none);
			pieces[squares::h8] = pieces::bR;
			pieces[squares::f8] = pieces::none;
			hash ^= values[getHashIndex(squares::h8, pieces::bR)];
			hash ^= values[getHashIndex(squares::f8, pieces::bR)];
		}
		else if (move.toSquare == squares::c8)
		{
			assert(pieces[squares::a8] == pieces::none);
			pieces[squares::a8] = pieces::bR;
			pieces[squares::d8] = pieces::none;
			hash ^= values[getHashIndex(squares::a8, pieces::bR)];
			hash ^= values[getHashIndex(squares::d8, pieces::bR)];
		}
	}
}

void BoardState::makeNonSpecializedMove(const Move& move)
{
	using namespace hashValues;

	pieces[move.fromSquare] = pieces::none;
	hash ^= values[getHashIndex(move.fromSquare, move.movingPiece)];
	if (move.capturedPiece != pieces::none)
	{
		pieces[move.capturedSquare] = pieces::none;
		hash ^= values[getHashIndex(move.capturedSquare, move.capturedPiece)];
	}

	pieces[move.toSquare] = move.movingPiece;
	hash ^= values[getHashIndex(move.toSquare, move.movingPiece)];
}

void BoardState::unmakeNonSpecializedMove(const Move& move, bool updateHash)
{
	using namespace hashValues;

	pieces[move.toSquare] = pieces::none;
	if (move.capturedPiece != pieces::none)
	{
		pieces[move.capturedSquare] = move.capturedPiece;
	}
	pieces[move.fromSquare] = move.movingPiece;

	if (updateHash)
	{
		hash ^= values[getHashIndex(move.toSquare, move.movingPiece)];
		if (move.capturedPiece != pieces::none)
		{
			hash ^= values[getHashIndex(move.capturedSquare, move.capturedPiece)];
		}
		hash ^= values[getHashIndex(move.fromSquare, move.movingPiece)];
	}
}

