//用作服务器
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	int err;
	int len;
	int ch;
	
	char cSendBuf[1000];				//发送至客户端的字符串
	char cRecvBuf[1000];				//接收客户端的字符串

	SOCKET sockServe;				//定义服务器套接字
	SOCKET sockClient;				//定义客户端套接字
	SOCKADDR_IN addrServe;			//定义服务端地址
	SOCKADDR_IN addrClient;			//定义客户端地址
	
	WSADATA wsaData;				//winsock结构体
	WORD wVersionRequested;			//版本
	
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	
	if (err != 0)
	{
		printf("程序错误！");
		exit(1);
	}

	if ((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2))
	{
		printf("程序错误！");
		WSACleanup();
		exit(1);
	}

	//创建服务端套接字
	sockServe = socket(AF_INET, SOCK_STREAM, 0);

	//给服务端网络地址赋值
	addrServe.sin_family = AF_INET;
	addrServe.sin_port = htons(6000);
	addrServe.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//将网络地址与服务端口绑定
	bind(sockServe, (SOCKADDR*)&addrServe, sizeof(SOCKADDR));
	
	//启用监听
	listen(sockServe, 10);
	printf("服务器已启动！\n可以通话...\n");

	len = sizeof(SOCKADDR);
	while(1)
	{
		sockClient = accept(sockServe, (SOCKADDR*)&addrClient, &len);
		sprintf(cSendBuf, "欢迎连接到我的服务器！通信已连接，可以通话...");
		send(sockClient, cSendBuf, strlen(cSendBuf)+1, 0);
		while(1)
		{	
			recv(sockClient, cRecvBuf, strlen(cRecvBuf), 0);
			printf("%s;%s\n", inet_ntoa(addrClient.sin_addr), cRecvBuf);
			printf(">>");
			scanf("%s", cSendBuf);
			send(sockClient, cSendBuf, strlen(cSendBuf)+1, 0);
		}
	closesocket(sockClient);	
	}
}
