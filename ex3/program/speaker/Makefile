obj-m	=	driver.o
KDIR	=	/lib/modules/`uname -r`/build
PWD		=	`pwd`
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean	

speaker: speaker.c
	gcc -o $@ $^
