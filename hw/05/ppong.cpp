#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> i{1000000};


 void ping(){
 	while(i>1)
 		if(i%2==0){
 			std::cout << "ping" << std::endl;
 			i -= 1;
 		}
}		

void pong(){
	while(i>0)
		if (i%2==1){
			std::cout << "pong" << std::endl;
			i -= 1;
		}
}

int main(){
	std::thread t1(&ping);
	std::thread t2(&pong);
	t1.join();
	t2.join();
	return 0;
}
