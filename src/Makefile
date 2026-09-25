CC = gcc
CFLAGS = -Wall -Iinclude
OBJDIR = ../obj
BINDIR = ../bin
INCDIR = ../include

OBJECTS = $(OBJDIR)/mystrfunctions.o $(OBJDIR)/myfilefunctions.o $(OBJDIR)/main.o

TARGET = $(BINDIR)/client

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

.PHONY: all clean
