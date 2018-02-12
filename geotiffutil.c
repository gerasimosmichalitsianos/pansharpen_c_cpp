#include <stdio.h>
#include <stdlib.h>
#include "gdal.h"
#include "cpl_conv.h"
#include "geotiffutil.h"

/* ********************************************************************
 * struct Geotiff readGeotiff(const char*):
 * This function reads a 1-band Geotiff file. It
 * returns a structure that holds the x,y, and z
 * dimensions of the Geotiff (that is, the number
 * of columns, rows, and bands), Geotiff filename
 * string, geotransform, projection, as well as a
 * pointer to an array holding the data for the
 * first band in the Geotiff.
 *
 * Args:
 *  const char* : input filename string for Geotiff.
 * Returns:
 *  Struct Geotiff : Geotiff object. See header file for attributes.
 */

Geotiff readGeotiff(const char* fname) {

  GDALAllRegister();
  GDALDatasetH geotiffReader;
  GDALDriverH geotiffDriver;
  Geotiff GeotiffObj;

  GeotiffObj.filename = fname ;
  geotiffReader = GDALOpen( fname , GA_ReadOnly);
  geotiffDriver = GDALGetDatasetDriver( geotiffReader ) ;

  if(geotiffReader == NULL ) {
    printf("Geotiff not opened successfully:\n" );
    printf("%s\n" , fname);
    exit(1);
  }

  readDimensionsGeotiff(fname,geotiffReader,&GeotiffObj);
  readProjectionGeotiff(geotiffReader,&GeotiffObj);
  readBandGeotiff(fname,geotiffReader,&GeotiffObj,1);
  return GeotiffObj;

}

/* **************************************************************************
 * void readBandGeotiff(const char*,GDALDatasetH,struct Geotiff,int):
 * Function to read the first band of Geotiff file into
 * an array. Assigns the "band" attribute (unsigne int**) into the
 * Geotiff structure object returned by readGeotiff() method above.
 *
 * Args:
 *  const char* : filename string for Geotiff passed into readGeotff().
 *  GDALDatasetH : Geotiff GDAL dataset reader object returned by GDALOpen().
 *  struct Geotiff* : Pointer to Geotiff structure returned by readGeotff().
 *  int : Band number. 1 for now.
 * Returns:
 *  None. But this function does write the 2D array data as a pointer
 *  (unsigned int**) to output Geotiff structure returned by readGeotiff()
 *  above via its "band" attribute.
 */

void readBandGeotiff(
  const char* filename,
  GDALDatasetH reader,
  Geotiff *tiff,
  int bandNumber
) {

  int ncols, nrows; /* 2D dimensionsfor Geotiff. */
  int row;
  int col;

  GDALRasterBandH handleBand;
  float** outBand;
  handleBand = GDALGetRasterBand(reader,bandNumber);

  /* get 2D dimensions of Geotiff */
  ncols = tiff->xsize;
  nrows = tiff->ysize;

  outBand = (float**)malloc(nrows*sizeof(float*));
  GDALDataType datatype;
  datatype = GDALGetRasterDataType( GDALGetRasterBand(reader,bandNumber));
  unsigned short* scanLine = (unsigned short*) CPLMalloc(sizeof(unsigned short)*ncols);

  for(row=0; row<nrows; row++) {
    outBand[row] = (float*)malloc(ncols*sizeof(float));
    GDALRasterIO(
      handleBand, GF_Read, 0, row, ncols, 1,
      scanLine, ncols, 1,
      datatype, 0, 0
    );

    for(col=0; col<ncols; col++) {
      outBand[row][col] = (float)scanLine[col];
    }
  }

  CPLFree(scanLine);
  tiff->band = outBand;

}

/* ****************************************************************************
 * void readProjectionGeotiff(GDALDatasetH,struct Geotiff*):
 * This function reads in the geotransform and projection
 * information into the Geotiff structure. The geotransform
 * is a 6-element array and the projection is a string.
 *
 * Args:
 *  GDALDatasetH : Geotiff dataset object returned by GDALOpen() method.
 *  struct Geotiff* : Pointer to output Geotiff structure returned by readGeotff().
 * Returns:
 *  None. But does assign values to the geotransform[] and
 *  and projection string attributes to Geotiff structure object
 *  returned by readGeotiff() above.
 *
 */

void readProjectionGeotiff(
  GDALDatasetH reader,
  Geotiff *tiff
) {

  double gt[DIMGT];
  double *pGT; // iterator for geotransform[] array (6 elements)
  char* projection;
  GDALGetGeoTransform(reader,gt);
  pGT = &gt[0]; // alternatively just: geotransformValue = gt;
  tiff->projection = (char*)GDALGetProjectionRef(reader);

  int counter; /* assign values into geotransformi inside Geotiff struct  */
  counter=0;
  while(counter<DIMGT) {
    tiff->geotransform[counter] = *(pGT+counter)  ;
       // alternatively just: tiff->geotransform[counter] = gt[counter];
    counter++;
  }
}

/* *****************************************************************************
 * void readDimensionsGeotiff:
 * This function takes in pointers to the geotiff
 * filename and pointer to a GDALDataset reader object,
 * as well as a Geotiff structure. The number of rows,
 * columns, and bands are read-in from the dataset,
 * and assigned into the structure.
 *
 * Args:
 *  const char* : Geotiff filename string.
 *  GDALDatasetH : Geotiff dataset object returned by GDALOpen() method.
 *  struct Geotiff* : Pointer to Geotiff data structure returned by readGeotiff().
 * Returns:
 *  None. But does write dimensions into structure returned by readGeotiff() via
 *  the xsize, ysize, and nbands Geotiff structure attributes
 *  (that is, the number of columns, rows, and bands in Geotiff)
 */

