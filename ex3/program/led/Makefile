obj-m	=	driver.o
CC      =   /usr/local/arm-2009q3/bin/arm-none-linux-gnueabi-gcc 
LD      =   /usr/local/arm-2009q3/bin/arm-none-linux-gnueabi-ld
KDIR	=	/home/student/kernel-3.0
PWD		=	`pwd`
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean	

led:	led.c
	$(CC) -o $@ $^
