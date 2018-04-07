#
# Makefile for GEMAssist (gemasist)
#
TARGET = gemasist.GTP

# compiler settings
CC = gcc -DDEBUG
AS = $(CC) -c
LD = $(CC) 
CP = cp
RM = rm -f

#CPU = 68000
#CPU = 68030
#CPU = 68040
CPU = 68020-60
#CPU = 5475

DEFS = -DLIBDFRM

OPTS = $(CPU:%=-m%) -funsigned-char \
       -fomit-frame-pointer -O2 -fstrength-reduce \
       -fno-strict-aliasing

ifeq ($(CPU),5475)
    OPTS = $(CPU:%=-mcpu=%) $(OPTFLAGS)
else
    OPTS = $(CPU:%=-m%) $(OPTFLAGS)
endif

WARN = \
	###-Wall \
	-Wmissing-prototypes \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wimplicit-function-declaration  ###-Werror

INCLUDE = -I/usr/GEM/include -I/usr/include

CFLAGS = $(INCLUDE) $(WARN) $(OPTS) $(DEFS)
ASFLAGS = $(OPTS)
LDFLAGS = -Wl -t
LIBS = -L/usr/GEM/lib -ldfrm -lwindom -lldg -lgem -lezxml -L/usr/lib/m68020-60 -lezxml

ifeq ($(CPU),5475)
    OBJDIR = obj.$(CPU)
else
    OBJDIR = obj$(CPU:68%=.%)
endif

#
# C source files
#
$(OBJDIR)/%.o: %.c
	@echo "$(CC) $(CFLAGS) -c $< -o $@"; \
	$(CC) -Wp,-MMD,.deps/$(<:.c=.P_) $(CFLAGS) -c $< -o $@
	@cat .deps/$(<:.c=.P_) \
	    | sed "s,^\(.*\)\.o:,$(OBJDIR)/\1.o:," > .deps/$(<:.c=.P)
	@rm -f .deps/$(<:.c=.P_)

#
# files
#
CFILES = \
	 gemasist.c \
	 DateUtils.c

HDR = \
	gemasist.h

SFILES = 

OBJS = $(SFILES:%.s=$(OBJDIR)/%.o) $(CFILES:%.c=$(OBJDIR)/%.o)
OBJS_MAGIC := $(shell mkdir ./$(OBJDIR) > /dev/null 2>&1 || :)

DEPENDENCIES = $(addprefix ./.deps/, $(patsubst %.c,%.P,$(CFILES)))


$(TARGET): $(OBJS)
	$(LD) -o $@ $(CFLAGS) $(OBJS) $(LIBS)
	stack --fix=128k $@
	flags -g -v $@

000: ; $(MAKE) CPU=68000
030: ; $(MAKE) CPU=68030
040: ; $(MAKE) CPU=68040
060: ; $(MAKE) CPU=68020-60
v4e: ; $(MAKE) CPU=5475

clean:
	rm -Rf *.bak */*.bak */*/*.bak *[%~] */*[%~] */*/*[%~]
	rm -Rf obj.* */obj.* */*/obj.* .deps */.deps */*/.deps *.o */*/*.o
	rm -Rf *.app *.[gt]tp *.prg *.ovl */*.ovl

distclean: clean
	rm -Rf .cvsignore */.cvsignore */*/.cvsignore CVS */CVS */*/CVS


#
# adjust file names
#
$(CFILES) $(HDR) Makefile: ; mv `echo $@ | tr A-Z a-z` $@
case: Makefile $(HDR) $(CFILES)


#
# dependencies
#
DEPS_MAGIC := $(shell mkdir ./.deps > /dev/null 2>&1 || :)

-include $(DEPENDENCIES)

#gcc  -I/usr/GEM/include/windom -w -Wcomment -Wimplicit-int -m68040 -funsigned-char -fomit-frame-pointer -O2 -fstrength-reduce -fno-strict-aliasing -c gentool.c -o gentool.o
#
#gcc -o gentool.app gentool.o -m68040 -funsigned-char -fomit-frame-pointer -O2 -fstrength-reduce -fno-strict-aliasing -L/usr/GEM/lib -ldfrm -lwindom -lldg -lgem
