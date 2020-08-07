CFLAGS += -w -Wno-unknown-pragmas -Wno-format -O3 

BINARY_DIR = linux
BINARY_NAME = sclparser

SOURCES += src/main.cxx
SOURCES += src/SCL.cxx
SOURCES += src/SCL_BaseSimpleTypes.cxx
SOURCES += src/SCL_BaseTypes.cxx
SOURCES += src/SCL_Communication.cxx
SOURCES += src/SCL_DataTypeTemplates.cxx
SOURCES += src/SCL_Enums.cxx
SOURCES += src/SCL_IED.cxx
SOURCES += src/SCL_Substation.cxx

INCLUDES += -Isrc/
INCLUDES += -I../xerces-c-3.2.3/src/
INCLUDES += -I../config/
INCLUDES += -I../libxsd/

CC = g++
STRIP = strip

ifeq ($(BUILD), STATIC)
LDFLAGS = --static
endif

LIBS = /usr/lib/libxerces-c-3.2.so

all: $(BINARY_NAME) strip

$(BINARY_NAME):	$(SOURCES)
	mkdir -p $(BINARY_DIR)
	$(CC) $(CFLAGS) -o $(BINARY_DIR)/$(BINARY_NAME) $(SOURCES) $(INCLUDES) $(LIBS) $(LDFLAGS) 

strip:
	$(STRIP) -s $(BINARY_DIR)/$(BINARY_NAME)	
	
clean : 
		rm -r $(BINARY_DIR)
