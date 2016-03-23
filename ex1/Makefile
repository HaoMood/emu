CC = /usr/local/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
CFLAGS = 

TARGET = hello
OBJS = $(TARGET).o

all: $(TARGET)

$(TARGET) : $(TARGET).o
	$(CC) $(CFLAGS) -o $@ $^ -static
$(TARGET).o : $(TARGET).c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	\rm -f $(OBJS) $(TARGET) *.elf *.gdb
	
