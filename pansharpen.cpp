#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdalwarper.h"
#include "ogr_spatialref.h"
#include "geotiffutil.h"
#include "pansharpen.h"

/* ********************************************************************
 * void pansharpenBrovey(Geotiff,Geotiff,Geotiff,Geotiff,Geotiff):
 * This function performs pan-sharpening via the Brovey or FIHS method.
 * It does the basic computations for either algorithm, creating
 * 4 pan-sharpened float** arrays (Red,Green,Blue,NIR). It then
 * calls writeGeotiff() in geotiffutil.c to write the 4
 * pan-sharpened arrays to a 4-band output Geotiff.
 *
 * The arguments are 5 Geotiff structure objects, which contain float**
 * 2D array data for panchromatic band array data as well as that
 * for the resampled red, NIR, green, and blue images (resampled to
 * same dimensions as panchromatic band via bicubic resampling.)
 *
 * Args:
 *  struct Geotiff:
 *   geotiffNIR
 *   geotiffRed
 *   geotiffGreen
 *   geotiffBlue
 *   geotiffPan
 * Returns:
 *   None. But calls either pansharpenFIHS or pansharpendBrovey, each of which
 *   subsequently call writeGeotiff() to write out new pan-sharpened
 *   4-band Geotiff image file.
 */

void pansharpen(
  Geotiff geotiffNIR,
  Geotiff geotiffRed,
  Geotiff geotiffGreen,
  Geotiff geotiffBlue,
  Geotiff geotiffPan
) {

  pansharpenFIHS(geotiffNIR, /* call FIHS pan-sharpening method.  */
    geotiffRed,
    geotiffGreen,
    geotiffBlue,
    geotiffPan
  );

  pansharpenBrovey(geotiffNIR, /* call the Brovey pan-sharpening method. */
    geotiffRed,
    geotiffGreen,
    geotiffBlue,
    geotiffPan
  );

}

/* ********************************************************************
 * void pansharpenBrovey(Geotiff,Geotiff,Geotiff,Geotiff,Geotiff):
 * This function performs pan-sharpening via the Brovey method.
 * It does the basic computations for this algorithm, creating
 * 4 pan-sharpened float** arrays (Red,Green,Blue,NIR). It then
 * calls writeGeotiff() in geotiffutil.c to write the 4
 * pan-sharpened arrays to a 4-band output Geotiff.
 *
 * The arguments are 5 Geotiff structure objects, which contain float**
 * 2D array data for panchromatic band array data as well as that
 * for the resampled red, NIR, green, and blue images (resampled to
 * same dimensions as panchromatic band via bicubic resampling.)
 *
 * Args:
 *  struct Geotiff:
 *   geotiffNIR
 *   geotiffRed
 *   geotiffGreen
 *   geotiffBlue
 *   geotiffPan
 * Returns:
 *   None. But calls writeGeotiff() to write out new pan-sharpened
 *   4-band Geotiff image file.
 */

void pansharpenBrovey(
  Geotiff geotiffNIR,
  Geotiff geotiffRed,
  Geotiff geotiffGreen,
  Geotiff geotiffBlue,
  Geotiff geotiffPan
) {

  float** NIR = geotiffNIR.band;
  float** red = geotiffRed.band;
  float** green = geotiffGreen.band;
  float** blue = geotiffBlue.band;
  float** pan = geotiffPan.band;

  int nrows, ncols;
  int row, col;
  nrows = geotiffPan.ysize;
  ncols = geotiffPan.xsize;

  float** L = (float**)malloc(nrows*sizeof(float*));
  float** NIRsharp = (float**)malloc(nrows*sizeof(float*));
  float** redsharp = (float**)malloc(nrows*sizeof(float*));
  float** greensharp = (float**)malloc(nrows*sizeof(float*));
  float** bluesharp = (float**)malloc(nrows*sizeof(float*));
  float sum;

  for(row=0; row<nrows; row++) {

    NIRsharp[row] = (float*)malloc(ncols*sizeof(float));
    redsharp[row] = (float*)malloc(ncols*sizeof(float));
    greensharp[row] = (float*)malloc(ncols*sizeof(float));
    bluesharp[row] = (float*)malloc(ncols*sizeof(float));

    for(col=0; col<ncols; col++) {
      sum = (float)(red[row][col] + green[row][col] + blue[row][col] + NIR[row][col]);
      redsharp[row][col] = ((float)red[row][col] / sum)*((float)pan[row][col]);
      greensharp[row][col] = ((float)green[row][col] / sum)*((float)pan[row][col]);
      bluesharp[row][col] = ((float)blue[row][col] / sum)*((float)pan[row][col]);
      NIRsharp[row][col] = ((float)NIR[row][col] / sum)*((float)pan[row][col]);
    }
  }
  writeGeotiff(geotiffPan,redsharp,greensharp,bluesharp,NIRsharp,"pansharpendBrovey.tif");
}

