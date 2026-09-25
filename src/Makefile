CC = gcc
CFLAGS = -Wall -Iinclude
AR = ar
ARFLAGS = rcs

OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib
INCDIR = ../include

# Objects for static library (no -fPIC needed)
LIB_OBJECTS = $(OBJDIR)/mystrfunctions.o $(OBJDIR)/myfilefunctions.o

# Objects for dynamic library (must be position-independent)
PIC_OBJECTS = $(OBJDIR)/mystrfunctions_pic.o $(OBJDIR)/myfilefunctions_pic.o

MAIN_OBJECT = $(OBJDIR)/main.o

STATIC_LIB     = $(LIBDIR)/libmyutils.a
DYNAMIC_LIB    = $(LIBDIR)/libmyutils.so
TARGET_STATIC  = $(BINDIR)/client_static
TARGET_DYNAMIC = $(BINDIR)/client_dynamic

all: $(TARGET_STATIC) $(TARGET_DYNAMIC)

# ---- Static build ----
$(STATIC_LIB): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $(STATIC_LIB) $(LIB_OBJECTS)

$(TARGET_STATIC): $(MAIN_OBJECT) $(STATIC_LIB)
	$(CC) $(MAIN_OBJECT) -L$(LIBDIR) -lmyutils -o $(TARGET_STATIC)

# ---- Dynamic build ----
$(DYNAMIC_LIB): $(PIC_OBJECTS)
	$(CC) -shared -o $(DYNAMIC_LIB) $(PIC_OBJECTS)

$(TARGET_DYNAMIC): $(MAIN_OBJECT) $(DYNAMIC_LIB)
	$(CC) $(MAIN_OBJECT) -L$(LIBDIR) -lmyutils -o $(TARGET_DYNAMIC)

# ---- Object rules ----
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR)/%_pic.o: %.c
	$(CC) $(CFLAGS) -fPIC -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(STATIC_LIB) $(DYNAMIC_LIB) $(TARGET_STATIC) $(TARGET_DYNAMIC)

PREFIX = /usr/local
BINDIR_INSTALL = $(PREFIX)/bin
MANDIR_INSTALL = $(PREFIX)/share/man/man1

install: $(TARGET_DYNAMIC)
	install -d $(BINDIR_INSTALL)
	install -m 755 $(TARGET_DYNAMIC) $(BINDIR_INSTALL)/client
	install -d $(MANDIR_INSTALL)
	install -m 644 ../man/man3/*.1 $(MANDIR_INSTALL)
	install -d /etc/ld.so.conf.d
	echo "$(shell pwd)/../lib" > /etc/ld.so.conf.d/libmyutils.conf
	ldconfig

uninstall:
	rm -f $(BINDIR_INSTALL)/client
	rm -f $(MANDIR_INSTALL)/mystrlen.1 $(MANDIR_INSTALL)/mystrcpy.1 \
	      $(MANDIR_INSTALL)/mystrncpy.1 $(MANDIR_INSTALL)/mystrcat.1 \
	      $(MANDIR_INSTALL)/wordCount.1 $(MANDIR_INSTALL)/mygrep.1 \
	      $(MANDIR_INSTALL)/client.1
	rm -f /etc/ld.so.conf.d/libmyutils.conf
	ldconfig


.PHONY: all clean install uninstall
