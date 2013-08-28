

GPPFLAGS=-std=gnu++11 
SRCDIR = ../ 
#INCDIR = -I/usr/locainclude -I../src -I../test
CPPFLAGS = -std=c++11 -std=gnu++11 -g -D__STRICT_ANSI__ #-Wunused-parameter #-Wall
LFLAGS = -lboost_system -lpthread 
CC_CLANG = clang++
CC_GCC = g++ -pipe

BOOSTFLAGS=-I /home/apandey/boost_1_54_0 -L /home/apandey/Downloads/boost_1_54_0/stage/lib
BOOSTFLAGS+= -I/usr/include/python3.2 -I/usr/local/include/boost/python 
BOOSTFLAGS+= -I/usr/include/python2.7 -L/usr/lib/python2.7 -L/usr/lib/python3.2
CPPFLAGS+= -I/usr/include/x86_64-linux-gnu/c++/4.8
#CPPFLAGS+= -I/usr/include/x86_64-linux-gnu/c++/4.8 -I /usr/include -I /usr/lib/gcc/x86_64-linux-gnu/4.8/include/
BOOSTFLAGS+= -L/usr/lib 

BOOSTLIBS=-lboost_chrono -lboost_date_time -lboost_filesystem -lboost_graph -lboost_iostreams -lboost_locale -lboost_log_setup -lboost_log -lboost_math_c99f -lboost_math_c99l -lboost_math_c99 -lboost_math_tr1f -lboost_math_tr1l -lboost_math_tr1 -lboost_prg_exec_monitor -lboost_program_options -lboost_python -lboost_random -lboost_regex -lboost_serialization -lboost_signals -lboost_system -lboost_thread -lboost_timer -lboost_unit_test_framework -lboost_wave -lboost_wserialization -lpthread -lpython2.7 -lpython3.2mu

### FILES
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
TARGET = mods.out

g: CXX='/usr/bin/g++' -pipe
c: CXX='/home/dev/clang_3.2/bin/clang++'

all: g c

c: $(OBJECTS)
	$(CC_CLANG) $(GPPFLAGS) $(CPPFLAGS) $(OBJECTS) $(LFLAGS) $(BOOSTLIBS) $(BOOSTFLAGS) -o $(TARGET)


g: $(OBJECTS)
	$(CC_GCC) $(GPPFLAGS) $(CPPFLAGS) $(OBJECTS) $(LFLAGS) $(BOOSTLIBS) $(BOOSTFLAGS) -o $(TARGET)

depend: 
	@makedepend -Y -m $(SOURCES) 2> /dev/null

clean:
	@rm -f *.o *.bak mods.out *~ *%

##### End of Makefile
# DO NOT DELETE
