
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
  //  client.receiveTrip();
    client.updateDriver();

    return 0;
}

Client::Client() {

}

Client::~Client() {
    delete driver;
    delete cab;
    delete udp;
    delete node;
    delete trip;
}

Client::Client(char *argv) {
    this->udp = new Udp(0, atoi(argv)); // set the udp by the port from the argv.
    this->udp->initialize();
    this->end = buffer + 4095;
    this->driver = NULL;
    this->cab = NULL;
    this->node = new Node(Point(0, 0), NULL);
    this->trip = NULL;
    this->mission = 0;

}

void Client::scanDriver() {
    char dummy;
    int driverId, vehicleId;
    double age, experience;
    char status;
    // Get the parameters from the user.
    cin >> driverId >> dummy >> age >> dummy >> status >> dummy >> experience >> dummy
        >> vehicleId;
    this->node = new Node(Point(0, 0), NULL);
    MaritalStatus maritalStatus = MaritalStatus(status);
    // Create the driver object.
    this->driver = new Driver(driverId, age, maritalStatus, experience, vehicleId, node);
}

void Client::receiveCab() {
    // receve the cab from the server.
    this->udp->reciveData(buffer, sizeof(buffer));
    basic_array_source<char> device(buffer, end);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
            device);
    binary_iarchive ia(s2);
    ia >> this->cab;
    this->driver->setCab(this->cab);
}

void Client::sendDriver() {
    // send the driver to the server.
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
        if (this->trip == NULL) {

            this->mission = this->receiveMission();
            if (mission == 9) {
                this->receiveTrip();
            }
            if (mission == 7) {
                break;
            }
        } else if (*(this->driver->getcurrentPoint()) == this->trip->getEndPoint()) {
            this->mission = this->receiveMission();
            if (mission == 7) {
                break;
            }
/*            if (this->driver->getcurrentPoint() == NULL) {
                break;
            }*/
           // cout << mission;
           // if (mission == 9) {
/*            if(this->trip != NULL) {
                delete trip;
            }*/
                this->trip = NULL;
            //    cout << "aa";
           // }

        }
            // Recieve the driver from the
        else {
/*            if(this->driver != NULL){
                delete driver;
            }*/
            this->udp->reciveData(buffer, sizeof(buffer));
            basic_array_source<char> device(buffer, end);
            boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
                    device);
            binary_iarchive ia(s2);
            ia >> this->driver;
            this->trip->setDriver(this->driver);
            //cout << *(this->driver->getcurrentPoint());
            // End the program if the current point is NULL.
/*       if (this->driver->getcurrentPoint() == NULL) {
            break;
        }*/
            if (mission == 7) {
                break;
            }
        }
    }
}

void Client:: receiveTrip() {

        // receve the trip from the server.
    this->udp->reciveData(buffer, sizeof(buffer));
    basic_array_source<char> device(buffer, end);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
            device);
    binary_iarchive ia(s2);
    ia >> this->trip;
    //cout << this->trip->getEndPoint();

}

int Client:: receiveMission() {

    // receve the trip from the server.
    this->udp->reciveData(buffer, sizeof(buffer));
    basic_array_source<char> device(buffer, end);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
            device);
    binary_iarchive ia(s2);
    ia >> this->mission;
    return mission;
}