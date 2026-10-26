#include "solver/kociemba/patternIndexer.h"

using namespace std;

int CubeIndexer::getCornerOrientationIndex(CubeIndexModel& cube) {
    return cube.corners[0].orientation +
        cube.corners[1].orientation * 3 +
        cube.corners[2].orientation * 9 +
        cube.corners[3].orientation * 27 +
        cube.corners[4].orientation * 81 +
        cube.corners[5].orientation * 243 +
        cube.corners[6].orientation * 729;
}

int CubeIndexer::getEdgeOrientationIndex(CubeIndexModel& cube) {
    return cube.edges[0].orientation +
        cube.edges[1].orientation * 2 +
        cube.edges[2].orientation * 4 +
        cube.edges[3].orientation * 8 +
        cube.edges[4].orientation * 16 +
        cube.edges[5].orientation * 32 +
        cube.edges[6].orientation * 64 +
        cube.edges[7].orientation * 128 +
        cube.edges[8].orientation * 256 +
        cube.edges[9].orientation * 512 +
        cube.edges[10].orientation * 1024;
}

int CubeIndexer::getUDSliceIndex(CubeIndexModel& cube) {
    array<int, 4> edgeComb;
    int cur = 0;
    for (int i = 0; i < 12 && cur < 4; i++) {
        int index = cube.edges[i].index;
        if (index > 3 && index < 8)
            edgeComb[cur++] = i;
    }

    return edgeCombIndexer.lehmerIndex(edgeComb);
}

int CubeIndexer::getCornerPermIndex(CubeIndexModel& cube) {
    array<int, 8> cornerIndeces = {
        cube.corners[0].index,
        cube.corners[1].index,
        cube.corners[2].index,
        cube.corners[3].index,
        cube.corners[4].index,
        cube.corners[5].index,
        cube.corners[6].index,
        cube.corners[7].index,
    };

    return perm8Indexer.lehmerIndex(cornerIndeces);
}

int CubeIndexer::getEdgeRPermIndex(CubeIndexModel& cube) {
    array<int, 8> edgeRIndeces = {
    cube.edges[0].index > 3 ? cube.edges[0].index - 4 : cube.edges[0].index,
    cube.edges[1].index > 3 ? cube.edges[1].index - 4 : cube.edges[1].index,
    cube.edges[2].index > 3 ? cube.edges[2].index - 4 : cube.edges[2].index,
    cube.edges[3].index > 3 ? cube.edges[3].index - 4 : cube.edges[3].index,
    cube.edges[8].index > 3 ? cube.edges[8].index - 4 : cube.edges[8].index,
    cube.edges[9].index > 3 ? cube.edges[9].index - 4 : cube.edges[9].index,
    cube.edges[10].index > 3 ? cube.edges[10].index - 4 : cube.edges[10].index,
    cube.edges[11].index > 3 ? cube.edges[11].index - 4 : cube.edges[11].index
    };

    return perm8Indexer.lehmerIndex(edgeRIndeces);
}

int CubeIndexer::getEdgeMPermIndex(CubeIndexModel& cube) {
    array<int, 4> edgeMIndeces = {
            cube.edges[4].index - 4,
            cube.edges[5].index - 4,
            cube.edges[6].index - 4,
            cube.edges[7].index - 4,
    };

    return perm4Indexer.lehmerIndex(edgeMIndeces);
}

int CubeIndexer::getUDSliceSortIndex(CubeIndexModel& cube) {
    array<int, 4> edgeSlicePerm;
    for (int i = 0; i < 12; i++) {
        int& index = cube.edges[i].index;
        if (index > 3 && index < 8)
            edgeSlicePerm[index - 4] = i;
    }
    return edgePermIndexer.lehmerIndex(edgeSlicePerm);
}

int CubeIndexer::getUSliceSortIndex(CubeIndexModel& cube) {
    array<int, 4> edgeSlicePerm;
    for (int i = 0; i < 12; i++) {
        int& index = cube.edges[i].index;
        if (index < 4 && index > -1)
            edgeSlicePerm[index] = i;
    }
    return edgePermIndexer.lehmerIndex(edgeSlicePerm);
}

int CubeIndexer::getDSliceSortIndex(CubeIndexModel& cube) {
    array<int, 4> edgeSlicePerm;
    for (int i = 0; i < 12; i++) {
        int& index = cube.edges[i].index;
        if (index > 7)
            edgeSlicePerm[index - 8] = i;
    }
    return edgePermIndexer.lehmerIndex(edgeSlicePerm);
}


