#include <stdio.h>
#include <stdlib.h>
#include "gdal.h"
#include "cpl_conv.h"
#include "GeotiffUtil.h"

Geotiff ReadGeotiff( const char* GeotiffFileName ) {
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

  // register GDAL drivers, create GDALDatasetH, GDALDriverH objects.
  // ****************************************************************
  GDALAllRegister();
  GDALDatasetH GeotiffReader;
  GDALDriverH GeotiffDriver;

  // create structure object (see GeotiffUtil.h header file
  // for its definition): note this is a C structure.
  // ******************************************************
  Geotiff GTiff;

  // set the filename
  // **************** 
  GTiff.filename = GeotiffFileName;

  // open the geotiff file as a GDAL dataset, open GDAL driver
  // *********************************************************
  GeotiffReader = GDALOpen( GeotiffFileName , GA_ReadOnly );
  GeotiffDriver = GDALGetDatasetDriver( GeotiffReader ) ;

  // make sure Geotiff file opened successfully. Check for this.
  // *********************************************************** 
  if( GeotiffReader == NULL ) {
    printf("ERROR (fatal): Geotiff not opened successfully:\n" );
    printf("%s\n" , GeotiffFileName );
    exit(1);
  }

  // read the Geotiff X/Y dimensions into C structure. Also,
  // read its projection as well
  // ******************************************************* 
  ReadDimensionsGeotiff( GeotiffFileName,GeotiffReader,&GTiff );
  ReadProjectionGeotiff( GeotiffReader,&GTiff );
  ReadNoDataValue( GeotiffReader,&GTiff );

  // return the C structure.
  // ***********************
  return GTiff;
}

void ReadNoDataValue( GDALDatasetH reader,Geotiff *tiff ) {
  /* ********************************************************************************
   * void ReadNoDataValue( GDALDatasetH, Geotiff * ):
   * This function reads and sets the NoData value from the
   * Geotiff.
   *
   * Args:
   *  GDALDatasetH : Geotiff dataset object returned by GDALOpen() method.
   *  struct Geotiff* : Pointer to output Geotiff structure returned by readGeotff().
   * Returns:
   *  None. But does assign NoData value to Geotiff
   *  structure object returned by ReadGeotiff() above.
   *
   */
  double NoDataValue;
  GDALRasterBandH Band = GDALGetRasterBand( reader,1 );
  NoDataValue = GDALGetRasterNoDataValue(Band,NULL);
  tiff->NoDataValue = NoDataValue;
}

void ReadProjectionGeotiff( GDALDatasetH reader,Geotiff *tiff ){

 /* ********************************************************************************
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
  *  returned by ReadGeotiff() above.
  *
  */

  // initialize C array to hold Geotiff geotransform.
  //   note:
  //     DIMGT is 6, and defined in header file. 
  // ************************************************
  double gt[DIMGT];

  // define pointers to Geotiff projection (character array)
  // and to the geotransform array
  // *******************************************************
  double *pGT;
 
  // take pointers just defined and point them to appropriate objects
  // **************************************************************** 
  GDALGetGeoTransform(reader,gt);
  pGT = &gt[0];
  tiff->projection = (char*)GDALGetProjectionRef(reader);

  // fill in the geotransform double[] array
  // ***************************************
  int counter;
  counter=0;
  while( counter<DIMGT ) 
  {
    tiff->geotransform[counter] = *(pGT+counter);
    counter++;
  }
}

void ReadDimensionsGeotiff( const char* filename,GDALDatasetH reader,Geotiff *tiff ) {
 /* **********************************************************************************
  * void readDimensionsGeotiff:
  *   This function takes in pointers to the geotiff
  *   filename and pointer to a GDALDataset reader object,
  *   as well as a Geotiff structure. The number of rows,
  *   columns, and bands are read-in from the dataset,
  *   and assigned into the structure.
  *
  * Args:
  *  const char*     : Geotiff filename string.
  *  GDALDatasetH    : Geotiff dataset object returned by GDALOpen() method.
  *  struct Geotiff* : Pointer to Geotiff data structure returned by readGeotiff().
  * Returns:
  *  None. But does write dimensions into structure returned by readGeotiff() via
  *  the xsize, ysize, and nbands Geotiff structure attributes
  *  (that is, the number of columns, rows, and bands in Geotiff)
  */

  // initialize dimensions for geotiff file
  // **************************************
  int xdim, ydim,nbands;

  // using the geotiff object, get dimensions
  // ****************************************
  xdim = GDALGetRasterXSize( reader ) ;
  ydim = GDALGetRasterYSize( reader ) ;
  nbands = GDALGetRasterCount( reader) ;

  // set the dimensions onto the structure object
  // ********************************************
  tiff->xsize = xdim;
  tiff->ysize = ydim;
  tiff->nbands = nbands;
}
