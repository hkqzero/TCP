/*linux socket AF_INET TCP 编程示例，单进程单线程，ehco 服务端*/
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

int main()
{
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serv_addr;
	int serv_addr_len = sizeof(serv_addr);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8123);
	
	if (bind(server_sock, (struct sockaddr*)&serv_addr, serv_addr_len) < 0)
	{
		printf("bind error\n");
		return 0;
	}
	
	if (listen(server_sock, 5)) 
	{
		printf("listen error\n");
		return 0;
	}
	
	char buf[10];
	while (1) 
	{
		struct sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);
		
		printf("等待链接的到来\n");
		int client_sock = accept( server_sock, (struct sockaddr*)&client_addr, (unsigned int *)&client_addr_len);

		if (client_sock == -1) 
		{
			printf("accept error\n");
			return 0;
		}
		printf("有新连接到来\n");
		int n = recv(client_sock, buf, sizeof(buf), 0);
		while (n > 0) 
		{
			printf("%s\n", buf);
			if (send(client_sock, buf, sizeof(buf), 0) < 0) 
			{
				printf("send error\n");
				return 0;
			}
			n = recv(client_sock, buf, sizeof(buf), 0);
		}

		close(client_sock);
		printf("\n\n连接离开\n");
	}

	close(server_sock);
	return 0;
}
