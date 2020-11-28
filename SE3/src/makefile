app: app.o
	gcc app.o -L. -l fut -o app -l jansson -l curl

app.o: app.c
	gcc -c -Wall -g app.c
	
clean:
	rm *.o app *.i
