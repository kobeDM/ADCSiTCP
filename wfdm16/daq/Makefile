PROG = daq
ROOTFLAGS=-pthread -Wno-deprecated-declarations -I${ROOTSYS}/include -I./ShUtil -I./
CXXFLAGS=${ROOTFLAGS} -std=c++14
ROOTLIBS=$(shell root-config --libs)
# LDLIBS += -L/usr/local/lib/paprica -lpaprica
OBJS=${PROG}.o ShUtil/src/util.o ShUtil/src/rootutil.o ShUtil/src/physutil.o ShUtil/src/shStyle.o SiTCP.o

all: $(PROG)

$(PROG): $(OBJS)
	g++ $(CXXFLAGS) -o $(PROG) $(OBJS) $(ROOTLIBS) -lgcc
clean:
	rm -f  $(PROG) $(OBJS)
