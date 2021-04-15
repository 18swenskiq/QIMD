#pragma once
#include "xsocket_init.hpp"
#include "xsocket_include.h"
#include "xsocket_endpoint.h"

namespace net {
	class socket {
	public:
		socket();
		socket(af fam, sock socktype);
		socket(af fam, sock socktype, int port);

		socket& operator =(int newfd);

		int init(af fam, sock socktype);
		int accept(endpoint* ep);
		int listen(int n);
		int bind(const endpoint ep);
		int bind(const std::string addr, int port);
		int bind(int port);
		int connect(const endpoint ep);

		std::size_t sendto(const char* data, std::size_t len, endpoint ep);
		std::size_t sendto(const std::vector<char>& data, endpoint ep);
		std::size_t sendto(const std::string* data, endpoint ep);
		std::size_t recvfrom(char* buf, std::size_t len, endpoint* ep);
		std::size_t recvfrom(std::vector<char>& buf, endpoint* ep);
		std::size_t recvfrom(std::string* buf, std::size_t len, endpoint* ep);
		std::size_t send(const char* data, std::size_t len);
		std::size_t send(const std::vector<char>& data);
		std::size_t send(const std::string* data);
		std::size_t recv(char* buf, std::size_t len);
		std::size_t recv(std::vector<char>& buf);
		std::size_t recv(std::string* buf, std::size_t len);

		int close();
		int shutdown(shut how);
		int setnonblocking(bool block);

		// TODO: implement the timeout for windows
		int settimeout(int sec, int us);

		endpoint getlocaladdr()	const;
		endpoint getremoteaddr() const;

		int geterror() const;
		bool good() const;

	private:

		// getname calls getsockname/getpeername and returns it as an endpoint type
		template< typename t >
		inline endpoint getname(int fd, t target, af fam) const {
			endpoint ep;
			socklen_t al = ep.size();
			target(fd, ep.data(), &al);
			ep.initialize(al, fam);
			return ep;
		}

		int fd;
		af addrfam;
	};
}