void readDimensionsGeotiff(
  const char* filename,
  GDALDatasetH reader,
  Geotiff *tiff
) {

  int xdim, ydim;
  int nbands;
  xdim = GDALGetRasterXSize( reader ) ;
  ydim = GDALGetRasterYSize( reader ) ;
  nbands = GDALGetRasterCount( reader) ;

  tiff->xsize = xdim;
  tiff->ysize = ydim;
  tiff->nbands = nbands;

}

/* ***********************************************************************
 * int writeGeotiff(struct Geotiff,float**,float**,float**,float**):
 * This function writes out a 4-band Geotiff file. This Geotiff
 * file will contain the pan-sharpened bands for Red, Green, Blue,
 * and NIR data arrays. To this end, this function will use the dimensions,
 * map projection string, and geotransform from the input high-resolution
 * Geotiff file (i.e. 1-band panchromatic Geotiff image file). This
 * information is fed-into this function via a Geotiff structure object
 * return by readGeotiff() above with the high-resolution Geotiff file
 * passed-in.
 *
 * Args:
 *  struct Geotiff: Geotiff structure returned by readGeotiff() holding
 *    higher-resolution panchromatic image pixel data. Also holds the 
 *    geotransform and projection information from the high-resolution
 *    Geotiff, which is necessary to write-out the pan-sharpened 4-band 
 *    Goetiff holding pan-sharpened RGB,NIR bands. 
 *  float** : float** (2D) arrays for resampled RGB,NIR bands. 
 * Returns:
 *  int: a 0 for success.
 *
 */

int writeGeotiff(
  Geotiff pan,
  float** red,
  float** green,
  float** blue,
  float** NIR,
  char* outname
) {

 char* projection = pan.projection;
 double geotransform[DIMGT];
 GDALDriverH outHandleDriver;
 GDALDatasetH outDataset;
 int nrows, ncols;
 int row, col;
 ncols = pan.xsize;
 nrows = pan.ysize;

 /* iterate and fill in the 6-element
  * geotransform[] array for the output Geotiff file.
  */
 int count;
 count=0;
 while(count<DIMGT) {
   geotransform[count] = pan.geotransform[count];
   count++;
 }

 outHandleDriver = GDALGetDriverByName("GTiff");
 outDataset = GDALCreate( outHandleDriver ,
   outname  ,
   ncols, nrows , 3 ,
   GDT_Float32, NULL);
 GDALSetGeoTransform( outDataset, geotransform) ;
 GDALSetProjection( outDataset , projection) ;

 /* allocate appropriate memory for one scanline (row in 2D array/band)
  * for each of the output pan-sharpened Red, Green, Blue, and NIR bands.
  */

 float* scanLineRed = (float*) CPLMalloc(sizeof(float)*ncols);
 float* scanLineGreen = (float*) CPLMalloc(sizeof(float)*ncols);
 float* scanLineBlue = (float*) CPLMalloc(sizeof(float)*ncols);
 float* scanLineNIR = (float*) CPLMalloc(sizeof(float)*ncols);

 /* initialize objects for output GDALRasterBandH handle objects
  * for the output Red, Green, Blue, and NIR pan-sharpened
  * images (2D arrays).
  */

 GDALRasterBandH handleRedBand;
 GDALRasterBandH handleGreenBand;
 GDALRasterBandH handleBlueBand;
 GDALRasterBandH handleNIRBand;

 handleRedBand = GDALGetRasterBand(outDataset,1);
 handleGreenBand = GDALGetRasterBand(outDataset,2);
 handleBlueBand = GDALGetRasterBand(outDataset,3);
 handleNIRBand = GDALGetRasterBand(outDataset,4);

 for(row=0; row<nrows; row++) {

   for(col=0; col<ncols; col++) {
     scanLineRed[col] = red[row][col];
     scanLineGreen[col] = green[row][col];
     scanLineBlue[col] = blue[row][col];
     scanLineNIR[col] = NIR[row][col];
   }

   GDALRasterIO( /* write out scanline and/or row for pansharpened red band */
     handleRedBand, GF_Write, 0, row, ncols, 1,
     scanLineRed, ncols, 1,
     GDT_Float32, 0, 0
   );

   GDALRasterIO( /* write out scanline and/or row for pansharpened Green band */
     handleGreenBand, GF_Write, 0, row, ncols, 1,
     scanLineGreen, ncols, 1,
     GDT_Float32, 0, 0
   );

   GDALRasterIO( /* write out scanline and/or row for pansharpened Blue band */
     handleBlueBand, GF_Write, 0, row, ncols, 1,
     scanLineBlue, ncols, 1,
     GDT_Float32, 0, 0
   );

   GDALRasterIO( /* write out scanline and/or row for pansharpened NIR band */
     handleNIRBand, GF_Write, 0, row, ncols, 1,
     scanLineNIR, ncols, 1,
     GDT_Float32, 0, 0
   );


 }

 /* close out the 4-band output pan-sharpened Geotiff dataset */
 GDALClose(outDataset);
 return 0;
}
