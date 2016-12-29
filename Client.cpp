//
// Created by yanaiela on 12/10/16.
//

#include "Udp.h"
#include <unistd.h>
#include "MainFlow.h"
#include "StandardCab.h"
#include "FactoryCab.h"

using namespace std;
using namespace boost::iostreams;
using namespace boost::archive;

using namespace std;

int main(int argc, char *argv[]) {
    int mission;
    char buffer[4096];
    char dummy;
    int driverId, vehicleId;
    double age, experience;
    char status;
    cin >> driverId >> dummy >> age >> dummy >> status >> dummy >> experience >> dummy
        >> vehicleId;
    AbstractNode *node = new Node(Point(0,0),NULL);
    MaritalStatus maritalStatus = MaritalStatus(status);
    Driver *driver = new Driver (driverId, age, maritalStatus, experience, vehicleId,node);

    string serial_str;
    back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    binary_oarchive oa(s);
    oa << driver;
    s.flush();



    std::cout << "Hello, from client" << std::endl;

    //cout << argv[1] << endl;
    Socket *udp = new Udp(0, atoi(argv[1]));
    udp->initialize();
    udp->sendData(serial_str);

    while (mission!=7) {


        udp->reciveData(buffer, sizeof(buffer));
        char *end = buffer + 4095; //todo
        basic_array_source<char> device(buffer, end);
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
                device);
        binary_iarchive ia(s2);
        ia >> node;
        driver->setCurrentPoint(node);
        cout << *(driver->getcurrentPoint());
    }
   // cout << buffer << endl;

    return 0;
}