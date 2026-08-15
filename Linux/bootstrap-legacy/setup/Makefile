setup:setup.exec
	objcopy	-O binary setup.exec setup

setup.exec:setup.o main.o
	gcc -fno-builtin -nostdinc -nostdlib -Wl,-N -Wl,-Ttext -Wl,0x1000 \
	-o setup.exec setup.o main.o
 
setup.o:setup.S
	gcc -fno-builtin -nostdinc -c setup.S

main.o:main.c
	gcc -fno-builtin -nostdinc -s -c -o main.o main.c
