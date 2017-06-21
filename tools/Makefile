CC = cc
INSTALL = /usr/sbin/install
MAKE = make
RM = rm -f
TOUCH = touch
WBUILD = wbuild -n

INSTALLDIR = /usr/local/neural-kit

CFLAGS = -g -I../src -I. -I/usr/X11R6/include
LDFLAGS = -L../src -lnn -lm
X_LDFLAGS = $(LDFLAGS) -L/usr/X11R6/lib -lXext -lXmu -lXt -lX11 -lXaw
WBUILDFLAGS = 

DEPS = ../src/libnn.a

X_OBJS = Neur.o interf.o

TOOLS = nnview mnist_extract hand_digit

PROGS = $(TOOLS)

all: $(TOOLS)

nnview:  $(X_OBJS) nnview.o $(DEPS)
	$(CC) -o nnview nnview.o $(X_OBJS) $(X_LDFLAGS)

mnist_extract: mnist_extract.o $(DEPS)
	$(CC) -o mnist_extract mnist_extract.o $(LDFLAGS)

hand_digit:  hand_digit.o $(DEPS)
	$(CC) -o hand_digit hand_digit.o $(LDFLAGS)

fproto:
	$(TOUCH) *.c
	do_proto -I$(LIBLSEDIR) -I/usr/X11R6/include -I. *.h

clean:
	$(RM) $(PROGS) *.o

install: $(PROGS)
	$(INSTALL) -m 0711 -u root -g bin -f $(INSTALLDIR)/bin $(PROGS)

.SUFFIXES: .w

.w.c:
	$(WBUILD) $(WBUILDFLAGS) $<

.c.o:
	$(CC) $(CFLAGS) -c $<
