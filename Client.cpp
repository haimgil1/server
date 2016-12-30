#include "Udp.h"
#include "Client.h"

using namespace std;
using namespace boost::iostreams;
using namespace boost::archive;

using namespace std;

int main(int argc, char *argv[]) {
    Client client(argv[1]);
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
    delete cab;
    delete udp;
    delete node;
}

Client::Client(char *argv) {
    this->udp = new Udp(0, atoi(argv));
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
    this->node = new Node(Point(0, 0), NULL);
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
        this->udp->reciveData(buffer, sizeof(buffer));
        basic_array_source<char> device(buffer, end);
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(
                device);
        binary_iarchive ia(s2);
        ia >> this->driver;
        if (this->driver->getcurrentPoint() == NULL) {
            break;
        }
        //cout << *(this->driver->getcurrentPoint());
    }
}
