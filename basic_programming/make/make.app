include $(MKDIR)/make.var


CFLAGS  += -DMAKEAPP -g -Wall -I. -I$(SRCDIR) -I$(SRCDIR)/md5lib
LDFLAGS += 

all: $(PROG)

%.o: %.c
	$(CC) $(CFLAGS) $(ECFLAGS) -c $< -o $@


$(PROG): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS) $(ELDFLAGS) $(LDLIBS)

clean:
	rm -rf *.o
	rm -rf $(PROG)

