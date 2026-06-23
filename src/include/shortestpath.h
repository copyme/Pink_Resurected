#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "list.h"

/*!
 * \file shortestpath.h
 * \brief 3D shortest-path (geodesic) extraction by wave propagation.
 *
 * Operates in place on a 3D byte volume addressed as img[x][y][z].
 * Foreground voxels are > 0; background voxels are 0. compute() finds a
 * shortest path between two foreground voxels and rewrites the volume so
 * that only the path voxels are set to 255 (everything else becomes 0).
 *
 * NOTE: neighbour access in addNeigbour() is not bounds-checked, so the
 * caller MUST guarantee that no foreground voxel lies on the outermost
 * border of the volume (e.g. by padding it with a background border).
 */
class ShortestPath
{
    unsigned char ***img;
    int sizeX;
    int sizeY;
    int sizeZ;
    int MAXCODE;
    List *list;
    List *path;        // ordered path voxels, recorded during back-tracing
    int pointsCount;

    void addNeigbours(int ***field, int x, int y, int z, int metricF, int metricE, int metricV);
    void addNeigbour(int ***field, int x, int  y, int z, int d1, int d2, int d3, int metric);


public:
    ShortestPath(unsigned char ***img, int sizeX, int sizeY, int sizeZ);
    ~ShortestPath(void);
    int getPointsCount(void);
    /*!
     * \brief Ordered path recorded by the last compute() call.
     * Voxels are stored in back-trace order, i.e. from END to START.
     * Returns NULL if compute() has not found a path yet.
     */
    List *getPath(void);
    /*!
     * \brief Extract the shortest path from (sX,sY,sZ) to (eX,eY,eZ).
     * \return true if a path was found, false otherwise.
     */
    bool compute(int sX, int sY, int sZ, int eX, int eY, int eZ);
};

#endif // SHORTESTPATH_H
