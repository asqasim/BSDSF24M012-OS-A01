CC = gcc
CFLAGS = -Wall -Iinclude
AR = ar
ARFLAGS = rcs

OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib
INCDIR = ../include

LIB_OBJECTS = $(OBJDIR)/mystrfunctions.o $(OBJDIR)/myfilefunctions.o
MAIN_OBJECT = $(OBJDIR)/main.o

STATIC_LIB = $(LIBDIR)/libmyutils.a
TARGET_STATIC = $(BINDIR)/client_static

all: $(TARGET_STATIC)

# Build the static library archive from the module object files
$(STATIC_LIB): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $(STATIC_LIB) $(LIB_OBJECTS)

# Link main.o against the static library
$(TARGET_STATIC): $(MAIN_OBJECT) $(STATIC_LIB)
	$(CC) $(MAIN_OBJECT) -L$(LIBDIR) -lmyutils -o $(TARGET_STATIC)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(STATIC_LIB) $(TARGET_STATIC)

.PHONY: all clean
