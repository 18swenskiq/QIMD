#include "xsocket_socket.h"

namespace net {

	socket::socket() {
		fd = -1;
		addrfam = af::inet6;
	}

	socket::socket(af fam, sock socktype) {
		init(fam, socktype);
	}

	socket::socket(af fam, sock socktype, int port) : socket(fam, socktype) {
		int r = bind(port);
		if (r == -1) {
			close();
			fd = -1;
		}
	}

	socket& socket::operator =(int newfd) {
		fd = newfd;
		return *this;
	}

	int socket::init(af fam, sock socktype) {
		fd = ::socket((int)fam, (int)socktype, 0);
		addrfam = fam;
#ifdef XS_NONBLOCKING
		setnonblocking(true);
#endif
		return fd;
	}

	int socket::accept(endpoint* ep) {
		socklen_t al = ep->size();
		int i = ::accept(fd, ep->data(), &al);
		ep->initialize(al, addrfam);
		return i;
	}

	int socket::listen(int n) {
		return ::listen(fd, n);
	}

	int socket::bind(const endpoint ep) {
		return ::bind(fd, ep.data(), ep.size());
	}

	int socket::bind(const std::string addr, int port) {
		return bind(endpoint(addr, port, addrfam));
	}

	int socket::bind(int port) {
		return bind("0", port);
	}

	int socket::connect(const endpoint ep) {
		return ::connect(fd, ep.data(), ep.size());
	}

	std::size_t socket::sendto(const char* data, std::size_t len, endpoint ep) {
		return ::sendto(fd, data, len, 0, ep.data(), ep.size());
	}

	std::size_t socket::sendto(const std::vector<char>& data, endpoint ep) {
		return sendto(data.data(), data.size(), ep);
	}

	std::size_t socket::sendto(const std::string* data, endpoint ep) {
		return sendto(data->c_str(), data->size(), ep);
	}

	std::size_t socket::recvfrom(char* buf, std::size_t len, endpoint* ep) {
		socklen_t al = ep->size();
		int i = ::recvfrom(fd, buf, len, 0, ep->data(), &al);
		ep->initialize(al, addrfam);
		return i;
	}

	std::size_t socket::recvfrom(std::vector<char>& buf, endpoint* ep) {
		return recvfrom(buf.data(), buf.size(), ep);
	}

	std::size_t socket::recvfrom(std::string* buf, std::size_t len, endpoint* ep) {
		std::vector<char> buffer(len);
		int r = recvfrom(buffer, ep);
		if (r > 0)
			*buf = std::string(buffer.data(), r);
		return r;
	}

	std::size_t socket::send(const char* data, std::size_t len) {
		return ::send(fd, data, len, 0);
	}

	std::size_t socket::send(const std::vector<char>& data) {
		return send(data.data(), data.size());
	}

	std::size_t socket::send(const std::string* data) {
		return send((char*)data->c_str(), data->size());
	}

	std::size_t socket::recv(char* buf, std::size_t len) {
		return ::recv(fd, buf, len, 0);
	}

	std::size_t socket::recv(std::vector<char>& buf) {
		return recv(buf.data(), buf.size());
	}

	std::size_t socket::recv(std::string* buf, std::size_t len) {
		std::vector<char> buffer(len);
		int r = recv(buffer);
		if (r > 0)
			*buf = std::string(buffer.data(), r);
		return r;
	}

	int socket::close() {
#ifndef _WIN32
		return ::close(fd);
#else
		return ::closesocket(fd);
#endif
	}

	int socket::shutdown(shut how) {
		return ::shutdown(fd, (int)how);
	}

	int socket::setnonblocking(bool block) {
#ifndef _WIN32
		return fcntl(fd, F_SETFL, O_NONBLOCK, block);
#else
		DWORD nb = (int)block;
		return ioctlsocket(fd, FIONBIO, &nb);
#endif
	}

	// TODO: implement the timeout for windows
	int socket::settimeout(int sec, int us) {
#ifndef _WIN32
		timeval tv;
		tv.tv_sec = sec;
		tv.tv_usec = us;
		int i = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		return i;
#else
		return -1;
#endif
	}

	endpoint socket::getlocaladdr()	const {
		return getname(fd, getsockname, addrfam);
	}

	endpoint socket::getremoteaddr() const {
		return getname(fd, getpeername, addrfam);
	}

	int socket::geterror() const {
		return errno;
	}

	bool socket::good() const {
		if (fd != -1)
			return true;
		return false;
	}
	int fd;
	af addrfam;

}