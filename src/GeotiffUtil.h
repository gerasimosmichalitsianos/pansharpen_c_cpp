#ifndef GEOTIFFUTIL_H_
#define GEOTIFFUTIL_H_

// define number of elements in geotransform
// *****************************************
#define DIMGT 6

// define C structure to hold properties of the
// geotiff, including its projection, dimensions,
// and geotransform
// **********************************************
typedef struct {
  const char* filename;
  int xsize;
  int ysize;
  int nbands;
  double geotransform[6];
  double NoDataValue;
  char* projection;
} Geotiff;

// define function prototypes
// **************************
Geotiff ReadGeotiff( const char* );
void ReadProjectionGeotiff( GDALDatasetH,Geotiff* );
void ReadNoDataValue( GDALDatasetH,Geotiff* );
void ReadDimensionsGeotiff( const char*,GDALDatasetH,Geotiff* );
#endif
