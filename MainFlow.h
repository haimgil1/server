
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
    // Default constructor.
    MainFlow();
    // start the game to get the parameters and run the game.
    void startGame(int argc, char *argv[]);
    // destructor.
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
    // The function send driver from the server to the client.
    void sendUpdateDriver(Driver *driver);
    // The function send cab from the server to the client.
    void sendUpdateCab(Cab *cab);
    // The function receive driver from the client.
    void receiveDriver();
    // The function update the obstacle in the map.
    void updateObstacles();

};


#endif //TASK2BFS_MAINFLOW_H
