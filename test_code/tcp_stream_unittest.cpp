#include "gtest/gtest.h"
#include "pthread/pthread.h"
#include "tcp_stream/tcp_inputstream.h"
#include "tcp_stream/tcp_outputstream.h"
#include <iostream>

using namespace std;

/**
	Tcp Test
	按顺序发包，按顺序收包
*/
void* sender(void*)
{
	SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, 0);
	EXPECT_NE(connectSocket, SOCKET_ERROR);

	sockaddr_in m_their_addr;
	m_their_addr.sin_family = AF_INET;
	m_their_addr.sin_port = htons(9001);
	m_their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int rst = connect(connectSocket, (struct sockaddr*)&m_their_addr, sizeof(struct sockaddr));
	EXPECT_EQ(rst, 0);

	for (int i = 0; i < 1000000; i++)
	{
		TcpObject tcpObject;
		TcpOutputSream outputStream;
		tcpObject.objectType = (TcpObjectType)100;
		tcpObject.buffer = (char*)&i;
		tcpObject.size = sizeof(int);
		outputStream.writeObject(&tcpObject, connectSocket);
	}

	closesocket(connectSocket);
	return NULL;
}

void* receiver(void*)
{
	SOCKET receiverId = socket(AF_INET, SOCK_STREAM, 0);
	EXPECT_NE(receiverId, SOCKET_ERROR);

	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(9001);
	my_addr.sin_addr.s_addr = INADDR_ANY; 

	if (bind(receiverId, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{ 
		return false;
	}

	if (listen(receiverId, 30) == SOCKET_ERROR)
	{ 
		return false; 
	}

	int sin_size = sizeof(struct sockaddr_in); 
	struct sockaddr_in their_addr;   
	
	bool switchOn = true;
	while(switchOn)
	{
		SOCKET senderId = accept(receiverId, (struct sockaddr *)&their_addr, &sin_size);
		EXPECT_NE(senderId, SOCKET_ERROR);

		int preV = -1;
		int V;
		TcpObject obj;
		TcpInputSream inputStream;
		obj.buffer = (char*)&V;
		while(inputStream.readObject(&obj, senderId))
		{
			EXPECT_EQ(V, preV + 1);
			preV++;
		}
		switchOn = false;
	}

	return NULL;
}

TEST(TcpTest, main)
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2,2),&ws);

	pthread_t pid[2];
	pthread_create(&pid[1], NULL, receiver, NULL);
	pthread_create(&pid[0], NULL, sender, NULL);

	pthread_join(pid[0], NULL);
	pthread_join(pid[1], NULL);

	WSACleanup();
}