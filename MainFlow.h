
#ifndef TASK2BFS_MAINFLOW_H
#define TASK2BFS_MAINFLOW_H

#include <iostream>
#include <string.h>
#include "Cab.h"
#include "TripInformation.h"
#include "Driver.h"
#include "Passenger.h"
#include "TaxiCenter.h"
#include <stdexcept>
#include "Socket.h"
#include "Udp.h"

using namespace std;

class MainFlow {

private:
    Cab *cab;
    Driver *driver;
    TripInformation *tripInformation;
    Grid *map;
    double time;
    Socket *udp;
public:
    MainFlow();

    void startGame();

    virtual ~MainFlow();

    // The function gets parameters and return a cab.
    Cab *cabParser(int cabId, int cabType, char manufacturer, char color1);

    // The function gets parameters and return a driver.
    Driver *driverParser(int driverId, double age, char status, double experience,
                         int cabId);

    // The function gets parameters and return a trip information.
    TripInformation *tripInfoParser(int tripId, int startX, int startY, int endX, int endY,
                                    int numOfPassengers, double tariff,
                                    Grid *map,double time);

    // The function gets parameters and return a map.
    Grid *MapParser(int n, int m);

    void sendUpdateDriver(Driver *driver);

    void sendUpdateCab(Cab *cab);

    void receiveDriver();

    void updateObstacles();
};


#endif //TASK2BFS_MAINFLOW_H
