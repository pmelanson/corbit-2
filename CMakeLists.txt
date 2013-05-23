cmake_minimum_required (VERSION 2.6 FATAL_ERROR)
project (Corbit)


#custom cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules"})

#this file has all our source files listed
include(FileList)

include_directories (${CMAKE_SOURCE_DIR}/include)


#check dependencies
find_package (ALLEGRO5 REQUIRED)
include_directories (${ALLEGRO_INCLUDE_DIR})
#set (LIBS ${LIBS} ${ALLEGRO_LIBRARIES})

find_package(Boost 1.36.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include(CheckCXXSourceCompiles)
set(CMAKE_CXX_FLAGS "-Wextra -Wall -std=c++11 ${ALLEGRO_LIBRARIES}")

message("${CMAKE_CXX_FLAGS}")

add_executable(Corbit ${CORBIT_SRC_FILES})
