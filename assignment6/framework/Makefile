PROGNAME = main
SRCFILES = main.cpp levels.cpp
TARNAME = box2d.tgz

BOX_H = include/
# BOX_H = Box2D/
BOX_LIB = Box2D/Box2D/
# BOX_LIB = /usr/local/lib

CC = g++

WARNFLAGS = -Wall -Wshadow -pedantic-errors
CFLAGS = -g -O2 $(WARNFLAGS) -I$(BOX_H)
LFLAGS = -L$(BOX_LIB) -lBox2D -lGL -lglut

# Do some substitution to get a list of .o files from the given .cpp files.
OBJFILES = $(patsubst %.cpp,%.o,$(SRCFILES))

.PHONY: all run clean dist buildbox

all: $(PROGNAME)

$(PROGNAME): $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

run: $(PROGNAME)
	LD_LIBRARY_PATH=$(BOX_LIB) ./$(PROGNAME)

dist:
	tar cvzf $(TARNAME) Makefile *.cpp *.h

clean:
	rm -fv $(PROGNAME) $(OBJFILES) $(TARNAME)
	
buildbox:
	sudo apt-get install unzip cmake
	unzip Box2D_v2.2.1.zip
	mv Box2D_v2.2.1 Box2D
	cd Box2D && cmake -DBOX2D_INSTALL=ON -DBOX2D_BUILD_SHARED=ON && make
