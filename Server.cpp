#include <iostream>
#include "Udp.h"
#include <unistd.h>
#include "MainFlow.h"
#include "StandardCab.h"


using namespace std;
using namespace boost::iostreams;
using namespace boost::archive;

int main(int argc, char *argv[]) {
    MainFlow mainFlow;
    try {
        mainFlow.startGame(argc, argv);
    } catch (std::invalid_argument &err){
        cout << err.what();
    }
    return 0;
}