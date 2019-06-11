TARGET = hello
OBJS = main.o lib_multimedia/mp3player.o
 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS = -lpspgu -lpng -lz -lm -lmad -lpspaudiolib -lpspaudio

PSP_FW_VERSION = 371

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Snake
PSP_EBOOT_ICON = portada.png
PSP_EBOOT_PIC1 = fondo.png
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
