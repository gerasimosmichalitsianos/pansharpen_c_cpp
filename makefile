#
# name of output executable program 
#

PROG = pansharpen  

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

CPPFLAGS = -g -Wall -I/usr/include/gdal \
	-I/root/.local/share/.cpan/build/Geo-GDAL-2.010301-MVePvE/gdal/ogr \
	-I/root/.local/share/.cpan/build/Geo-GDAL-2.010301-MVePvE/gdal/port \
	-I/root/.local/share/.cpan/build/Geo-GDAL-2.010301-MVePvE/gdal/gcore \
	-I/root/.local/share/.cpan/build/Geo-GDAL-2.010301-MVePvE/gdal/alg

# 
# flags for compilation  
#

LDFLAGS = -L/usr/lib -lgdal -lm 

 
all:
	@$(CC) geotiffutil.c -c $(CPPFLAGS) $(LDFLAGS)  
	@$(CPP) runpansharpen.cpp resample.cpp geotiffutil.c pansharpen.cpp $(CPPFLAGS) $(LDFLAGS) -o $(PROG)
clean: 
	@rm $(PROG)
	@rm *.o  
