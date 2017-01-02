//
// Created by haim gil on 30/12/2016.
//

#ifndef EX4_CLIENT_H
#define EX4_CLIENT_H

#include "Driver.h"
#include "Socket.h"
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

class Client {
public:
    Client();

    Client(char *argv);

    virtual ~Client();

    void scanDriver();

    void sendDriver();

    void receiveCab();

    void updateDriver();
private:
    Socket *udp;
    char buffer[4096];
    char *end;
    Driver* driver;
    Cab* cab;
    AbstractNode* node;


};

#endif //EX4_CLIENT_H
