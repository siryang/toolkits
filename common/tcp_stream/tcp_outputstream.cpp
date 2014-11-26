#include "tcp_outputstream.h"
#include "stdio.h"

typedef unsigned int uint32;

TcpOutputSream::TcpOutputSream()
{
	m_buffer = (char*)malloc(MAX_DATASIZE);
}

TcpOutputSream::~TcpOutputSream()
{
	free(m_buffer);
}

bool TcpOutputSream::writeObject(TcpObject* o, SOCKET socketId)
{
	uint32 packetLen = packetObject(o);
	if (send(socketId, m_buffer, (int)packetLen, 0) != (int)packetLen) 
	{		
#ifdef MAPBAR_WIN32
		SHOW_LAST_ERROR("send error\n");
#else
		printf("send error\n");
#endif
		return false;
	}
	return true;
}

uint32 TcpOutputSream::packetObject(TcpObject* o)
{
	int packetLen;
	memcpy(m_buffer, TCP_PACKET_MAGIC_HEADER, TCP_PACKET_MAGIC_HEADER_LENGTH);
	packetLen = TCP_PACKET_MAGIC_HEADER_LENGTH;

	memcpy(m_buffer + packetLen, &(o->size), sizeof(uint32));
	packetLen += sizeof(uint32);

	memcpy(m_buffer + packetLen, &(o->objectType), sizeof(TcpObjectType));
	packetLen += sizeof(TcpObjectType);

	if (o->size > 0)
	{
		memcpy(m_buffer + packetLen, o->buffer, o->size);
		packetLen += o->size;
	}
	return packetLen;
}
