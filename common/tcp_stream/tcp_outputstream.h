#pragma once
#include "tcp_stream_def.h"

class TcpOutputSream
{
public:
	TcpOutputSream();
	~TcpOutputSream();

	bool writeObject(TcpObject* o, SOCKET socketId);

private:
	char* m_buffer;
	unsigned int packetObject(TcpObject* o);
};
