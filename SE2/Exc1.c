#include <stdio.h>

void rotate_left(unsigned long value[], size_t n);/*{
	n%=128;
	if(n==0) return;
	long aux;
	if(n>=64){
		aux=value[1];
		value[1]=value[0];
		value[0]=aux;
		if(n!=64) rotate_left(value, n-64);
		return;
	}
	aux = value[0];
	value[0]=value[1]>>(64-n) | value[0]<<n;
	value[1]=aux>>(64-n) | value[1]<<n;
	
}*/



int main(){
	
	long arr[]= {0, 1};
	
	rotate_left(arr, 129);
	
	printf("1: %ld\n", arr[0]);
	printf("2: %ld\n", arr[1]);
	
}
