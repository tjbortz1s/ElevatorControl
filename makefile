ElevatorControl.exe : ElevatorControl.o types.o queuefunctions.o threadfunctions.o controlfunctions.o
	gcc -pthread ElevatorControl.o types.o queuefunctions.o threadfunctions.o controlfunctions.o -o ElevatorControl.exe
	rm ElevatorControl.o types.o queuefunctions.o threadfunctions.o controlfunctions.o
ElevatorControl.o : ElevatorControl.c
	gcc -pthread -c ElevatorControl.c -o ElevatorControl.o
types.o : types.h types.c
	gcc -pthread -c types.c -o types.o
queuefunctions.o : queuefunctions.h queuefunctions.c
	gcc -pthread -c queuefunctions.c -o queuefunctions.o
threadfunctions.o : threadfunctions.h threadfunctions.c
	gcc -pthread -c threadfunctions.c -o threadfunctions.o
controlfunctions.o : controlfunctions.h controlfunctions.c
	gcc -pthread -c controlfunctions.c -o controlfunctions.o
