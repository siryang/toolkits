#include "tcp_inputstream.h"
#include "stdio.h"
typedef unsigned int uint32;

TcpInputSream::TcpInputSream()
{
	m_buffer = (char*)malloc(MAX_DATASIZE);
	m_bufferLen = 0;
}

TcpInputSream::~TcpInputSream()
{
	free(m_buffer);
}

bool TcpInputSream::readObject(TcpObject* o, SOCKET socketId)
{
	int receiveLen;
	
	while( !unpacketObject(o) ) 
	{
		// third param in recv is size_t in posix-c
		receiveLen = recv(socketId, m_buffer + m_bufferLen ,(int)(m_maxBufferSize - m_bufferLen), 0);
		if (receiveLen == 0)
		{
			printf("connect closed\n");
			return false;
		}
		else if(receiveLen == SOCKET_ERROR)
		{ 
#ifdef MAPBAR_WIN32
			SHOW_LAST_ERROR("receive error\n");
#else
			printf("receive error\n");
#endif
			return false;
		}

		m_bufferLen += receiveLen;
	}

	return true;
}

bool TcpInputSream::unpacketObject(TcpObject* obj)
{
	static const size_t fixedLen = TCP_PACKET_MAGIC_HEADER_LENGTH + sizeof(int) + sizeof(TcpObjectType);

	if (m_bufferLen < fixedLen)
		return false;

	if (strncmp(m_buffer, TCP_PACKET_MAGIC_HEADER, strlen(TCP_PACKET_MAGIC_HEADER)) != 0){
		return false;
	}

	char* p = m_buffer + TCP_PACKET_MAGIC_HEADER_LENGTH;
	uint32 packLen = *(uint32*)p;
	// received entire one packet.
	if (m_bufferLen < fixedLen + packLen)
		return false;

	p += sizeof(uint32);

	obj->objectType = *(TcpObjectType*)p;
	p += sizeof(TcpObjectType);

	obj->size = packLen;
	memcpy(obj->buffer, p, obj->size);

	size_t remainderLen = m_bufferLen - (fixedLen + packLen);
	memmove(m_buffer, m_buffer + (packLen + fixedLen), remainderLen);
	m_bufferLen = remainderLen;
	return true;
}

