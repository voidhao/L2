cmake_minimum_required(VERSION 2.5)
project(L2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -g -ggdb -std=c++11")

include_directories(
        /usr/local/include
        /opt/local/include
        src
)

link_directories(
        /usr/local/lib
        /opt/local/lib
)

AUX_SOURCE_DIRECTORY("./src/netmap" netmap_files)
AUX_SOURCE_DIRECTORY("./src/nat" nat_files)
AUX_SOURCE_DIRECTORY("./src/switcher" switcher_files)
AUX_SOURCE_DIRECTORY("./src/client" client_files)
AUX_SOURCE_DIRECTORY("./src" root_files)
AUX_SOURCE_DIRECTORY("./src/conf" conf_files)

add_executable(L2 ${netmap_files}
        ${nat_files}
        ${switcher_files}
        ${client_files}
        ${conf_files}
        ${root_files})
target_link_libraries(L2 tins zlog)
