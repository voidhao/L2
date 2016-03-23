#include <conf/iface.h>
#include <utility>
#include <fstream>
#include <stdio.h>
#include <conf/log.h>

namespace conf{

static const char* ifaces_file = "conf/interface.conf";

string_vector read_iface(){
    string_vector ifaces;

    std::ifstream ifo;
    ifo.open(ifaces_file);
    if(!ifo.is_open()){
        zlog_error(sys_log, "%s not exist", ifaces_file);
        return std::move(ifaces);
    }
    std::string iface;
    while(std::getline(ifo, iface)){
        auto it = std::find(ifaces.begin(), ifaces.end(), iface);
        if (it == ifaces.end()){
            ifaces.push_back(iface);
        }
    }
    ifo.close();
    return std::move(ifaces);
}

} // namespace conf
