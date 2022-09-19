bootsect:bootsect.exec
	objcopy -O binary bootsect.exec bootsect

bootsect.exec:bootsect.o
	gcc -fno-builtin -nostdinc -nostdlib -Wl,-N -Wl,-Ttext -Wl,0x0 \
	-o bootsect.exec bootsect.o

bootsect.o:bootsect.S
	gcc -fno-builtin -nostdinc -c bootsect.S
