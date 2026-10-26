#ifndef PATTERN_INDEXER_H
#define PATTERN_INDEXER_H

#include "cube/cubeIndexModel.h"
#include "util/math.h"

#include <array>
#include <bitset>
#include <vector>

// Templates have to be fully defined in the header so the compiler can instantiate them
template <size_t N, size_t K = N>
class PermutationIndexer {
    std::array<int, (1 << N) - 1> onesCountLookup;
    std::array<int, K> factorials;

public:

    PermutationIndexer() {
        for (int i = 0; i < (1 << N) - 1; i++) {
            std::bitset<N> bits(i);
            onesCountLookup[i] = bits.count();
        }

        for (int i = 0; i < K; i++)
            factorials[i] = pick(N - 1 - i, K - 1 - i);
    }

    int lehmerIndex(const std::array<int, K>& perm) const {

        std::array<int, K> lehmer;
        std::bitset<N> seen;
        lehmer[0] = perm[0];
        seen[N - 1 - perm[0]] = 1;

        for (int i = 1; i < K; i++) {
            seen[N - 1 - perm[i]] = 1;
            int numOnes = onesCountLookup[seen.to_ulong() >> (N - perm[i])];
            lehmer[i] = perm[i] - numOnes;
        }

        int index = 0;
        for (int i = 0; i < K; i++)
            index += lehmer[i] * factorials[i];
        return index;
    }
};


template<size_t N, size_t K>
class CombinationIndexer {
    std::array<std::array<int, K + 1>, N + 1> choices;

public:

    CombinationIndexer() {
        for (int n = 0; n <= N; n++)
            for (int k = 0; k <= K; k++)
                choices[n][k] = choose(n, k);
    }

    int lehmerIndex(const std::array<int, K>& comb) const {
        int index = choices[N][K];
        for (int i = 0; i < K; i++)
            index -= choices[N - (comb[i] + 1)][K - i];
        return index - 1;
    }
};


class CubeIndexer {

    CombinationIndexer<12, 4>   edgeCombIndexer;
    PermutationIndexer<8>       perm8Indexer;
    PermutationIndexer<4>       perm4Indexer;
    PermutationIndexer<12, 4>   edgePermIndexer;

public:

    int getCornerOrientationIndex(CubeIndexModel& cube);
    int getEdgeOrientationIndex(CubeIndexModel& cube);
    int getUDSliceIndex(CubeIndexModel& cube);
    int getCornerPermIndex(CubeIndexModel& cube);
    int getEdgeRPermIndex(CubeIndexModel& cube);
    int getEdgeMPermIndex(CubeIndexModel& cube);
    int getUDSliceSortIndex(CubeIndexModel& cube);
    int getUSliceSortIndex(CubeIndexModel& cube);
    int getDSliceSortIndex(CubeIndexModel& cube);
};


class ReverseCubeIndexer {

    std::array<std::array<int, 12>, 4>    reverseCombData;

public:

    ReverseCubeIndexer();

    void updateCubeCornerOrientation(CubeIndexModel& cube, int cornerIndex);
    void updateCubeEdgeOrientaion(CubeIndexModel& cube, int edgeIndex);
    void updateCubeEdgeMComb(CubeIndexModel& cube, int edgeMCombIndex);
    std::vector<int> reversePermIndex(int n, int k, int index);
    void updateCubeCornerPerm(CubeIndexModel& cube, int cornerPermIndex);
    void updateCubeEdgeRPerm(CubeIndexModel& cube, int edgeRPermIndex);
    void updateCubeEdgeMPerm(CubeIndexModel& cube, int edgeMPermIndex);
    void updateCubeUDSlice(CubeIndexModel& cube, int UDSliceSortIndex);
    void updateCubeUSlice(CubeIndexModel& cube, int USliceSortIndex);
    void updateCubeDSlice(CubeIndexModel& cube, int DSliceSortIndex);
};


#endif // !PATTERN_INDEXER_H
