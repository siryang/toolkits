#pragma once
#include "tcp_stream_def.h"


class TcpInputSream
{
public:
	TcpInputSream();
	~TcpInputSream();

	bool readObject(TcpObject* o, SOCKET socketId);

private:
	char* m_buffer;
	size_t m_bufferLen;

	static const size_t m_maxBufferSize = MAX_DATASIZE;
	bool unpacketObject(TcpObject* o);
};
