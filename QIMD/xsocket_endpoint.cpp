#include "xsocket_endpoint.h"

namespace net {
	endpoint::endpoint() {
		memset(&addr, 0, sizeof(sockaddr_storage));
		addrlen = sizeof(sockaddr_storage);
	}

	endpoint::endpoint(int port) {
		set("0", port);
	}

	endpoint::endpoint(std::string ip, int port) {
		set(ip, port);
	}

	endpoint::endpoint(std::string ip, int port, af fam) {
		set(ip, port, fam);
	}

	bool endpoint::operator== (const endpoint& e) {
		if (memcmp(&addr, e.data(), e.size()) == 0)
			return true;
		return false;
	}

	bool endpoint::operator!= (const endpoint& e) {
		return !operator==(e);
	}

	std::vector<endpoint> endpoint::resolve(const char* host, const char* service, af f) {
		//set up our addrinfo hints for the getaddrinfo call
		addrinfo hints;
		memset(&hints, 0, sizeof(addrinfo));
		hints.ai_family = (int)f;
		hints.ai_socktype = 0;

		//if host is nullptr, we need to set AI_PASSIVE, meaning its meant for binding
		if (host == nullptr)
			hints.ai_flags = AI_PASSIVE;

		addrinfo* res, * rp;

		int i = getaddrinfo(host, service, &hints, &res);
		if (i != 0) {
			//std::cout << gai_strerror( i ) << std::endl;
			return std::vector<endpoint>();
		}

		std::vector<endpoint> buffer;

		for (rp = res; rp != nullptr; rp = rp->ai_next) {
			endpoint ep;
			memcpy(&ep.addr, rp->ai_addr, rp->ai_addrlen);
			ep.initialize(rp->ai_addrlen, (af)rp->ai_family);

			if (std::find(buffer.begin(), buffer.end(), ep) == buffer.end())
				buffer.push_back(ep);
		}

		freeaddrinfo(res);

		return buffer;
	}

	void endpoint::set(std::string ip, int port, af f) {
		const char* host = ip.c_str();
		if (ip == "0")
			host = nullptr;
		std::vector<endpoint> epList = endpoint::resolve(host, std::to_string(port).c_str(), f);
		*this = epList[0];
	}

	void endpoint::initialize(socklen_t alen, af fam) {
		addrlen = alen;
		addrfam = fam;
	}

	std::string endpoint::get_host(int flags) const {
		char hostbuf[INET6_ADDRSTRLEN];
		getnameinfo(data(), size(), &hostbuf[0], INET6_ADDRSTRLEN, nullptr, 0, flags);
		return &hostbuf[0];
	}

	std::string endpoint::get_ip() const {
		return get_host(NI_NUMERICHOST);
	}

	std::string endpoint::get_service(int flags) const {
		char servbuf[INET6_ADDRSTRLEN];
		getnameinfo(data(), size(), nullptr, 0, &servbuf[0], INET6_ADDRSTRLEN, flags);
		return &servbuf[0];
	}

	int endpoint::get_port() const {
		return std::atoi(get_service(NI_NUMERICSERV).c_str());
	}

	af endpoint::get_af() const {
		return addrfam;
	}

	sockaddr* endpoint::data() const {
		return (sockaddr*)&addr;
	}

	int endpoint::size() const {
		return addrlen;
	}

	std::string endpoint::to_string() const {
		std::stringstream ss;
		ss << get_ip() << ":" << get_port();
		return ss.str();
	}
}