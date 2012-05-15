#ifndef BOTMOD_HPP
#define BOTMOD_HPP

class bot
{
	int sockfd;

	int num;
	int money;
	int product;
	int raw;

	int msell;
	int mbuy;

	char *buffer;
	int weight;

	void botread();
	int find();
	void prod();
	void market();
	void sell();
	void buy();

	public:

	bot(int fd) :sockfd(fd) { buffer=new char[1024]; buffer[0]=0; weight=0; }
	void waitnum();
	void wait();
	void editinfo();
	void doturn();
	~bot() { if (buffer) delete[] buffer; }
};

void launchbot(int);

#endif
