#
# name of output executable program 
#
PROG = bin/pansharpen  

#
# name of C compiler 
#
CC = gcc  

#
# name of C++ compiler 
#
CPP = g++ 

#
# C++ compilation flags 
#
CPPFLAGS = -g -Wall -std=c++17 -I/usr/include/gdal

# 
# flags for compilation  
#
LDFLAGS = -L/usr/lib -lgdal -lm 

all:
	@$(CC) src/GeotiffUtil.c -c $(CPPFLAGS) $(LDFLAGS) -o bin/GeotiffUtil.o
	@$(CPP) src/Main.cpp src/Resample.cpp src/Pansharpen.cpp $(CPPFLAGS) $(LDFLAGS) -o $(PROG)
clean: 
	@rm $(PROG)
	@rm bin/*.o
