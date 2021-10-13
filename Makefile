LINK    = @echo linking $@ && arm-linux-gnueabihf-g++
GCC     = @echo compiling $@ && arm-linux-gnueabihf-g++ 
GC      = @echo compiling $@ && arm-linux-gnueabihf-gcc
AR      = @echo generating static library $@ && ar crv 
FLAGS   = -g -DDEBUG -W -Wall -fPIC -std=c++11
GCCFLAGS = 
DEFINES = 
HEADER  = -I./ -I/usr/arm32/include
LIBS    = -L./ -L/usr/arm32/lib
LINKFLAGS =

LIBS    += -pthread -lcurl -lcrypto -lssl -lm -lqiniu

SOURCE_FILES :=\
		main.cpp\
		app/function/functions.cpp\
		app/log/logger.cpp\
		app/httpclient/httpclient.cpp\
		app/qiniu/qiniu.cpp\

PROJECTNAME = QiniuDemo.out

TARGET = main

all: $(SOURCE_FILES)
	$(LINK) $(FLAGS) $(LINKFLAGS) -o ${PROJECTNAME} $^ $(LIBS) ${HEADER}
clean:
	rm -rf *.o ${PROJECTNAME}