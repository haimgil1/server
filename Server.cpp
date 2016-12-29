#include <iostream>
#include "Udp.h"
#include <unistd.h>
#include "MainFlow.h"
#include "StandardCab.h"


using namespace std;
using namespace boost::iostreams;
using namespace boost::archive;

int main() {
    std::cout << "Hello, from server" << std::endl;


    MainFlow mainFlow;
    try {
        mainFlow.startGame();
    } catch (std::invalid_argument &err){
        cout << err.what();
    }

//    Socket *udp = new Udp (1, 12345);
//    //Udp udp(1, 12345);
//    udp->initialize();
//
//    char buffer[4096];
//    //Node *gp2;
//    //FactoryCab factoryCab;
//    Driver* gp2;
//    udp->reciveData(buffer, sizeof(buffer));
//    char *end = buffer+4095;
//    basic_array_source<char> device(buffer, end);
//    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
//    binary_iarchive ia(s2);
//    ia >> gp2;
//    // cout << oa;
//    //s.flush();
//    //string a = oa;
//    cout << gp2->getCab()->getCabId();
//    cout << gp2->getAge();
//    cout << *(gp2->getcurrentPoint());
//    cout << gp2->getExp();
//    // cout << *gp2;
//    cout << buffer << endl;
//    udp->sendData("sup?");

    // support more than one client?
    return 0;
}