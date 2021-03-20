#include "PrivateInclude/FastSqLookup.h"

#include <array>

namespace
{
	struct SweepSearchParams
	{
		Rank rankLimit;
		File fileLimit;
		Square jump;
	};

	void sweepSearch(const SweepSearchParams& params, Square sq, int32_t distance,
		std::vector<Square>& outSqs)
	{
		const Rank rank = ranks::toRank(sq);
		const File file = files::toFile(sq);
		if (rank == params.rankLimit || file == params.fileLimit)
		{
			if (distance > 0)
			{
				outSqs.push_back(sq);
			}

			return;
		}

		if (distance > 0)
		{
			outSqs.push_back(sq);
		}

		return sweepSearch(params, sq + params.jump, distance + 1, outSqs);
	}

	std::vector<Square> getKnightReachableSquares(Square sq)
	{
		auto rankInsideBounds = [](Rank rank) {return (rank >= 0 && rank <= 7); };
		auto fileInsideBounds = [](File file) {return (file >= 0 && file <= 7); };

		const Rank rank = ranks::toRank(sq);
		const File file = files::toFile(sq);

		// First in pair is rank, second in pair is file.
		static constexpr std::array<std::pair<Square, Square>, 8> jumps = {
			std::make_pair(2, 1), std::make_pair(1, 2), std::make_pair(-1, 2), std::make_pair(-2, 1),
			std::make_pair(-2,-1), std::make_pair(-1, -2), std::make_pair(1, -2), std::make_pair(2, -1) };

		std::vector<Square> reachableSqs;
		for (const auto& jump : jumps)
		{
			if (rankInsideBounds(rank + jump.first) && fileInsideBounds(file + jump.second))
			{
				reachableSqs.push_back(sq + 8 * jump.first + jump.second);
			}
		}

		return reachableSqs;
	}

	bool contains(std::vector<Square> vec, Square sq)
	{
		return std::find(vec.begin(), vec.end(), sq) != vec.end();
	}
}

FastSqLookup::FastSqLookup() noexcept
{
	init();
}

void FastSqLookup::init()
{
	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		// Diagonals.
		std::vector<Square> diagTa8;
		std::vector<Square> diagTh8;
		std::vector<Square> diagTh1;
		std::vector<Square> diagTa1;
		sweepSearch({ 7, 0, 7 }, sq, 0, diagTa8);
		sweepSearch({ 7, 7, 9 }, sq, 0, diagTh8);
		sweepSearch({ 0, 7, -7 }, sq, 0, diagTh1);
		sweepSearch({ 0, 0, -9 }, sq, 0, diagTa1);
		diagTowardsA8[sq] = diagTa8;
		diagTowardsH8[sq] = diagTh8;
		diagTowardsH1[sq] = diagTh1;
		diagTowardsA1[sq] = diagTa1;

		// Straights.
		std::vector<Square> straightTRmin;
		std::vector<Square> straightTRmax;
		std::vector<Square> straightTFmin;
		std::vector<Square> straightTFmax;
		sweepSearch({ 0, -1, -8 }, sq, 0, straightTRmin);
		sweepSearch({ 7, -1, 8 }, sq, 0, straightTRmax);
		sweepSearch({ -1, 0, -1 }, sq, 0, straightTFmin);
		sweepSearch({ -1, 7, 1 }, sq, 0, straightTFmax);
		straightTowardsRankMin[sq] = straightTRmin;
		straightTowardsRankMax[sq] = straightTRmax;
		straightTowardsFileMin[sq] = straightTFmin;
		straightTowardsFileMax[sq] = straightTFmax;

		// Knight reachable squares.
		knightReachableSquares[sq] = getKnightReachableSquares(sq);

		// King reachable squares (casteling moves are not included).
		if (diagTowardsA8[sq].size() > 0) {
			kingNonCastlingReachableSquares[sq].push_back(diagTowardsA8[sq][0]);
		}
		if (diagTowardsH8[sq].size() > 0) {
			kingNonCastlingReachableSquares[sq].push_back(diagTowardsH8[sq][0]);
		}
		if (diagTowardsA1[sq].size() > 0) {
			kingNonCastlingReachableSquares[sq].push_back(diagTowardsA1[sq][0]);
		}
		if (diagTowardsH1[sq].size() > 0) {
			kingNonCastlingReachableSquares[sq].push_back(diagTowardsH1[sq][0]);
		}
		if (straightTowardsRankMin[sq].size() > 0) {
			kingNonCastlingReachableSquares[sq].push_back(straightTowardsRankMin[sq][0]);
		}
		if (straightTowardsRankMax[sq].size() > 0) {
			kingNonCastlingReachableSquares[sq].push_back(straightTowardsRankMax[sq][0]);
		}
		if (straightTowardsFileMin[sq].size() > 0) {
			kingNonCastlingReachableSquares[sq].push_back(straightTowardsFileMin[sq][0]);
		}
		if (straightTowardsFileMax[sq].size() > 0) {
			kingNonCastlingReachableSquares[sq].push_back(straightTowardsFileMax[sq][0]);
		}

		// Pawn capture squares.		
		if (diagTowardsA8[sq].size() > 0) { whitePawnCaptureSquares[sq].push_back(diagTowardsA8[sq][0]); }
		if (diagTowardsH8[sq].size() > 0) { whitePawnCaptureSquares[sq].push_back(diagTowardsH8[sq][0]); }
		if (diagTowardsA1[sq].size() > 0) { blackPawnCaptureSquares[sq].push_back(diagTowardsA1[sq][0]); }
		if (diagTowardsH1[sq].size() > 0) { blackPawnCaptureSquares[sq].push_back(diagTowardsH1[sq][0]); }

		for (Square sightSq = squares::a1; sightSq < squares::num; sightSq++)
		{
			if (sq == sightSq) { inlinearLineOfSight[sq][sightSq] = true; }
			else if (contains(diagTowardsA8[sq], sightSq)) { inlinearLineOfSight[sq][sightSq] = true; }
			else if (contains(diagTowardsH8[sq], sightSq)) { inlinearLineOfSight[sq][sightSq] = true; }
			else if (contains(diagTowardsA1[sq], sightSq)) { inlinearLineOfSight[sq][sightSq] = true; }
			else if (contains(diagTowardsH1[sq], sightSq)) { inlinearLineOfSight[sq][sightSq] = true; }
			else if (contains(straightTowardsRankMin[sq], sightSq)) { inlinearLineOfSight[sq][sightSq] = true; }
			else if (contains(straightTowardsRankMax[sq], sightSq)) { inlinearLineOfSight[sq][sightSq] = true; }
			else if (contains(straightTowardsFileMin[sq], sightSq)) { inlinearLineOfSight[sq][sightSq] = true; }
			else if (contains(straightTowardsFileMax[sq], sightSq)) { inlinearLineOfSight[sq][sightSq] = true; }
			else { inlinearLineOfSight[sq][sightSq] = false; }
		}
	}
}
