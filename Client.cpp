#include "Udp.h"
#include "Client.h"

using namespace std;
using namespace boost::iostreams;
using namespace boost::archive;

using namespace std;

int main(int argc, char *argv[]) {
    Client client(argv[2]);
    client.scanDriver();
    client.sendDriver();
    client.receiveCab();
    client.updateDriver();

    return 0;
}

Client::Client() {

}

Client::~Client() {
    delete driver;
    delete udp;
}

Client::Client(char *argv) {
    this->udp = new Udp(0, atoi(argv)); // Set the udp.
    this->udp->initialize();
    this->end = buffer + 4095;
    this->driver = NULL;
    this->cab = NULL;
    this->node = new Node(Point(0, 0), NULL);
}

void Client::scanDriver() {
    char dummy;
    int driverId, vehicleId;
    double age, experience;
    char status;
    cin >> driverId >> dummy >> age >> dummy >> status >> dummy >> experience >> dummy
        >> vehicleId;
    MaritalStatus maritalStatus = MaritalStatus(status);
    this->driver = new Driver(driverId, age, maritalStatus, experience, vehicleId, node);
}

void Client::receiveCab() {
    this->udp->reciveData(buffer, sizeof(buffer));
    basic_array_source<char> device(buffer, end);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
            device);
    binary_iarchive ia(s2);
    ia >> this->cab;
    this->driver->setCab(this->cab);
}

void Client::sendDriver() {
    string serial_str;
    back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    binary_oarchive oa(s);
    oa << this->driver;
    s.flush();
    this->udp->sendData(serial_str);
}

void Client::updateDriver() {
    while (true) {
        // check if the driver is not null to delete driver.
        if(this->driver!=NULL) {
            // check if the point in the driver is not null to delete the point.
            if(this->driver->getcurrentPoint() != NULL) {
                delete this->driver->getcurrentPoint();
               }
            delete this->driver;
            this->driver = NULL;
        }
        // get the driver from the server.
        this->udp->reciveData(buffer, sizeof(buffer));
        basic_array_source<char> device(buffer, end);
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
                device);
        binary_iarchive ia(s2);
        ia >> this->driver;
        if (this->driver->getId() == -1) { // check if close the client session.
            delete this->driver->getcurrentPoint(); // delete the driver.
            break;
        }
    }
}
