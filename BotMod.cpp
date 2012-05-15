#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClientMod.hpp"
#include "BotMod.hpp"

void bot::botread()
{
	int k;

	do
	{
		Label:
		k=read(sockfd,buffer+weight,1023-weight);
		if (!k)
		{
			printf("%sConnection closed!\n\n",buffer);
			exit(0);
		}
		weight+=k;
		buffer[weight]=0;
		if (weight<2) goto Label;
	} while (buffer[weight-2]!='>');
}

int bot::find()
{
 	int i=0;
	char s[1024];

	while ((buffer[i]<'0')||(buffer[i]>'9')) { ++i; }
	if (i!=0)
	{
		memmove(buffer,buffer+i,weight-=i);
		i=0;
	}
	while ((buffer[i]>='0')&&(buffer[i]<='9'))
	{
		s[i]=buffer[i];
		++i;
	}
	s[i]=0;
	memmove(buffer,buffer+i,weight-=i);
	return strtoint(s);
}

void bot::prod()
{
	if ((raw>=2)&&(money>=2*2000))
	{
		write(sockfd,"prod 2\n",7);
		write(0,"prod 2\n",7);
		botread();
	}
	if ((raw==1)&&(money>=2000))
	{
		write(sockfd,"prod 1\n",7);
		write(0,"prod 1\n",7);
		botread();
	}
	buffer[0]=0;
	weight=0;
}

void bot::market()
{
	write(sockfd,"market\n",7);
	write(0,"market\n",7);
	botread();
	find();find();find();
	mbuy=find();
	find();
	msell=find();
}

void bot::sell()
{
	char s[32];

	if (product!=0)
	{
		sprintf(s,"sell %d %d\n",product,msell);
		write(sockfd,s,strlen(s));
		write(0,s,strlen(s));
		botread();
	}
}

void bot::buy()
{
	char s[32];

	sprintf(s,"buy 2 %d\n",mbuy);
	write(sockfd,s,strlen(s));
	write(0,s,strlen(s));
	botread();
}

void bot::waitnum()
{
	fd_set readfds;
	int i;

	FD_ZERO(&readfds);
	FD_SET(sockfd,&readfds);
	for(;;)
	{
  	if (select(sockfd+1,&readfds,0,0,0)<1)
		{
			perror("select");
			exit(1);
		}
		botread();
		i=0;
		while (buffer[i]!=0)
		{
			if (buffer[i]=='Y') break;
			++i;
		}
		if (buffer[i]=='Y')
		{
			strcpy(buffer,buffer+i);
			weight-=i;
			num=find();
			printf("\nmy num - %d\n\n",num);
			break;
		}
	}
}

void bot::wait()
{
	/*
	for(int i=0;;)
	{
		botread();
		while (buffer[i]!=0)
		{
			if (buffer[i]=='#') break;
			++i;
		}
		if (buffer[i]=='#') break;
	}
	*/
	botread();
	buffer[0]=0;
	weight=0;
}

void bot::editinfo()
{
	char s[16];
	int i=0;

	buffer[0]=0;
	weight=0;
	sprintf(s,"player %d\n",num);
	write(sockfd,s,strlen(s));
	write(0,s,strlen(s));
	botread();
	while (buffer[i]!=' ') { ++i; }
	money=find();
	printf(" money=%d\n",money);
	while (buffer[i]!=' ') { ++i; }
	raw=find();
	printf(" raw materials=%d\n",raw);
	while (buffer[i]!=' ') { ++i; }
	product=find();
	printf(" product=%d\n",product);
	buffer[0]=0;
	weight=0;
}

void bot::doturn()
{
	prod();
	market();
	sell();
	buy();
	buffer[0]=0;
	weight=0;
	write(sockfd,"turn\n",5);
	write(0,"turn\n\n",6);
	botread();
}

void launchbot(int sockfd)
{
	bot mybot(sockfd);

	mybot.waitnum();
	for(;;)
	{
		mybot.editinfo();
		mybot.doturn();
		mybot.wait();
	}
}
