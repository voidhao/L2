#ifndef _CONF_CONF_H_
#define _CONF_CONF_H_

namespace conf{
    extern std::string manage_ip;
    extern uint16_t manage_port;
    extern uint16_t client_port;
    extern uint16_t switch_port;
    extern uint16_t min_nat_port;
    extern uint16_t max_nat_port;

    void read_conf();
}

#endif /* _CONF_CONF_H_ */
