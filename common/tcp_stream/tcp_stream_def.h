#pragma once

#ifdef MAPBAR_LINUX
#include <sys/socket.h>
#ifndef SOCKET
#define SOCKET int
#endif
#else
#include <WinSock.h>
#endif

#if !defined(SOCKET_ERROR)
#define SOCKET_ERROR -1
#endif

#if !defined(INVALID_SOCKET)
#define INVALID_SOCKET (SOCKET)(~0)
#endif

enum TcpObjectType
{
	TcpObject_tile = 0,
	TcpObject_tileGroup = 1,
	TcpObject_idle = 2,
	TcpObject_taskPng = 3,
	TcpObject_tilepng = 4,
	TcpObject_groupOver = 5
};

struct TcpObject
{
	TcpObjectType objectType;
	char* buffer;
	unsigned int maxSize;
	unsigned int size;
};

#define MAX_DATASIZE 1024 * 1024 * 2            // ... bytes
#define TCP_PACKET_MAGIC_HEADER "tcp_packet"
#define TCP_PACKET_MAGIC_HEADER_LENGTH 10 // strlen("tcp_packet")

#ifdef WIN32
#define SHOW_LAST_ERROR(...) {\
	LPWSTR errorInfo;\
	if(FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,\
			NULL, WSAGetLastError(), 0,  (LPWSTR)&errorInfo, 0, NULL) == 0) \
		wprintf(L"Format message failed with 0x%x\n", WSAGetLastError());\
	else\
		wprintf(L"%s\n", errorInfo);\
	printf(__VA_ARGS__);\
}
#else
#define SHOW_LAST_ERROR
#endif

