
AR = ar cq

CFLAGS = -g -I.

OBJS = misc.o mnist_load.o myerror.o neural.o nnsave.o ppm_load.o

LIBNN = libnn.a

all: $(LIBNN)

$(LIBNN): $(OBJS)
	rm -f $(LIBNN)
	$(AR) $(LIBNN) $(OBJS)

clean:
	$(RM) $(OBJS) $(LIBNN)
