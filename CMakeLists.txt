cmake_minimum_required(VERSION 3.26)
project(arp_ping)

set(CMAKE_CXX_STANDARD 17)

add_executable(arp_ping src/main.cpp src/arp_sender.cpp)