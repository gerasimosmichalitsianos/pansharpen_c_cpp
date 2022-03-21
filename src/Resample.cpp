#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include "unistd.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdalwarper.h"
#include "ogr_spatialref.h"
#include "Resample.h"
typedef std::string String;

std::map<String,String> ResampleImageGeotiffs( std::map<String,String>& image_filenames ) { // reference parameter

  /* ************************************************************************************
   * std::map<std::string,std::string> ResampleImageGeotiffs( std::map<String,String>& ):
   * 
   * This function takes in a reference parameter to a std::map object, which uses
   * the std::string class for both keys and values. These keys and values refer
   * to the imagery passsed in from command-line (see src/Main.cpp) for the panchromatic,
   * blue,green,red, and NIR image files. For each of the NIR, RGB image files, this
   * function calls the ResampleImageFile() function below to resample that Geotiff
   * image file to the same dimensions as the panchromatic image file. A map is
   * returned containing the new image filenames for the resampled imagery.
   *
   * Args:
   *   std::map<std::string,std::string>& : reference to map for filenames.
   * Returns:
   *   std::map<std:string,strd::string>  : map holding resampled imagery.
   *
   */

  // create map to hold output resampled imagery
  std::map<String,String> ResampledImagery;
  
  for( auto const& [filekey,filename] : image_filenames ) {

    // there is no need to resample the panchromatic image, so we
    // continue, so we only resample the multispectral (RGB,NIR) images
    // ****************************************************************
    if( filekey == "pan" ) continue;

    // resample the Geotiff so that it maches the same dimensions as
    // the panchroamtic Geotiff. we write to file, not in memory here.
    // ***************************************************************
    String OutNameResampled = ResampleImageFile( 
      filename.c_str(), image_filenames["pan"].c_str() ); 
  
    // append the map<String,String> add key/filename for the
    // resampled image Geotiff file.
    // ******************************************************
    ResampledImagery[ filekey+"_resampled" ] = OutNameResampled;   
  }

  // make sure panchromatic image is inside the new
  // data structure
  // **********************************************
  ResampledImagery[ "pan" ] = image_filenames[ "pan" ];
  return ResampledImagery;
}

String ResampleImageFile( const char* srcfname, const char* dstfname ){

 /* *******************************************************************
  * String resample(char*,char*):
  * 
  * This function resamples an input low-resolution 1-band
  * Geotiff file to new dimensions as specified by an input
  * higher-resolution (panchromatic) Geotiff file. Bicubic
  * resampling is used here.
  *
  * Args:
  *  char* : low-resolution 1-band Geotiff filename string.
  *  char* : higher-resolution 1-band Geotiff filename string.
  * Returns:
  *  String (std::string): Out filename for resampled Geotiff file.
  *
  */

  GDALAllRegister();
  GDALDatasetH srcDataset;
  const char* srcProjection;

  /* open up low-resolution Geotiff file dataset */
  srcDataset = GDALOpen( srcfname , GA_ReadOnly );
  CPLAssert( srcDataset != NULL );

  /* read projection from low-res. Geotiff. Also read its
   * data-type. For LANDSAT8 its unsigned int16.
   */

  srcProjection = GDALGetProjectionRef( srcDataset ) ;
  CPLAssert( srcProjection != NULL && strlen( srcProjection ) > 0) ;
  GDALDataType sourceDatatype;
  sourceDatatype = GDALGetRasterDataType( GDALGetRasterBand(srcDataset,1) );

  /* create output filename for bicubic resampled 1-band Geotiff. Just
   * replace the .tif extension with '_resampled.tif'.
   */

  String outfname;
  int len = ((String)srcfname).length();
  String extension("_resampled.tif");
  outfname = ((String)srcfname).substr(0,len-4)+extension;

  /* remove output resampled Geotiff file if it already exists */
  int deleted;
  if( access( outfname.c_str() , F_OK ) != -1) {
    deleted = remove(outfname.c_str());
  }

  /* open the destination (high-res) file (i.e. panchromatic image file) ,
   * read following parameters:
   *  (1) ncols : output resampled number of columns (samples)
   *  (2) nrows : output resampled number of rows (lines)
   *  (3) output geotransform for resampled low-res geotiff : array of 6 DOUBLE elements
   *  (4) output projection string
   */

  GDALDatasetH dstDataset;
  int dstnrows, dstncols;
  double dstGeotransform[6];
  const char* dstProjection;

  dstDataset = GDALOpen( dstfname , GA_ReadOnly);
  dstncols = GDALGetRasterXSize( dstDataset );
  dstnrows = GDALGetRasterYSize( dstDataset );
  GDALGetGeoTransform(dstDataset, dstGeotransform);
  dstProjection = GDALGetProjectionRef( dstDataset );

  /* create geotransform object ... pass in following parameters:
   * (1) srcDataset : open GDAL dataset from low-res file
   * (2) srcProjection : projection from low-res file
   * (3) destination projection : projection of panchromatic (high-res.)
   *     among other arguments.
   */

  void *handleTransformArg;
  handleTransformArg = GDALCreateGenImgProjTransformer( srcDataset, srcProjection,
    NULL, dstProjection, FALSE, 0,  1);
  CPLAssert( handleTransformArg != NULL);

  /* define geotransform (from source low-res dataset)
   * as well as its dimensions
   */

  double srcGeoTransform[6];
  int outnrows = 0;
  int outncols = 0;
  CPLErr eErr;

  /* Gather some information about the output dimensions and map projection
   * information for the upcoming GDAL warp transformation.
   */

  eErr = GDALSuggestedWarpOutput( srcDataset,
    GDALGenImgProjTransform,
    handleTransformArg,
    srcGeoTransform,
    &outncols ,
    &outnrows );
  CPLAssert( eErr == CE_None);
  GDALDestroyGenImgProjTransformer( handleTransformArg );

  /* begin creating output dataset. first create objects
   * for the output driver handle and output GDAL dataset object.
   */

  GDALDriverH outHandleDriver;
  GDALDatasetH outDataset;

  /* create output dataset. This will have the same dimensions
   * as the input high-resolution Geotiff datasat (i.e. a 1-band Geotiff file
   * holding a panchromatic band). This output dataset will also inherit
   * its geotransform and projection string from this input high-res. dataset.
   */

  outHandleDriver = GDALGetDriverByName("GTiff");
  outDataset = GDALCreate( outHandleDriver ,
    outfname.c_str() ,
    dstncols, dstnrows , 1 ,
    sourceDatatype, NULL);
  GDALSetProjection( outDataset , dstProjection) ;
  GDALSetGeoTransform( outDataset, dstGeotransform) ;

  /* project input source (low-res.) Geotiff dataset to same higher
   * dimensions as input high-res. panchromatic image. To this end we
   * use the GDALReprojectImage() method with the following arguments:
   *
   *  (1) input low-resolution Geotiff GDAL dataset object from GDALOpen().
   *  (2) input source (low-res.) projection string from GetProjectionRef().
   *  (3) output high-resolution resampled Geotiff dataset object.
   *  (4) destination or output projection from high-res. Geotiff.
   *  (5) Resampling method.
   *  ...
   */

  eErr = GDALReprojectImage( srcDataset ,
    srcProjection,
    outDataset ,
    dstProjection ,
    GRA_Cubic, 0.0, 0.0, NULL,NULL,NULL);
    // alternatively: GRA_Cubic, 0.0, 0.0, GDALTermProgress, NULL, NULL);
  CPLAssert( eErr == CE_None) ;
  GDALClose(outDataset); /* this line is important. Close output dataset! */
  return outfname;
}
