CC	=	/usr/local/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
SRC = circle.c fbinit.c point.c rectangle.c
OBJ = ${SRC:%.c=%.o}

fbtest: main.c
	$(CC) -o $@ $^ -L ./ -lfb

.c.o:
	$(CC) -c $^ -fPIC

libfb.so: $(OBJ)
	$(CC)  -o $@ -shared -fPIC $^

libfb.a: $(OBJ)
	ar cr $@ $^ 

clean:
	rm -f *.o fbtest
