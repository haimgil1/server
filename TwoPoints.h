//
// Created by haim gil on 08/01/2017.
//

#ifndef EX4_TWOPOINTS_H
#define EX4_TWOPOINTS_H


#include "Point.h"
#include "AbstractNode.h"
#include "Grid.h"
#include "Searchable.h"

class TwoPoints {
public:

    TwoPoints(const Point &startPoint, const Point &endPoint,
              stack<AbstractNode *> newTrack,
              Searchable *bfs, Grid *map);

    virtual ~TwoPoints();

    const Point &getStartPoint() const;

    const Point &getEndPoint() const;

    stack<AbstractNode *> getTrack() const;

    Searchable *getBfs() const;

    Grid *getMap() const;

    void
    setTrack(stack<AbstractNode *> newTrack);

private:
    Point startPoint;
    Point endPoint;
    stack<AbstractNode *> track;
    Searchable *bfs;
    Grid *map;
};


#endif //EX4_TWOPOINTS_H
