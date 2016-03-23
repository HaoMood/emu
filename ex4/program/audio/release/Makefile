CC = /usr/local/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
TARGET = record
SRC = fbtest.c
$(TARGET): $(TARGET).c $(SRC)
	$(CC) -o $@ $^ -lpthread

clean:
	rm $(TARGET)
