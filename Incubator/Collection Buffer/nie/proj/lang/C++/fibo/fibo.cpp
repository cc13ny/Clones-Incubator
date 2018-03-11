#include "fibo.h"
#include <iostream>
#include <vector>
using namespace std;

// This creates a Fibonacii series
void Fibonacii::create_series(void){
	data.push_back(0);
    data.push_back(1);
    for (int i = 2; i < size; ++i){
    	/* code */
    	data.push_back(data[i - 2] + data[i - 1]);
    	}
}
    
    // This is a constructor
void Fibonacii::Fibonacii(int s){
	size = s;
}
    
    // This method is used to print the series
void Fibonacii::get_data(void){
    for (long i: data)
    	cout << i << endl;
}
