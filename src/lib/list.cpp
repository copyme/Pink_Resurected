/*!
 * \file list.cpp
 * \author Michal Postolski
 */

#include "list.h"
List::List(void)
{
head = new struct node;
actual = head;
end = actual;
length = 0;
}
List::List(int x, int y, int z)
{
head = new struct node;
actual = head;
end = actual;
length = 0;
pushBack(x, y, z);
}
void List::pushBack(int x, int y, int z)
{
//theMsg->printf("CCL: before allocation\n");  //DEBUG
struct node *tmp = new struct node;
//theMsg->printf("CCL: after allocation\n");  //DEBUG
tmp->next=NULL;
tmp->x=x;
tmp->y=y;
tmp->z=z;
end->next=tmp;
end=tmp;
length++;
}
int List::getLength(void)
{
return length;
}
void List::next(void)
{
actual = actual->next;
}
bool List::isEnd(void)
{
if(actual->next == NULL) return true;
return false;
}
void List::clear(void)
{
struct node *tmp = new struct node;
actual = head;
while(!isEnd())
        {
tmp = actual;
next();
delete tmp;
        }
head = new struct node;
actual = head;
end = actual;
length = 0;
}
int List::getX(void)
{
return actual->x;
}
int List::getY(void)
{
return actual->y;
}
int List::getZ(void)
{
return actual->z;
}
List::~List(void)
{
if(length != 0) clear();
}
