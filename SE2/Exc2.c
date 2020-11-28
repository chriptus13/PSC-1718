#include <stdio.h>


void *memset(void *ptr, int value, size_t len);/*{
	long aux = value&255;
	aux |= aux<<8;
	aux |= aux<<16;
	aux |= aux<<32;
	
	while(len){
		if(len>=8 && !(((unsigned long) ptr)&15)){
			*((long*) ptr) = aux;
			ptr+=8;
			len-=8;
		}else{
			*((char*) ptr++) = (char)aux;
			len--;
		}
	}
}*/

int main(){
	char b[11];
	printf("%s\n", b);
	memset(&b, '?', 8);
	printf("%s\n", b);
}
