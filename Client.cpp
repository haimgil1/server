#include "Tcp.h"
#include "Client.h"

#include "easyloggingpp-8.91/easylogging++.h"
using namespace std;
using namespace boost::iostreams;
using namespace boost::archive;

using namespace std;

int main(int argc, char *argv[]) {
    Client client(argv[2]);
    client.scanDriver();
//    client.sendInt();
//    client.receiveInt();
    client.sendDriver();
    client.receiveCab();
    client.updateDriver();
    return 0;
}

Client::Client() {

}

Client::~Client() {
    delete driver;
    delete tcp;
}

Client::Client(char *argv) {
    this->tcp = new Tcp(0, atoi(argv)); // Set the tcp.
    this->tcp->initialize();
    //this->accept = this->tcp->acceptDescriptorCommunicate();
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
    this->tcp->reciveData(buffer, sizeof(buffer), 1);
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
    this->tcp->sendData(serial_str,1);
}

void Client::updateDriver() {
    while (true) {
        // check if the driver is not null to delete driver.
        if (this->driver != NULL) {
            // check if the point in the driver is not null to delete the point.
            if (this->driver->getcurrentPoint() != NULL) {
                delete this->driver->getcurrentPoint();
            }
            delete this->driver;
            this->driver = NULL;
        }
        // get the driver from the server.
        this->tcp->reciveData(buffer, sizeof(buffer),1);
        basic_array_source<char> device(buffer, end);
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
                device);
        binary_iarchive ia(s2);
        ia >> this->driver;
        LINFO << driver->getId();
        LINFO << *(driver->getcurrentPoint());

        if (this->driver->getId() == -1) { // check if close the client session.
            delete this->driver->getcurrentPoint(); // delete the driver.
            break;
        }
    }
}

void Client::receiveInt() {
    int  num;
    this->tcp->reciveData(buffer, sizeof(buffer),this->tcp->acceptDescriptorCommunicate());
    basic_array_source<char> device(buffer, end);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
            device);
    binary_iarchive ia(s2);
    ia >> num;
    cout<<num;
}

void Client::sendInt() {
    string serial_str;
    int num;
    cin >> num;
    back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    binary_oarchive oa(s);
    oa << num;
    s.flush();
    this->tcp->sendData(serial_str,this->tcp->acceptDescriptorCommunicate());
}