# target
TARGET=menu
# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=    main.o

#all: $(OBJS)
#	$(LD) src/main.c -o $(TARGET) $(OBJS) $(LDFLAGS)
all: $(OBJS)
	$(CC) src/main.c -o $(TARGET) $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o pending src/pending.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o series src/series.c $(WARN) $(GTKLIB) -export-dynamic
    
main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o
    
clean:
	rm -f *.o $(TARGET)
