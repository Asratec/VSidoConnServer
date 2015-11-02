#include "VSidoMotion.hpp"
using namespace VSido;

string forceStrRoot;


#include <thread>
#include <mutex>
#include <iostream>
using namespace std;
#include <unistd.h>


int main(int ac,char* av[])
{
	if(1< ac )
	{
		forceStrRoot = av[1];
	}
	Motion motion;
//	motion.loadXml("dance");
	motion.loadXmlAll();
	thread motionThread(std::ref(motion));
	
	cout << "play one dance" << endl;
	motion.start("dance",false);

	usleep(3*1000*1000);
	cout << "play one dance 3s" << endl;
	motion.start("dance",false);
	
	usleep(20*1000*1000);
	cout << "play at loop" << endl;
	motion.start("",true);

	motionThread.join();
	
}
