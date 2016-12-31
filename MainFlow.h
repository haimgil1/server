
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
/*
 * The class - MainFlow.
 * The class gets from the user the parameter,
 * and creates the cab, trip, map objects.
 */
class MainFlow {

private:
    Cab *cab;
    Driver *driver;
    TripInformation *tripInformation;
    Grid *map;
    double time;
    Socket *udp;
public:
    // Default conatructor.
    MainFlow();
    // Start the game to get all the parameters from the user.
    void startGame();
    // Destructor.
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
    // The function sends driver to the client.
    void sendUpdateDriver(Driver *driver);
    // The function sends cab to the client.
    void sendUpdateCab(Cab *cab);
    // The function receives driver from the client.
    void receiveDriver();
    // The function update the obstacles.
    void updateObstacles();
    void sendUpdateTrip(TripInformation *trip);
    void sendMission(int mission);

    };


#endif //TASK2BFS_MAINFLOW_H
