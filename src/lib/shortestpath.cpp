#include "shortestpath.h"

ShortestPath::ShortestPath(unsigned char ***img, int sizeX, int sizeY, int sizeZ)
{
        this->img = img;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->sizeZ = sizeZ;

        MAXCODE = sizeX*sizeY*sizeZ;
}

ShortestPath::~ShortestPath(void)
{
}


bool ShortestPath::compute(int sX, int sY, int sZ, int eX, int eY, int eZ)
{
        int ***SSfield = new int **[sizeX];

        list = new List();

        //generate SSfiled and BSfield
        for(int h=0;h<sizeX;h++)
        {
                SSfield[h] = new int *[sizeY];

                for(int b=0;b<sizeY;b++)
                {
                        SSfield[h][b] = new int [sizeZ];
                }
        }

        for(int z=0; z<sizeZ; z++)
                for(int y=0; y<sizeY; y++)
                        for(int x=0; x<sizeX; x++)
                        {
                                if(img[x][y][z]>0)
                                {
                                        SSfield[x][y][z] = MAXCODE;
                                }
                                else SSfield[x][y][z] = 0;
                        };

        //SSfield

        SSfield[sX][sY][sZ]=1;
        list->pushBack(sX, sY, sZ);

        while(1)
        {
                list->next();
                addNeigbours(SSfield, list->getX(), list->getY(), list->getZ(), 1, 2, 3);
                if(list->isEnd())
                {
                        break;
                }
        }
        list->clear();
        delete list;

        for(int z=0; z<sizeZ; z++)
                for(int y=0; y<sizeY; y++)
                        for(int x=0; x<sizeX; x++)
                        {
                                if(img[x][y][z]>0)
                                {
                                        img[x][y][z] = 0;
                                }
                        };

        int x=eX;
        int y=eY;
        int z=eZ;

        int newX=eX;
        int newY=eY;
        int newZ=eZ;

        img[x][y][z] = 255;

        pointsCount = 1;

        int minCode;

        bool found = true;

        while(1)
        {
            minCode=MAXCODE;
            for(int i=-1; i<2; i++)
                for(int j=-1; j<2; j++)
                    for(int h=-1; h<2; h++)
                    {
                        if(i==0 && j==0 && h==0) continue;
                        if(SSfield[x+i][y+j][z+h]!=0 && SSfield[x+i][y+j][z+h] < minCode)
                        {
                            minCode=SSfield[x+i][y+j][z+h];
                            newX=x+i; newY=y+j; newZ=z+h;
                        }
                    }
            // --- SAFETY GUARD ---
            // No labelled neighbour found: the end point is not connected to
            // the start point. Stop cleanly instead of dereferencing an
            // uninitialised coordinate / looping forever.
            if(minCode==MAXCODE)
            {
                found = false;
                break;
            }
            img[newX][newY][newZ]=255;
            pointsCount++;
            x=newX; y=newY; z=newZ;
            if(minCode==1) break;
        }

        for(int h=0;h<sizeX;h++)
        {
                for(int b=0;b<sizeY;b++)
                {
                        delete [] SSfield[h][b];
                }
                delete [] SSfield[h];
        }
        delete [] SSfield;

        return found;
}

int ShortestPath::getPointsCount(void)
{
    return pointsCount;
}

void ShortestPath::addNeigbours(int ***field, int x, int y, int z, int metricF, int metricE, int metricV)
{
        addNeigbour(field, x, y, z, -1, 0, 0, metricF);
        addNeigbour(field, x, y, z, +1, 0, 0, metricF);
        addNeigbour(field, x, y, z, 0, -1, 0, metricF);
        addNeigbour(field, x, y, z, 0, +1, 0, metricF);
        addNeigbour(field, x, y, z, 0, 0, -1, metricF);
        addNeigbour(field, x, y, z, 0, 0, +1, metricF);

        addNeigbour(field, x, y, z, 0, -1, -1, metricE);
        addNeigbour(field, x, y, z, 0, -1, +1, metricE);
        addNeigbour(field, x, y, z, 0, +1, -1, metricE);
        addNeigbour(field, x, y, z, 0, +1, +1, metricE);
        addNeigbour(field, x, y, z, -1, 0, -1, metricE);
        addNeigbour(field, x, y, z, -1, 0, +1, metricE);
        addNeigbour(field, x, y, z, +1, 0, -1, metricE);
        addNeigbour(field, x, y, z, +1, 0, +1, metricE);
        addNeigbour(field, x, y, z, -1, -1, 0, metricE);
        addNeigbour(field, x, y, z, -1, +1, 0, metricE);
        addNeigbour(field, x, y, z, +1, -1, 0, metricE);
        addNeigbour(field, x, y, z, +1, +1, 0, metricE);

        addNeigbour(field, x, y, z, -1, -1, -1, metricV);
        addNeigbour(field, x, y, z, -1, -1, +1, metricV);
        addNeigbour(field, x, y, z, -1, +1, -1, metricV);
        addNeigbour(field, x, y, z, -1, +1, +1, metricV);
        addNeigbour(field, x, y, z, +1, -1, -1, metricV);
        addNeigbour(field, x, y, z, +1, -1, +1, metricV);
        addNeigbour(field, x, y, z, +1, +1, -1, metricV);
        addNeigbour(field, x, y, z, +1, +1, +1, metricV);
}

void ShortestPath::addNeigbour(int ***field, int x, int  y, int z, int d1, int d2, int d3, int metric)
{

    // brakuje zabezpieczenia wokseli na brzegach obrazu!!!
    // (boundary protection is provided by the caller via volume padding)

        if(img[x+d1][y+d2][z+d3] > 0)
        {
                if(field[x][y][z]+metric < field[x+d1][y+d2][z+d3])
                {
                        field[x+d1][y+d2][z+d3] = field[x][y][z]+metric;
                        list->pushBack(x+d1, y+d2, z+d3);
                }
        }
}
