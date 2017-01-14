
#include "MainFlow.h"
#include "FactoryCab.h"
#include "Tcp.h"
#include <pthread.h>

#include "easyloggingpp-8.91/easylogging++.h"

_INITIALIZE_EASYLOGGINGPP


using namespace std;
using namespace boost::iostreams;
using namespace boost::archive;

pthread_mutex_t trackLock = PTHREAD_MUTEX_INITIALIZER;

MainFlow::MainFlow(char *argv[]) {
    this->cab = NULL;
    this->driver = NULL;
    this->tripInformation = NULL;
    this->map = NULL;
    this->tcp = NULL;
    this->tripThread = NULL;
    this->time = 0;
    this->tcp = new Tcp(1, atoi(argv[1])); // Set the port to tcp.
    for (int i= 0; i< 10;i++){
        descriptorVec[i]=-1;
    }
    this->taxiCenter = NULL;
    this->flagCase =0;
}

MainFlow::~MainFlow() {
    delete tcp;
    delete map;
    delete taxiCenter;
}

void MainFlow::startGame() {

    int sizeX, sizeY, driverId;
    char dummy;
    int numOfDrivers;
    cin >> sizeX >> sizeY; // Get the n,m of the map.
    this->map = this->MapParser(sizeX, sizeY);
    updateObstacles();
    taxiCenter= new TaxiCenter(this->map); // Create taxi center.
    int mission; // the num of choice in the menu.

    do {
        cin >> mission;
        switch (mission) {

            case 3: // Get cab parameters.
                int cabId, cabType;
                char manufacturer, color1;
                cin >> cabId >> dummy >> cabType >> dummy >> manufacturer >> dummy >> color1;
                this->cab = this->cabParser(cabId, cabType, manufacturer, color1);
                taxiCenter->addCab(this->cab);
                break;

            case 1:  // Get driver parameters.
                cin >> numOfDrivers;
                //int clientPort = atoi(argv[1]);
                tcp->initialize();
                while (numOfDrivers > 0) { // Getting the drivers.
                    tempDescriptor = tcp->acceptDescriptorCommunicate();
                    pthread_t t1;
                    int status = pthread_create(&t1, NULL, handleThread, (void*) this);
                    if(status) {
                        cout << "thread not create" << endl;
                    }
                    cout << "create" << endl;
/*                    receiveDriver(tempDescriptor);
                    this->cab = taxiCenter.findCabById(this->driver->getCabId());
                    this->driver->setCab(this->cab);
                    sendUpdateCab(this->cab,tempDescriptor); // Send cab to the client.
                    taxiCenter.addDriver(this->driver);*/
                    numOfDrivers--;
                }
                break;

            case 2:  // Get trip parameters.
                int tripId, startX, startY, endX, endY, numOfPassengers;
                double tariff, time;
                cin >> tripId >> dummy >> startX >> dummy >> startY >> dummy >> endX >> dummy
                    >> endY >> dummy >> numOfPassengers >> dummy >> tariff >> dummy >> time;

                // Trip can't be negative and more than map coordinate.
                if (startX >= sizeX || startY >= sizeY || startX < 0 || startY < 0
                    || endX >= sizeX || endY >= sizeY || endX < 0 || endY < 0) {
                    throw invalid_argument("wrong trip range\n");
                }

                this->tripInformation = this->tripInfoParser(tripId, startX, startY, endX, endY,
                                                             numOfPassengers, tariff,
                                                             this->map, time);
                createTripThread(this->tripInformation);
                //taxiCenter.addTripsThread(this->tripThread);
                taxiCenter->addTrip(
                        this->tripInformation); // add the trip to the list of taxi center.
                break;

            case 9:
                // Start the trips.
                this->time++;
                // Make one step and send the update driver to the client.
                taxiCenter->driving(this->time, this->tcp ,descriptorVec);
                this->flagCase =9;

                break;

            case 4:
                // Print driver location
                cin >> driverId;
                taxiCenter->printDriverLocation(driverId);
                this->flagCase =4;
                break;
            case 7:
                for (int i= 0; i< 10;i++){
                    if (descriptorVec[i]==-1){
                        continue;
                    }
                    this->driver = taxiCenter->getDriverVec()[0];
                    this->driver->setId(-1); // Prepare to end the client.
                    sendUpdateDriver(this->driver, descriptorVec[i]);
                    taxiCenter->removeDriver(this->driver);
                }
                //pthread_mutex_destroy(&trackLock);
                this->flagCase =7;

                return;
            default:
                throw invalid_argument("invalid number of mission\n");
        }
    } while (mission != 7);
}

