#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QList>
#include "figure3d.h"

class GraphNode
{
public:
    GraphNode();

    Point3D coords;
    double radius;


    QList<GraphNode*> nodes;


};

#endif // GRAPHNODE_H
