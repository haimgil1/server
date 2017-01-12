
#include "TwoPoints.h"

TwoPoints::~TwoPoints() {

}

const Point &TwoPoints::getStartPoint() const {
    return startPoint;
}

const Point &TwoPoints::getEndPoint() const {
    return endPoint;
}

 stack<AbstractNode *> TwoPoints::getTrack() const {
    return track;
}

Searchable *TwoPoints::getBfs() const {
    return bfs;
}

Grid *TwoPoints::getMap() const {
    return map;
}

TwoPoints::TwoPoints(const Point &startPoint, const Point &endPoint,
                     stack<AbstractNode *> newTrack,
                     Searchable *bfs, Grid *map) : startPoint(startPoint), endPoint(endPoint),
                                                   track(newTrack), bfs(bfs), map(map) {}

void TwoPoints::setTrack(
        stack<AbstractNode *> newTrack) {
    track = newTrack;
}
