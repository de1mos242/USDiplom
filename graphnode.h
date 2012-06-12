#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QList>

class GraphNode
{
public:
    GraphNode();

    QList<double> coords;


    QList<GraphNode*> nodes;


};

#endif // GRAPHNODE_H
