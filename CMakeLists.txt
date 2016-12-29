cmake_minimum_required(VERSION 3.6)
project(Ex4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x ")

#add_subdirectory(lib/googletest-master)
#include_directories(lib/googletest-master/googletest/include)
#include_directories(lib/googletest-master/googlemock/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SOURCE_FILES Bfs.cpp Bfs.h Node.cpp Node.h Point.cpp Point.h
        Grid.h Matrix.cpp Matrix.h AbstractNode.cpp AbstractNode.h Cab.cpp
        Cab.h LuxuryCab.cpp LuxuryCab.h CarManufacturer.h Color.h StandardCab.cpp
        StandardCab.h FactoryCab.cpp FactoryCab.h Tests/CabTest.cpp Tests/CabTest.cpp
        Tests/Defines.h Driver.cpp Driver.h MaritalStatus.h Passenger.cpp Passenger.h
        TripInformation.cpp TripInformation.h Tests/DriverTest.cpp Tests/PassengerTest.cpp
        Tests/TripInfoTest.cpp Tests/NodeTest.cpp TaxiCenter.cpp TaxiCenter.h Tests/MatrixTest.cpp
        Tests/BfsTest.cpp Tests/TaxiCenterTest.cpp Tests/PointTest.cpp MainFlow.cpp MainFlow.h
        Grid.cpp Searchable.cpp Searchable.h Tests/MainFlowTest.cpp Udp.cpp Udp.h Tcp.h Tcp.cpp
        Socket.h Socket.cpp)
add_executable(EX2 ${SOURCE_FILES} Tests/CabTest.cpp Tests/CabTest.cpp Driver.cpp Driver.h
        MaritalStatus.h Passenger.cpp Passenger.h TripInformation.cpp
        TripInformation.h Tests/DriverTest.cpp Tests/PassengerTest.cpp Tests/TripInfoTest.cpp
        Tests/NodeTest.cpp TaxiCenter.cpp TaxiCenter.h Tests/PointTest.cpp MainFlow.cpp
        MainFlow.h Grid.cpp Tests/MainFlowTest.cpp Socket.cpp Udp.h Udp.cpp
        Socket.h Tcp.cpp Tcp.h)

set(BOOST_ROOT CACHE PATH "/usr/include/boost/")
find_package(Boost 1.57.0 COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIR})

add_executable(Client ${SOURCE_FILES} Client.cpp Driver.cpp Cab.cpp AbstractNode.cpp Node.cpp
        Point.h Color.h CarManufacturer.h MaritalStatus.h)
add_executable(Server ${SOURCE_FILES} Server.cpp Bfs.h Node.cpp Node.h Point.cpp Point.h
        Grid.h Matrix.cpp Matrix.h AbstractNode.cpp AbstractNode.h Cab.cpp
        Cab.h LuxuryCab.cpp LuxuryCab.h CarManufacturer.h Color.h StandardCab.cpp
        StandardCab.h FactoryCab.cpp FactoryCab.h Tests/CabTest.cpp Tests/CabTest.cpp
        Tests/Defines.h Driver.cpp Driver.h MaritalStatus.h Passenger.cpp Passenger.h
        TripInformation.cpp TripInformation.h Tests/DriverTest.cpp Tests/PassengerTest.cpp
        Tests/TripInfoTest.cpp Tests/NodeTest.cpp TaxiCenter.cpp TaxiCenter.h Tests/MatrixTest.cpp
        Tests/BfsTest.cpp Tests/TaxiCenterTest.cpp Tests/PointTest.cpp MainFlow.cpp MainFlow.h
        Grid.cpp Searchable.cpp Searchable.h Tests/MainFlowTest.cpp Udp.cpp Udp.h Tcp.h Tcp.cpp
        Socket.h Socket.cpp)
target_link_libraries(Client ${Boost_LIBRARIES})
target_link_libraries(Server ${Boost_LIBRARIES})

target_link_libraries(EX2 gtest gtest_main)