ReverseCubeIndexer::ReverseCubeIndexer() {
    for (int n = 0; n < 12; n++)
        for (int k = 0; k < 4; k++)
            reverseCombData[k][n] = choose(n, k + 1);
}

void ReverseCubeIndexer::updateCubeCornerOrientation(CubeIndexModel& cube, int cornerIndex) {

    int totalOri = 0;
    for (int i = 0; i < 7; i++) {
        cube.corners[i].orientation = cornerIndex % 3;
        totalOri += cornerIndex % 3;
        cornerIndex /= 3;
    }
    cube.corners[7].orientation = (3 - (totalOri % 3)) % 3;
}

void ReverseCubeIndexer::updateCubeEdgeOrientaion(CubeIndexModel& cube, int edgeIndex) {

    int totalOri = 0;
    int ogOr = edgeIndex;
    for (int i = 0; i < 11; i++) {
        cube.edges[i].orientation = edgeIndex % 2;
        totalOri += edgeIndex % 2;
        edgeIndex /= 2;
    }
    cube.edges[11].orientation = totalOri % 2;
}

void ReverseCubeIndexer::updateCubeEdgeMComb(CubeIndexModel& cube, int edgeMCombIndex) {
    cube.edges[4].index = 0;
    cube.edges[5].index = 0;
    cube.edges[6].index = 0;
    cube.edges[7].index = 0;

    int deltaIndex = 495 - (edgeMCombIndex + 1);
    for (int i = 3; i >= 0; i--)
        for (int j = 11; j >= 0; j--)
            if (reverseCombData[i][j] <= deltaIndex) {
                cube.edges[12 - 1 - j].index = 7 - i;
                deltaIndex -= reverseCombData[i][j];
                break;
            }
}

vector<int> ReverseCubeIndexer::reversePermIndex(int n, int k, int index) {
    vector<int> lehmer(k);
    for (int i = 0; i < k; i++) {
        lehmer[i] = index / pick(n - 1 - i, k - 1 - i);
        index -= lehmer[i] * pick(n - 1 - i, k - 1 - i);
    }

    vector<int> seen(n), left(n);
    for (int i = 0; i < n; i++)
        left[i] = i;

    vector<int> permutation(k);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            if (left[j] == lehmer[i] && !seen[j]) {
                permutation[i] = j;
                seen[j] = 1;
                for (int p = j + 1; p < n; p++)
                    left[p]--;
                break;
            }
        }
    }
    return permutation;
}

void ReverseCubeIndexer::updateCubeCornerPerm(CubeIndexModel& cube, int cornerPermIndex) {

    vector<int> permutation = reversePermIndex(8, 8, cornerPermIndex);
    for (int i = 0; i < 8; i++)
        cube.corners[i].index = permutation[i];
}

void ReverseCubeIndexer::updateCubeEdgeRPerm(CubeIndexModel& cube, int edgeRPermIndex) {

    vector<int> permutation = reversePermIndex(8, 8, edgeRPermIndex);
    for (int i = 0; i < 8; i++) {
        int position = i;
        int index = permutation[i];
        if (i > 3)
            position += 4;
        if (permutation[i] > 3)
            index += 4;
        cube.edges[position].index = index;
    }
}

void ReverseCubeIndexer::updateCubeEdgeMPerm(CubeIndexModel& cube, int edgeMPermIndex) {

    vector<int> permutation = reversePermIndex(4, 4, edgeMPermIndex);
    for (int i = 0; i < 4; i++)
        cube.edges[i + 4].index = permutation[i] + 4;
}

void ReverseCubeIndexer::updateCubeUDSlice(CubeIndexModel& cube, int UDSliceSortIndex) {

    vector<int> permutation = reversePermIndex(12, 4, UDSliceSortIndex);
    for (int i = 0; i < 4; i++)
        cube.edges[permutation[i]].index = i + 4;
}

void ReverseCubeIndexer::updateCubeUSlice(CubeIndexModel& cube, int USliceSortIndex) {

    vector<int> permutation = reversePermIndex(12, 4, USliceSortIndex);
    for (int i = 0; i < 4; i++)
        cube.edges[permutation[i]].index = i;
}

void ReverseCubeIndexer::updateCubeDSlice(CubeIndexModel& cube, int DSliceSortIndex) {

    vector<int> permutation = reversePermIndex(12, 4, DSliceSortIndex);
    for (int i = 0; i < 4; i++)
        cube.edges[permutation[i]].index = i + 8;
}
