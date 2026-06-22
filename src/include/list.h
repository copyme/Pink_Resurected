#ifndef LIST_H
#define LIST_H

/*!
 * \file list.h
 * \brief Singly-linked list / FIFO queue of 3D voxel coordinates.
 * \author Michal Postolski
 *
 * Companion of the ShortestPath operator (also by M. Postolski).
 */

#include <stdlib.h>
class List
{
struct node
            {
int x, y, z;
struct node *next;
            };
int length;
struct node *actual;
struct node *head;
struct node *end;
public:
List(void);
List(int x, int y, int z);
            ~List(void);
void pushBack(int x, int y, int z);
int getLength(void);
void next(void);
bool isEnd(void);
void clear(void);
int getX(void);
int getY(void);
int getZ(void);
};
#endif // LIST_H
