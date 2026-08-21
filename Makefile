LCC = $(GBDKDIR)bin/lcc -Wa-l -Wl-m -Wl-j
PNG2ASSET = $(GBDKDIR)bin/png2asset

BINS	= ur-game-boy.gb
ASSET_IMAGES := $(wildcard assets/*.png)
ASSET_SOURCES := $(ASSET_IMAGES:.png=.c)
ASSET_HEADERS := $(ASSET_IMAGES:.png=.h)
MAP_SOURCES := $(wildcard maps/*.c)
STRUCT_SOURCES := $(wildcard structs/*.c)
SOURCES := main.c $(ASSET_SOURCES) $(MAP_SOURCES) $(STRUCT_SOURCES)

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

all:	$(BINS)

assets/%.c assets/%.h: assets/%.png
	$(PNG2ASSET) $< -noflip -no_palettes -tiles_only

# Compile and link the game and asset sources in one pass
ur-game-boy.gb: $(SOURCES) $(ASSET_HEADERS)
	$(LCC) $(LCCFLAGS) -o $@ $^

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi *.rst assets/*.o assets/*.c assets/*.h assets/*~