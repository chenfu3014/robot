#// g++ main.cpp netComm.cpp stroeData.cpp -o robotQQServer	
robotQQServer:main.o netComm.o stroeData.o
	gcc main.o netComm.o stroeData.o -o robotQQServer.linux	
main.o:main.c
	gcc -c main.c -o main.o
netComm.o:netComm.cpp netComm.h
	gcc -c netComm.cpp -o netComm.o
stroeData.o:stroeData.cpp stroeData.h	
   gcc -c stroeData.cpp -o stroeData.o
