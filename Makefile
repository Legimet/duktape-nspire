DEBUG = FALSE

GCC = nspire-gcc
GENZEHN = genzehn

GCCFLAGS = -std=gnu11 -Wall -W -marm -fstrict-aliasing -fomit-frame-pointer
LDFLAGS = -Wl,--nspireio
ZEHNFLAGS = --name "Duktape"

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -O3
else
	GCCFLAGS += -O0 -g
endif

OBJS = $(patsubst %.c, %.o, $(shell find . -name \*.c))
EXE = duktape
DISTDIR = .
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: $(EXE).tns

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

$(EXE).elf: $(OBJS)
	mkdir -p $(DISTDIR)
	$(GCC) $^ -o $@ $(LDFLAGS)

$(EXE).zehn.tns: $(EXE).elf
	$(GENZEHN) --input $^ --output $@ $(ZEHNFLAGS)

$(EXE).tns: $(EXE).zehn.tns
	make-prg $^ $@

clean:
	rm -f $(OBJS) $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).zehn.tns