/* ********************************************************************
 * void pansharpenFIHS(Geotiff,Geotiff,Geotiff,Geotiff,Geotiff):
 * This function performs pan-sharpening via the FIHS method.
 * This is the "Fast Intensity Hue Saturation" method.
 *
 * It does the basic computations for this algorithm, creating
 * 4 pan-sharpened float** arrays (Red,Green,Blue,NIR). It then
 * calls writeGeotiff() in geotiffutil.c to write the 4
 * pan-sharpened arrays to a 4-band output Geotiff.
 *
 * The arguments are 5 Geotiff structure objects, which contain float**
 * 2D array data for panchromatic band array data as well as that
 * for the resampled red, NIR, green, and blue images (resampled to
 * same dimensions as panchromatic band via bicubic resampling.)
 *
 * Args:
 *  struct Geotiff:
 *   geotiffNIR
 *   geotiffRed
 *   geotiffGreen
 *   geotiffBlue
 *   geotiffPan
 * Returns:
 *   None. But calls writeGeotiff() to write out new pan-sharpened
 *   4-band Geotiff image file.
 */

void pansharpenFIHS(
  Geotiff geotiffNIR,
  Geotiff geotiffRed,
  Geotiff geotiffGreen,
  Geotiff geotiffBlue,
  Geotiff geotiffPan
) {

  float** NIR = geotiffNIR.band;
  float** red = geotiffRed.band;
  float** green = geotiffGreen.band;
  float** blue = geotiffBlue.band;
  float** pan = geotiffPan.band;

  int nrows, ncols;
  int row, col;
  nrows = geotiffPan.ysize;
  ncols = geotiffPan.xsize;

  float** L = (float**)malloc(nrows*sizeof(float*));
  float** NIRsharp = (float**)malloc(nrows*sizeof(float*));
  float** redsharp = (float**)malloc(nrows*sizeof(float*));
  float** greensharp = (float**)malloc(nrows*sizeof(float*));
  float** bluesharp = (float**)malloc(nrows*sizeof(float*));

  for(row=0; row<nrows; row++) {

    L[row] = (float*)malloc(ncols*sizeof(float));
    NIRsharp[row] = (float*)malloc(ncols*sizeof(float));
    redsharp[row] = (float*)malloc(ncols*sizeof(float));
    greensharp[row] = (float*)malloc(ncols*sizeof(float));
    bluesharp[row] = (float*)malloc(ncols*sizeof(float));

    for(col=0; col<ncols; col++) {

      L[row][col] = ((float)(red[row][col] + green[row][col] +
        blue[row][col] + NIR[row][col]))/4.0;
      redsharp[row][col] = red[row][col] + (pan[row][col] - L[row][col]);
      greensharp[row][col] = green[row][col] + (pan[row][col] - L[row][col]);
      bluesharp[row][col] = green[row][col] + (pan[row][col] - L[row][col]);
      NIRsharp[row][col] = NIR[row][col] + (pan[row][col] - L[row][col]);

    }
  }
  writeGeotiff(geotiffPan,redsharp,greensharp,bluesharp,NIRsharp,"pansharpendFIHS.tif");
}
