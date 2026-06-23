/*! \file shortestpath3d.cxx

\brief extracts a 3D shortest path between two points (M. Postolski's operator)

<B>Usage:</B> shortestpath3d in.pgm sX sY sZ eX eY eZ out.pgm [out.list]

<B>Description:</B>
Pink interface to Michal Postolski's ShortestPath operator. Reads a 3D byte
image, runs the wave-propagation shortest path between (sX,sY,sZ) and
(eX,eY,eZ), and writes the extracted curve (voxels = 255, rest = 0) to a new
image of the same dimensions.

If the optional \b out.list argument is given, the path points are also
written, ordered from START to END, in Pink list format 'B' (same as
pgm2list): a "B n" header line followed by n lines of "x y z".

<B>Types supported:</B> byte 3D

\ingroup connect
*/

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>

#include <mccodimage.h>
#include <mcimage.h>

#include "shortestpath.h"
#include "list.h"

int main(int argc, char **argv)
{
    if ((argc != 9) && (argc != 10))
    {
        fprintf(stderr, "usage: %s in.pgm sX sY sZ eX eY eZ out.pgm [out.list]\n", argv[0]);
        exit(1);
    }

    struct xvimage *image = readimage(argv[1]);
    if (image == NULL)
    {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
    }
    if (datatype(image) != VFF_TYP_1_BYTE)
    {
        fprintf(stderr, "%s: only byte 3D images are supported\n", argv[0]);
        exit(1);
    }

    int32_t rs = rowsize(image);
    int32_t cs = colsize(image);
    int32_t ds = depth(image);
    int32_t ps = rs * cs;

    int sX = atoi(argv[2]), sY = atoi(argv[3]), sZ = atoi(argv[4]);
    int eX = atoi(argv[5]), eY = atoi(argv[6]), eZ = atoi(argv[7]);

    if (sX < 0 || sX >= rs || sY < 0 || sY >= cs || sZ < 0 || sZ >= ds ||
        eX < 0 || eX >= rs || eY < 0 || eY >= cs || eZ < 0 || eZ >= ds)
    {
        fprintf(stderr, "%s: start or end point is out of bounds\n", argv[0]);
        exit(1);
    }

    uint8_t *in = UCHARDATA(image);

    /* Padded volume vol[x][y][z]: a 1-voxel background border keeps the
       operator's unchecked 26-neighbour access inside bounds. */
    const int pX = rs + 2, pY = cs + 2, pZ = ds + 2;
    unsigned char ***vol = new unsigned char **[pX];
    for (int x = 0; x < pX; x++)
    {
        vol[x] = new unsigned char *[pY];
        for (int y = 0; y < pY; y++)
        {
            vol[x][y] = new unsigned char[pZ];
            for (int z = 0; z < pZ; z++) vol[x][y][z] = 0;
        }
    }
    for (int z = 0; z < ds; z++)
        for (int y = 0; y < cs; y++)
            for (int x = 0; x < rs; x++)
                vol[x + 1][y + 1][z + 1] = in[(size_t)z * ps + (size_t)y * rs + x];

    ShortestPath sp(vol, pX, pY, pZ);
    bool found = sp.compute(sX + 1, sY + 1, sZ + 1, eX + 1, eY + 1, eZ + 1);
    if (!found)
    {
        fprintf(stderr, "%s: no path found between the two points\n", argv[0]);
        exit(1);
    }

    struct xvimage *out = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    out->xdim = image->xdim; out->ydim = image->ydim; out->zdim = image->zdim;
    uint8_t *od = UCHARDATA(out);
    for (int z = 0; z < ds; z++)
        for (int y = 0; y < cs; y++)
            for (int x = 0; x < rs; x++)
                od[(size_t)z * ps + (size_t)y * rs + x] = vol[x + 1][y + 1][z + 1];

    writeimage(out, argv[8]);

    /* optional ordered point list (Pink 'B' format), start -> end */
    if (argc == 10)
    {
        /* getPath() holds the voxels in back-trace order (END -> START),
           in padded coordinates; collect, un-pad, then write reversed. */
        std::vector<int> px, py, pz;
        List *p = sp.getPath();
        if (p != NULL)
        {
            p->next();                      /* move past sentinel head */
            while (true)
            {
                px.push_back(p->getX() - 1);
                py.push_back(p->getY() - 1);
                pz.push_back(p->getZ() - 1);
                if (p->isEnd()) break;
                p->next();
            }
        }

        FILE *fd = fopen(argv[9], "w");
        if (!fd)
        {
            fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[9]);
            exit(1);
        }
        fprintf(fd, "B %d\n", (int)px.size());
        for (int i = (int)px.size() - 1; i >= 0; i--)   /* reverse: START -> END */
            fprintf(fd, "%d %d %d\n", px[i], py[i], pz[i]);
        fclose(fd);
    }

    freeimage(out);
    freeimage(image);
    return 0;
}
