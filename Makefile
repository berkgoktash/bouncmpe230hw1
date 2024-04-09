ringmaster:		main.o
				gcc main.o -o ringmaster

main.o:			main.c
				gcc -c main.c