CC	=	/usr/local/arm-2009q3/bin/arm-none-linux-gnueabi-gcc

fbtest: fbtest.c main_gb.c
	$(CC) -o $@ $^ -lm -fsigned-char

clean:
	\rm -f *.o fbtest
