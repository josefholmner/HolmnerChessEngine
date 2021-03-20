#pragma once

#include "PiecesAndSquares.h"

#include <array>
#include <vector>

/**
* The FastSqLookup enables fast lookup of e.g. squares that are on the same diagonal or same
* rank/file (among other things) as some specific square. Note that the construction of this
* object is slow, but once it has been constructed the lookup's are very fast. This means that
* instances of this class should be kept and stored somewhere for repeated use if possible.
*/
class FastSqLookup
{
public:
	FastSqLookup() noexcept;

	const std::array<std::vector<Square>, squares::num>& getDiagTowardsA8() const
	{
		return diagTowardsA8;
	}

	const std::array<std::vector<Square>, squares::num>& getDiagTowardsH8() const
	{
		return diagTowardsH8;
	}

	const std::array<std::vector<Square>, squares::num>& getDiagTowardsH1() const
	{
		return diagTowardsH1;
	}

	const std::array<std::vector<Square>, squares::num>& getDiagTowardsA1() const
	{
		return diagTowardsA1;
	}

	const std::array<std::vector<Square>, squares::num>& getstraightTowardsRankMin() const
	{
		return straightTowardsRankMin;
	}

	const std::array<std::vector<Square>, squares::num>& getstraightTowardsRankMax() const
	{
		return straightTowardsRankMax;
	}

	const std::array<std::vector<Square>, squares::num>& getstraightTowardsFileMin() const
	{
		return straightTowardsFileMin;
	}

	const std::array<std::vector<Square>, squares::num>& getstraightTowardsFileMax() const
	{
		return straightTowardsFileMax;
	}

	const std::array<std::vector<Square>, squares::num>& getknightReachableSquares() const
	{
		return knightReachableSquares;
	}

	const std::array<std::vector<Square>, squares::num>& getkingNonCastlingReachableSquares() const
	{
		return kingNonCastlingReachableSquares;
	}

	const std::array<std::vector<Square>, squares::num>& getWhitePawnCaptureSquares() const
	{
		return whitePawnCaptureSquares;
	}

	const std::array<std::vector<Square>, squares::num>& getBlackPawnCaptureSquares() const
	{
		return blackPawnCaptureSquares;
	}

	bool areSquaresInLinearLineOfSight(Square sq1, Square sq2) const
	{
		return inlinearLineOfSight[sq1][sq2];
	}

private:
	void init();

	std::array<std::vector<Square>, squares::num> diagTowardsA8;
	std::array<std::vector<Square>, squares::num> diagTowardsH8;
	std::array<std::vector<Square>, squares::num> diagTowardsH1;
	std::array<std::vector<Square>, squares::num> diagTowardsA1;

	std::array<std::vector<Square>, squares::num> straightTowardsRankMin;
	std::array<std::vector<Square>, squares::num> straightTowardsRankMax;
	std::array<std::vector<Square>, squares::num> straightTowardsFileMin;
	std::array<std::vector<Square>, squares::num> straightTowardsFileMax;

	std::array<std::vector<Square>, squares::num> knightReachableSquares;

	std::array<std::vector<Square>, squares::num> kingNonCastlingReachableSquares;

	std::array<std::vector<Square>, squares::num> whitePawnCaptureSquares;
	std::array<std::vector<Square>, squares::num> blackPawnCaptureSquares;

	std::array<std::array<bool, squares::num>, squares::num> inlinearLineOfSight;
};