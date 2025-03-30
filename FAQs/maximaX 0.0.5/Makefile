# Uncomment this for using "editres"
# USE_EDITRES	=	-DUSE_EDITRES

# Where should all the default files go
CONFIGPATH	=	/usr/local/maximaX/etc

# Group and user that are allowed to use maximaX
EXECGROUP	=	bin
EXECUSER	=	root

# Uncomment this for debugging the code
# DEBUGFLAGS	=	-g

# Where should the binary go
INSTALL_BIN	=	/usr/X11R6/bin

# Where should the app_defaults go
INSTALL_AD	=	/usr/X11R6/lib/X11/app-defaults

#################################################################
# There should be nothing to change beyond this line
#################################################################

SYSTEM		=	`uname -s`
ifeq ($(SYSTEM),FreeBSD)
	LOCKDIR	=	/var/spool/lock
else
	LOCKDIR	=	/var/lock
	XTRALD	=	-Wl,-rpath-link,/usr/X11R6/lib
endif

LDEFINES	=	$(USE_EDITRES) -I/usr/X11R6/include

ifdef USE_EDITRES
	XMULIB	=	-lXmu
endif

XLIBEXT		=	-L/usr/X11R6/lib -lXpm -lXmt -lXm -lXt -lX11 -lXext \
				$(XMULIB) $(XTRALD)

SXLIBEXT	=	-L/usr/X11R6/lib /usr/X11/lib/libXmt.a /usr/X11/lib/libXm.a \
				-lXpm -lXt -lX11 -lXext \
				$(XMULIB) $(XTRALD)

VERSION		=	0.0.5

PROGRAMS	=	maximaX

OBJECTS		=	checksum.o commutils.o utils.o maxconf.o kwzconf.o \
				nahber.o sperrber.o rufzuord.o globdat.o tlnperm.o \
				teltyp.o amtperm.o calldist.o gebdat.o loccharge.o\
				steuer.o gebuehr.o teilndat.o gebkit.o maximaX.o 

ADHFILES	=	maximaX.ad.h maxconf.ad.h kwzconf.ad.h nahber.ad.h \
				sperrber.ad.h rufzuord.ad.h globdat.ad.h tlnperm.ad.h \
				teltyp.ad.h amtperm.ad.h calldist.ad.h gebdat.ad.h \
				loccharge.ad.h max_help.ad.h steuer.ad.h gebuehr.ad.h \
				teilndat.ad.h gebkit.ad.h

ADFILES		=	maximaX.ad maxconf.ad kwzconf.ad nahber.ad \
				sperrber.ad rufzuord.ad globdat.ad tlnperm.ad \
				teltyp.ad amtperm.ad calldist.ad gebdat.ad \
				loccharge.ad max_help.ad steuer.ad gebuehr.ad \
				teilndat.ad gebkit.ad 
.IGNORE:

all:	$(PROGRAMS)

static: maximaX.static

%.ad.h: %.ad
	ad2c $< > $@

%.o: %.c is1.h
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(LDEFINES) -DCFGPATH=\"$(CONFIGPATH)\" -o $@ -c $<

commutils.o: commutils.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(LDEFINES) -DLOCKDIR=\"$(LOCKDIR)\" -o $@ -c $<

maximaX.o: maximaX.c is1.h maximaX.h $(ADHFILES)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(LDEFINES) -DVERSION=\"$(VERSION)\" \
	-DCFGPATH=\"$(CONFIGPATH)\" -o $@ -c $<

maximaX: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(XLIBEXT)

maximaX.static: maximaX
	$(CC) -o $@ $(OBJECTS) $(SXLIBEXT)
	strip $@

tools: is1.h gebkit.h
	(cd tools; make)

tools.clean:
	(cd tools; make clean)

adfile:
	cat $(ADFILES) > ./MaximaX

install: $(PROGRAMS) adfile
	install -g $(EXECGROUP) -o $(EXECUSER) -m 550 $(PROGRAMS) $(INSTALL_BIN)
	install -g $(EXECGROUP) -o $(EXECUSER) -m 440 ./MaximaX $(INSTALL_AD)

clean:
	@rm core *.o *.ad.h *.log* $(PROGRAMS) *.static

dist_src: clean tools.clean
	(cd .. ; \
	tar zcf /home/chrisb/maximaX-$(VERSION).src.tgz \
		maximaX/WARNUNG maximaX/README.maximaX maximaX/CHANGES\
		maximaX/Makefile maximaX/*.h maximaX/*.c maximaX/*.ad \
		maximaX/tools/*.c maximaX/tools/Makefile )
	
dist_bin: $(PROGRAMS) static adfile
	(cd tools; make)
	(cd .. ; tar zcf /home/chrisb/maximaX-$(VERSION).bin.tgz \
		maximaX/WARNUNG maximaX/maximaX maximaX/maximaX.static \
		maximaX/README.maximaX maximaX/CHANGES maximaX/MaximaX \
		maximaX/tools/incalls maximaX/tools/outcalls \
		maximaX/install*.sh )
	
dist: dist_src dist_bin
