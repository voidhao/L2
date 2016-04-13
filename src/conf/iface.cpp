#include <conf/iface.h>
#include <utility>
#include <fstream>
#include <stdio.h>
#include <conf/log.h>
#include "SimpleIni.h"

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

ifaces read_ifaces(){
	ifaces out;
	CSimpleIniA ini;
	auto err = ini.LoadFile(ifaces_file);
	if (err != 0){
		zlog_error(sys_log, "load %s failed", ifaces_file);
		return out;
	}
	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);
	for(auto it = sections.begin(); it != sections.end(); it++){
		auto v = ini.GetValue(it->pItem, "gateway");
		try{
			IPv4Address ip(v);
			struct iface iface;
			iface.name_ = it->pItem;
			iface.gateway_ = ip;
			out.push_back(iface);
		}catch(std::exception&){
			zlog_error(sys_log, "%s gateway %s invalid", it->pItem, v);
		}
	}
	return out;
}

} // namespace conf
