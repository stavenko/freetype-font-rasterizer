BIN=free-type-font-creator
SOURCEDIR=./src
BUILDDIR=./build
SOURCES=${wildcard ${SOURCEDIR}/*.cpp}
OBJECTS=${patsubst ${SOURCEDIR}/%.cpp,${BUILDDIR}/%.o,${SOURCES}}

INCLUDE_DIR=${shell pkg-config --cflags freetype2}
LIBS=${shell pkg-config --libs freetype2} -lboost_program_options-mt
CC=clang++
CFLAGS=-std=c++11 -Os -g -Wall $(INCLUDE_DIR)
LFLAGS=-Os -g -Wall $(LIBS)

$(info $(pkg-config --cflags freetype2))

all: dir ${BUILDDIR}/${BIN}

install: ${BUILDDIR}/${BIN}
	cp $^ /usr/local/bin/

dir: 
	mkdir -p ${BUILDDIR}

${BUILDDIR}/${BIN}: ${OBJECTS}
	${CC} ${LFLAGS} -o $@ $^

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf ${BUILDDIR}

