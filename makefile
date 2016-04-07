all: qrcode

qrcode: main.o 
	g++ main.o  -o qrcode

main.o: main.c
	g++ -c main.c

clean:
	rm -rf *.o qrcode