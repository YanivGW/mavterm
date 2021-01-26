CC = gcc
CFLAGS = -Wall -g
OBJDIR = objs
INCLUDES = -Isrc \

SRCS = src/main.c \

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