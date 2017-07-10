VERSION_SHORT = 0.1
DEBUG = FALSE

GCC = nspire-gcc
GENZEHN = genzehn

BUILD_DATE = $(shell date '+%Y-%m-%d')
VERSION = $(shell git describe --always --dirty 2> /dev/null || echo $(VERSION_SHORT))
GCCFLAGS = -std=gnu11 -Wall -Wextra -marm -fstrict-aliasing -fomit-frame-pointer -ffunction-sections -fdata-sections \
	   -DBUILD_DATE=\"$(BUILD_DATE)\" -DVERSION=\"$(VERSION)\" -DOLD_SCREEN_API -Wno-implicit-fallthrough \
	   -Wno-format-truncation
LDFLAGS = -Wl,--gc-sections,--nspireio
ZEHNFLAGS = --name "Duktape-nspire" --author "Legimet" --notice "JavaScript (ECMAScript) interpreter using Duktape"

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

module.o: module.c module.h
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
