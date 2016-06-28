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

//处理链接
int Work(int sockfd);

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
	
	
	struct sockaddr_in client_addr;
	int client_addr_len = sizeof(client_addr);
	pid_t childpid;
	while (1) 
	{
		printf("等待链接的到来\n");
		int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, (unsigned int *)&client_addr_len);

		if (client_sock == -1) 
		{
			printf("accept error\n");
			return 0;
		}

		printf("有新连接到来\n");

		//子进程
		if((childpid = fork()) == 0)
		{
			close(server_sock);
			Work(client_sock);

			exit(0);
		}

		close(client_sock);
	}

	close(server_sock);
	return 0;
}

int Work(int sockfd)
{
	char buf[10];
	int n = recv(sockfd, buf, sizeof(buf), 0);
	while (n > 0) 
	{
		printf("%s", buf);
		if (send(sockfd, buf, sizeof(buf), 0) < 0) 
		{
			printf("send error\n");
			return 0;
		}
		n = recv(sockfd, buf, sizeof(buf), 0);
	}
	printf("\n");

	return 0;

}
