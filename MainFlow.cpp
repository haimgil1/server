
#include "MainFlow.h"
#include "FactoryCab.h"
#include "Tcp.h"
#include <pthread.h>

//#include "easyloggingpp-8.91/easylogging++.h"

//_INITIALIZE_EASYLOGGINGPP


using namespace std;
using namespace boost::iostreams;
using namespace boost::archive;

pthread_mutex_t trackLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clientLock = PTHREAD_MUTEX_INITIALIZER;

MainFlow::MainFlow(char *argv[]) {
    this->cab = NULL;
    this->driver = NULL;
    this->tripInformation = NULL;
    this->map = NULL;
    this->tcp = NULL;
    this->time = 0;
    this->tcp = new Tcp(1, atoi(argv[1])); // Set the port to tcp.
    this->taxiCenter = NULL;
    this->flagCase =0;
    this->tcp->initialize();
    this->isJoiningTripThred= false;
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
                this->numOfDrivers = numOfDrivers;
                //int clientPort = atoi(argv[1]);
                while (numOfDrivers > 0) { // Getting the drivers.
                    //tempDescriptor = tcp->acceptDescriptorCommunicate();
                    pthread_t t1;
                    int status = pthread_create(&t1, NULL, handleThread, (void*) this);
                    if(status) {
                        cout << "thread not create" << endl;
                    }
                    cout << "create client\n";
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
                this->tripInformation->createTripThread();
                //createTripThread(this->tripInformation);
                //taxiCenter.addTripsThread(this->tripThread);
                taxiCenter->addTrip(
                        this->tripInformation); // add the trip to the list of taxi center.
                break;

            case 9:

                // Start the trips.
                this->time++;
                while (true){
                  if (this->numOfDrivers<=0){
                     break;
                  }
                }
//                if (!this->isJoiningTripThred){
//                    joiningThreads();
//                    this->isJoiningTripThred = true;
//                }


                // Make one step and send the update driver to the client.
                taxiCenter->driving(this->time, this->tcp);
                this->flagCase =9;
                //this->flagCase =0;

                break;

            case 4:
                // Print driver location
                cin >> driverId;
                taxiCenter->printDriverLocation(driverId);
                this->flagCase =4;
                //this->flagCase =0;
                break;
            case 7:
                this->driver = taxiCenter->getDriverVec()[0];
                for (int i = 0; i < (int) taxiCenter->getDriverVec().size(); i++) {
                    this->driver->setId(-1); // Prepare to end the client.
                    sendUpdateDriver(this->driver, taxiCenter->getDescriptors()[i]);
                   // taxiCenter->removeDriver(this->driver);
                }

                //pthread_mutex_destroy(&trackLock);
                this->flagCase =7;
                //this->flagCase =0;
                pthread_exit(NULL);
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
    this->numOfDrivers--;
}

Driver* MainFlow::receiveDriver(int descriptor) {
    char buffer[99999];
    tcp->reciveData(buffer, sizeof(buffer), descriptor);
    char *end = buffer + 99998;
    basic_array_source<char> device(buffer, end);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    binary_iarchive ia(s2);
    ia >> this->driver;
    // update the current point to the driver.
    delete this->driver->getcurrentPoint();
    this->driver->setCurrentPoint(this->map->getSourceElement(0, 0));
    return this->driver;
}

void MainFlow::updateObstacles() {
    char dummy;
    int numOfObstacles;
    cin >> numOfObstacles; // Get number of obstacles.

//    AbstractNode* node1 = this->map->getSourceElement(0,0);
//    AbstractNode* node2 = this->map->getSourceElement(1,0);
//    if ( *(node1) != *(node2) ) {
//        cout << "okokoko";
//    }
    while (numOfObstacles > 0) { // Get the points of the obstacles.
        int px, py;
        cin >> px >> dummy >> py;
        this->map->getSourceElement(px, py)->setObstacle(true);
        numOfObstacles--;
    }
}

void MainFlow::createTripThread(TripInformation *trip) {
    int status = pthread_create(trip->getTripThread(), NULL,
                                trip->settingTrack, (void *) trip);
    if (status){
        cout << "Didn't succeed to create thread";
    }
    cout << "createTripThread";
}

void* MainFlow::handleThread(void* mainflow) {

    MainFlow *mainFlow = (MainFlow*) mainflow;
    cout << "before connection\n";
    int descriptor = mainFlow->tcp->acceptDescriptorCommunicate();
    pthread_mutex_lock(&clientLock);
    cout << "after connection\n";
    Driver* driver = mainFlow->receiveDriver(descriptor);
    mainFlow->cab = mainFlow->taxiCenter->findCabById(driver->getCabId());
    driver->setCab(mainFlow->cab);
    mainFlow->sendUpdateCab(mainFlow->cab,descriptor); // Send cab to the client.
    mainFlow->taxiCenter->addDriver(driver);
    mainFlow->taxiCenter->addDescriptor(descriptor);
    pthread_mutex_unlock(&clientLock);


    AbstractNode* node = driver->getcurrentPoint();
    cout<< driver->getId() << endl;
    while(true){
        if(mainFlow->flagCase == 9) {
            //cout << "loop";
            if ((*(driver->getcurrentPoint())!=(*node))) {
                 //send the driver to the client.
                cout << "here9\n";
                mainFlow->sendUpdateDriver(driver, descriptor);
                node = driver->getcurrentPoint();
            }
//            mainFlow->sendUpdateDriver(driver, descriptor);
//            cout << "here9\n";
        }
        if(mainFlow->flagCase == 7) {
            cout << "here7\n";
            break;
        }
    }
}

void MainFlow::joiningThreads() {
    vector<TripInformation *> trips = taxiCenter->getTripQueue();
    for (int i = 0; i < (int) trips.size(); i++) {
        trips[i]->join();
        cout << "join" + i;
    }
}

