#pragma once
#include "xsocket_include.h"

namespace net {
	class endpoint {
	public:
		// Constructors
		endpoint();
		endpoint(int port);
		endpoint(std::string ip, int port);
		endpoint(std::string ip, int port, af fam);
		bool operator== (const endpoint& e);
		bool operator!= (const endpoint& e);

		//returns a vector of all possible endpoints for host:port for the specified sock_type and address family
		std::vector<endpoint> resolve(const char* host, const char* service, af f);
		void set(std::string ip, int port, af f = af::unspec);

		//must be called after you manually write into addr
		void initialize(socklen_t alen, af fam);

		std::string get_host(int flags = 0) const;
		std::string get_ip() const;
		std::string get_service(int flags = 0) const;

		int get_port() const;
		af get_af() const;
		sockaddr* data() const;
		int size() const;

		std::string to_string() const;

		sockaddr_storage addr;
		socklen_t addrlen;
		af addrfam;
	};
}