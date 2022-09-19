image:boot/bootsect setup/setup
	cat boot/bootsect setup/setup > image

boot/bootsect:boot/bootsect.S
	cd boot ; make 

setup/setup:setup/setup.S setup/main.c
	cd setup ; make
