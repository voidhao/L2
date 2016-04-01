cmake_minimum_required(VERSION 3.5)
project(L2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        /usr/local/include
        /opt/local/include
        src
)

link_directories(
        /usr/local/lib
        /opt/local/lib
)

set(SOURCE_FILES
    src/client/client.h
    src/client/client_manager.cpp
    src/client/client_manager.h
    src/client/pkt_handler.cpp
    src/client/pkt_handler.h
    src/client/session.cpp
    src/client/session.h
    src/common/errors.h
    src/common/limits.h
    src/common/reassembler.h
    src/common/util.h
    src/common/vxlan.h
    src/conf/conf.cpp
    src/conf/conf.h
    src/conf/ConvertUTF.c
    src/conf/ConvertUTF.h
    src/conf/iface.cpp
    src/conf/iface.h
    src/conf/log.cpp
    src/conf/log.h
    src/conf/SimpleIni.h
    src/dataflow/meta_data.h
    src/netmap/arp.hpp
    src/netmap/device.hpp
    src/netmap/devices.hpp
    src/netmap/fwd.hpp
    src/netmap/listener.hpp
    src/netmap/netmap.hpp
    src/netmap/nic.hpp
    src/netmap/send.hpp
    src/netmap/statistics.hpp
    src/switcher/switcher.cpp
    src/switcher/switcher.h
    src/switcher/tuple.h
    src/comm_def.h
    src/global.h
    src/init.cpp
    src/init.h
    src/main.cpp)

add_executable(L2 ${SOURCE_FILES})
target_link_libraries(L2 tins zlog)
