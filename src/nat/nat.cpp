#include <nat/nat.h>
#include <common/errors.h>
#include <nat/nat_session.h>
#include <netinet/in.h>
#include <nat/session_manager.h>
//#include <netmap/netmap.hpp>
namespace nat {

namespace detail{
	std::atomic_ushort ip_id{uint16_t(time(NULL))};
}


using nat_ptr = std::shared_ptr<nat_session>;

nat::nat(){
}

nat::~nat() {
}

nat_ptr nat::get_nat_ptr(uint32_t client, uint32_t path, IP& ip){
	auto out_key = detail::out_key_from_ip(client, ip);
	auto nat = nat_mgr_.find_nat(out_key);
	if(!nat){
		nat = make_nat_ptr(client, path, ip);
		nat_mgr_.add_nat(nat);
	}
	return nat;
}

error_t nat::nat_client_pkt(uint32_t client_id, uint32_t path, IP& ip){
	auto nat = get_nat_ptr(client_id, path, ip);

	ip.src_addr(nat->local_ip_);
	ip.dst_addr(nat->remote_ip_);
	ip.id(detail::get_ip_id());
	auto tcp = ip.find_pdu<TCP>();
	if(tcp){
		tcp->sport(nat->local_port_);
	}
	auto udp = ip.find_pdu<UDP>();
	if(udp){
		udp->sport(nat->local_port_);
	}
	auto data = ip.serialize();
//	lonlife::netmap::sendto(IPv4Address("10.0.0.167"), data);


	return err::SUCCESS;
}

nat_ptr	nat::make_nat_ptr(uint32_t client, uint32_t path, const IP& ip){
	auto nat = std::make_shared<nat_session>();
	nat->local_ip_ = "10.0.0.167";
	nat->origin_ip_ = ip.src_addr();
//	nat->remote_ip_ = ip.dst_addr();
	nat->remote_ip_ = IPv4Address("10.0.0.105");
	nat->proto_ = ip.protocol();
	nat->client_id_ = client;
	nat->path_ = path;
	fill_ports(nat, ip);


	return nat;
}

void nat::fill_ports(nat_ptr nat, const IP& ip){
	auto proto = ip.protocol();
	if(proto == IPPROTO_UDP){
		auto pdu = ip.find_pdu<UDP>();
		if(pdu){
			nat->origin_port_ = pdu->sport();
			nat->remote_port_ = pdu->dport();
			nat->local_port_ = port_mgr_.get_udp_port();
		}
		return;
	}
	if(proto == IPPROTO_TCP){
		auto pdu = ip.find_pdu<TCP>();
		if(pdu){
			nat->origin_port_ = pdu->sport();
			nat->remote_port_ = pdu->dport();
			nat->local_port_ = port_mgr_.get_tcp_port();
		}
		return;
	}
}

} /* namespace nat */
