#include "fibo.h"
#include <string>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]){
	
	/* code */
    if (argc == 2) {
		int value = stoul(argv[1], nullptr, 10);
    	static Fibonacii Fibo(value);
    	Fibo.create_series();
    	Fibo.get_data();
    	return 0;
    }
}
