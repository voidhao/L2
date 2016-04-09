#include <comm_def.h>
#include <conf/conf.h>
#include <conf/log.h>
#include <conf/SimpleIni.h>

namespace conf{

static const char* conf_file = "conf/main.ini";
static const char* gateway_file = "conf/gateway.ini";

std::string manage_ip;
uint16_t manage_port    = 0;
uint16_t client_port    = 0;
uint16_t switch_port    = 0;
uint16_t min_nat_port   = 0;
uint16_t max_nat_port   = 0;

std::map<std::string, std::vector<std::string>> gateways;	//store gateways from gateway.ini

void dump_conf(){
    zlog_info(sys_log, "------------------------------------");
    zlog_info(sys_log, "manage ip    : %s", manage_ip.c_str());
    zlog_info(sys_log, "manage port  : %d", manage_port);
    zlog_info(sys_log, "client port  : %d", client_port);
    zlog_info(sys_log, "switch_port  : %d", switch_port);
    zlog_info(sys_log, "min nat port : %d", min_nat_port);
    zlog_info(sys_log, "max nat port : %d", max_nat_port);
    zlog_info(sys_log, "------------------------------------");
}

bool read_gateway() {
	CSimpleIniA ini;
	auto err = ini.LoadFile(gateway_file);
	if (err != 0)
	{
		zlog_error(sys_log, "load %s failed", gateway_file);
		return false;
	}
	CSimpleIniA::TNamesDepend sections, keys, values;
	ini.GetAllSections(sections);
	CSimpleIniA::TNamesDepend::const_iterator s, k, v;
	std::vector<std::string> tmp;
	for (s = sections.begin(); s != sections.end(); ++s)
	{
		ini.GetAllKeys(s->pItem, keys);
		for (k = keys.begin(); k != keys.end(); ++k)
		{
			tmp.push_back(ini.GetValue(s->pItem, k->pItem, 0, 0));
		}
		gateways[s->pItem] = tmp;
	}
	return true;
}

void read_conf(){
    CSimpleIniA ini;
    auto err = ini.LoadFile(conf_file);
    if (err != 0){
        zlog_error(sys_log, "load %s failed", conf_file);
        exit(0);
    }

    manage_ip = ini.GetValue("main", "manage_ip", "");
    manage_port = (uint16_t)ini.GetLongValue("main", "manage_port", 0);
    client_port = (uint16_t)ini.GetLongValue("main", "client_port", 0);
    switch_port = (uint16_t)ini.GetLongValue("main", "switch_port", 0);
    min_nat_port = (uint16_t)ini.GetLongValue("main", "min_nat_port", 0);
    max_nat_port = (uint16_t)ini.GetLongValue("main", "max_nat_port", 0);
    dump_conf();
    if (manage_ip.empty() ||
            manage_port == 0 ||
            client_port == 0  ||
            switch_port == 0  ||
            min_nat_port == 0 ||
            max_nat_port == 0 ||
            (min_nat_port >= max_nat_port)){

        zlog_error(sys_log, "%s invalid", conf_file);
        exit(0);
    }
}

} // namespace conf

