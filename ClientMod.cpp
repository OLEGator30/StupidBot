#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "ClientMod.hpp"
#include "BotMod.hpp"

int strtoint(char *s)
{
	unsigned int i=0,k=0;

	for (;i<strlen(s);i++)
	{
	  if ((s[i]>='0')&&(s[i]<='9')) k=k*10+s[i]-'0';
	  else return (-1);
	}
	return k;
}

sock::sock()
{
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
}

void sock::joinserv(int port,char* servip)
{
	struct sockaddr_in addr;

	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	if(!inet_aton(servip,&(addr.sin_addr)))
	{
		perror("inet_aton");
		exit(1);
	}
	if(0!=connect(sockfd,(struct sockaddr*)&addr,sizeof(addr)))
	{
		perror("connect");
		exit(1);
	}
}

int sock::getfd()
{
	return sockfd;
}

int main(int argc,char** argv)
{
	sock mysock;

	if(argc==3)
	{
		int port;

		if ((port=strtoint(argv[2]))==-1)
		{
		  printf("\nWrong parametr (port)!\n\n");
			exit(1);
		}
		mysock.joinserv(port,argv[1]);
		printf("\nConnection was established\n"); 
		launchbot(mysock.getfd()); // goto BotMod
		printf("\nGame was over\n\n");
	}
	else
	{
		printf("\nWrong number of parametrs!\n\n");
		exit(1);
	}
  return 0;
}

