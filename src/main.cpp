
#include <init.h>
#include <net/net.h>
#include <unistd.h>
#include <net/nic/nic.h>
int main(int argc, char** argv) {
    string_vector tmp;
    std::string ifa("eth0");
    tmp.push_back(ifa);
    net::nic::nic_manager mgr;
    mgr.update_nic(tmp);
    init();
    net::init();
    net::start();
    sleep(30000);
    return 0;
}
