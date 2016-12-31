//
// Created by haim gil on 30/12/2016.
//

#ifndef EX4_CLIENT_H
#define EX4_CLIENT_H

#include "Driver.h"
#include "Socket.h"
#include "TripInformation.h"
#include <ostream>
#include <stdexcept>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

/* The class: Client.
 * The client class is run the main of client class.
 * The client sent a driver object to the server.
 * The client get some objects from the server too.
 * */
class Client {
public:
    // Default constructor.
    Client();
    // The main of the client that get the port to the server.
    Client(char *argv);
    // Destructor.
    virtual ~Client();
    // The function gets from the user the driver parameters,
    // and make the driver object.
    void scanDriver();
    // the function sends the driver object to the server.
    void sendDriver();
    // The function recives the cab from the server.
    void receiveCab();
    void receiveTrip();
    // The function update the driver after the server operates the trip.
    void updateDriver();
    int receiveMission();
        private:
    // Members.
    Socket *udp;
    char buffer[4096];
    char *end;
    Driver* driver;
    Cab* cab;
    AbstractNode* node;
    TripInformation *trip;
    int mission;
};

#endif //EX4_CLIENT_H
