cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (Corbit)

#corbit uses c++11
set(CMAKE_CXX_FLAGS	"-Wall -std=c++11")


#custom cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake"})


#check dependencies

#find allegro 5
find_package (ALLEGRO5 REQUIRED)
include_directories(${ALLEGRO_INCLUDE_DIR})
set(LIBS ${LIBS} ${ALLEGRO_LIBRARIES})

#find Eigen 3
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

#find JsonCpp
find_package(JsonCpp REQUIRED)
include_directories(${JSONCPP_INCLUDE_DIR})
set(LIBS ${LIBS} ${JSONCPP_LIBRARIES})

#find boost
find_package(Boost 1.53.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#add Corbit stuff
include_directories(${CMAKE_SOURCE_DIR}/include)


set(Corbit_SRCS
	${CMAKE_SOURCE_DIR}/src/camera.cpp
	${CMAKE_SOURCE_DIR}/src/graphics.cpp
	${CMAKE_SOURCE_DIR}/src/physical.cpp
	${CMAKE_SOURCE_DIR}/src/calc.cpp
	${CMAKE_SOURCE_DIR}/src/entity.cpp
	${CMAKE_SOURCE_DIR}/src/hab.cpp
	${CMAKE_SOURCE_DIR}/src/main.cpp
	${CMAKE_SOURCE_DIR}/src/nav.cpp)

file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR}) 

add_executable(Corbit ${Corbit_SRCS})

target_link_libraries(Corbit ${LIBS})
