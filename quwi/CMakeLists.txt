cmake_minimum_required(VERSION 3.16.0)

project(quwi)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS 
Widgets 
Network 
REQUIRED)

add_executable(quwi-client
    main.cpp
    
    http-auth.cpp
    http-auth.h
)

target_link_libraries(quwi-client Qt5::Widgets Qt5::Network)