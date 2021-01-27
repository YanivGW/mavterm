CC = gcc
CFLAGS = -Wall -g
OBJDIR = objs
INCLUDES = \
-Isrc \
-Isrc/drivers \
-Isrc/mavlink \
-Isrc/mavlink/autogen \
-Isrc/mavlink/autogen/common \

SRCS = \
src/main.c \
src/drivers/uart_mock.c \
src/mavlink/mavlink_terminal.c \

OBJS = $(SRCS:.c=.o)

TARGET = mavterm

.PHONY: depend clean

all:    $(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) - *.o *~ $(TARGET)

depend: $(SRCS)
	makedepend $(INCLUDES) $^