Cab *MainFlow::cabParser(int cabId, int cabType, char manufacturer, char color1) {
    CarManufacturer carManufacturer = CarManufacturer(manufacturer);
    Color color = Color(color1);
    FactoryCab factoryCab;
    return factoryCab.getCab(cabType, cabId, carManufacturer, color);
}

Driver *MainFlow::driverParser(int driverId, double age, char status, double experience,
                               int cabId) {
    MaritalStatus maritalStatus = MaritalStatus(status);
    return new Driver(driverId, age, maritalStatus, experience, cabId,
                      this->map->getSourceElement(0, 0));
}

TripInformation *MainFlow::tripInfoParser(int tripId, int startX, int startY, int endX, int endY,
                                          int numOfPassengers, double tariff,
                                          Grid *map, double time) {
    Point startP(startX, startY);
    Point endP(endX, endY);
    return new TripInformation(tripId, startP, endP, numOfPassengers, tariff, map, time, trackLock);
}

Grid *MainFlow::MapParser(int n, int m) {
    return new Matrix(n, m);
}

void MainFlow::sendUpdateDriver(Driver *driver,int descriptor) {
    string serial_str;
    back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    binary_oarchive oa(s);
    oa << driver;
    s.flush();
    tcp->sendData(serial_str,descriptor);
}

void MainFlow::sendUpdateCab(Cab *cab,int descriptor) {
    string serial_str;
    back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    binary_oarchive oa(s);
    oa << cab;
    s.flush();
    tcp->sendData(serial_str,descriptor);
}

Driver* MainFlow::receiveDriver(int descriptor) {
    char buffer[4096];
    tcp->reciveData(buffer, sizeof(buffer), descriptor);
    char *end = buffer + 4095;
    basic_array_source<char> device(buffer, end);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    binary_iarchive ia(s2);
    ia >> this->driver;
    // update the current point to the driver.
    delete this->driver->getcurrentPoint();
    this->driver->setCurrentPoint(this->map->getSourceElement(0, 0));
    descriptorVec[this->driver->getId()] = descriptor ;
    return this->driver;
}

void MainFlow::updateObstacles() {
    char dummy;
    int numOfObstacles;
    cin >> numOfObstacles; // Get number of obstacles.
    while (numOfObstacles > 0) { // Get the points of the obstacles.
        int px, py;
        cin >> px >> dummy >> py;
        this->map->getSourceElement(px, py)->setObstacle(true);
        numOfObstacles--;
    }
}

void MainFlow::createTripThread(TripInformation *trip) {
    int status = pthread_create(&trip->getTripThread(), NULL,
                                trip->settingTrack, (void *) trip);
    if (status){
        cout << "Not good";
    }
    LINFO << "createTripThread";
}

void* MainFlow::handleThread(void* mainflow) {

    MainFlow *mainFlow = (MainFlow*) mainflow;
    pthread_mutex_lock(&trackLock);
    Driver* driver=mainFlow->receiveDriver(mainFlow->tempDescriptor);
    mainFlow->cab = mainFlow->taxiCenter->findCabById(driver->getCabId());
    driver->setCab(mainFlow->cab);
    mainFlow->sendUpdateCab(mainFlow->cab,mainFlow->tempDescriptor); // Send cab to the client.
    mainFlow->taxiCenter->addDriver(driver);
    pthread_mutex_unlock(&trackLock);

/*    int d;
    for(int i=0; i<mainFlow->taxiCenter->getDriverVec().size(); i++){
        if(mainFlow->taxiCenter->getDriverVec()[i] == )
    }*/

    AbstractNode* node = driver->getcurrentPoint();
    cout<< driver->getId() << endl;
    while(true){
        if(mainFlow->flagCase == 9) {
            if (!(driver->getcurrentPoint()->operator==(*node))) {
                 //send the driver to the client.
                mainFlow->sendUpdateDriver(driver, mainFlow->descriptorVec[driver->getId()]);
                node = driver->getcurrentPoint();
                cout << "here9" << endl;
            }
        }

        if(mainFlow->flagCase == 7) {
            cout << "here7" <<endl;
            break;
        }
    }
}