#ifndef CLIENTMOD_HPP
#define CLIENTMOD_HPP

class sock
{
	int sockfd;

	public:

	sock();
	void joinserv(int,char*);
	int getfd();
};

int strtoint(char*);
int main(int,char**);

#endif
