PROGRAMS   = outcalls incalls # tagnacht 

.IGNORE:

DEBUGFLAGS =  -g 

OBJECTS    = outcalls.o incalls.o txtutils.o # tagnacht.o

all:	$(PROGRAMS)

%.o: %.c ../is1.h
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o $@ -c $<

#txtutils.o: txtutils.c is1.h
#	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o $@ -c $<

outcalls: $(OBJECTS)
	$(CC) $(DEBUGFLAGS) -o $@ outcalls.o txtutils.o ../commutils.o ../checksum.o

incalls: $(OBJECTS)
	$(CC) $(DEBUGFLAGS) -o $@ incalls.o txtutils.o ../commutils.o ../checksum.o

# tagnacht: $(OBJECTS)
# 	$(CC) $(DEBUGFLAGS) -o $@ tagnacht.o txtutils.o ../commutils.o ../checksum.o

install: $(PROGRAMS)

clean:
	@rm core *.o *.ad.h *.log* $(PROGRAMS)

