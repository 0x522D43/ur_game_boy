LCC = $(GBDKDIR)bin/lcc -Wa-l -Wl-m -Wl-j

BINS	= main.gb
ASSET_SOURCES := $(wildcard assets/*.c)
MAP_SOURCES := $(wildcard maps/*.c)
STRUCT_SOURCES := $(wildcard structs/*.c)
SOURCES := main.c $(ASSET_SOURCES) $(MAP_SOURCES) $(STRUCT_SOURCES)

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

all:	$(BINS)

# Compile and link the game and asset sources in one pass
main.gb: $(SOURCES)
	$(LCC) $(LCCFLAGS) -o $@ $^

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi *.rst assets/*.o assets/*~