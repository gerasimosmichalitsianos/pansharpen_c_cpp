/* geotiffutil.h Header file */

#ifndef GEOTIFFUTIL_H_
#define GEOTIFFUTIL_H_
#define DIMGT 6

/* define the structure in this header file, so main()
   in runpansharpen.cpp can see it, as well as function(s) in
   geotiffutil.c and others ... */

/* define structure to hold Geotiff attributes. This
 * includes its filename, its dimensions (number of
 * rows, columns, bands), its geotransform, projection,
 * as well as a band of pixel data.
 */

typedef struct {
  const char* filename;
  int xsize;
  int ysize;
  int nbands;
  double geotransform[6];
  char* projection;
  float** band;
} Geotiff;

 /* define function prototypes.
  */

Geotiff readGeotiff(const char*);
void readProjectionGeotiff(GDALDatasetH,Geotiff*);
void readDimensionsGeotiff(const char*,GDALDatasetH,Geotiff*);
void readBandGeotiff(const char*,GDALDatasetH,Geotiff*,int);
int writeGeotiff(Geotiff,float**,float**,float**,float**,char*);
#endif
