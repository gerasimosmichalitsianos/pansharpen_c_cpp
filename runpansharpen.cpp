#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "gdal.h"
#include "cpl_conv.h"
#include "geotiffutil.h"
#include "resample.h"
#include "pansharpen.h"
extern "C" {
  #include "geotiffutil.h"
};

void usage() {

  printf("\n"
   " NAME:\n "
   "  pansharpen\n"
   " DESCRIPTION:\n "
   "  This C/C++ program is meant to pansharpen a set of 4 \n "
   "  one-band Geotiff files holding low-resolution imagery for \n"
   "   a set of NIR, red, green, and blue bands. This is done by \n "
   "  resampling this imagery using bicubic resampling and applying \n"
   "   the Brovey and FIHS image pan-sharpening algorithms and \n "
   "  transformations. Imagery provided by a 1band panchromatic Geotiff \n"
   "   image file are also necessary. \n"
   " INPUTS:\n"
   "   Data from five 1-band Geotiffs are necessary to run this program.\n"
   "   These Geotiffs are a panchromatic image file (high-res.) and the \n"
   "   Geotiffs holding pixel data for the red, green, blue, and NIR bands.\n"
   " OUTPUTS:\n"
   "   Two 4-band Geotiff image files holding the pan-sharpened image bands \n"
   "   for the pan-sharpened red, green, blue, and NIR bands. Their geotransform \n"
   "   and projection information reflects the input panchromatic Geotiff image\n"
   "   file passed-in. One 4-band output file is a result of using the Brovey \n "
   "  transform pan-sharpening, and the other using the Fast Intensity Hue \n"
   "   Saturation pan-sharpening technique (FIHS).\n"
   " USAGE: \n"
   " \n "
   "   $ make \n"
   "    $ ./pansharpen  \n"
   "     (1) <PANCHROMATIC_GEOTIFF>{.tif} \n"
   "     (2) <NIR_GEOTIFF>{.tif} \n"
   "     (3) <RED_GEOTIFF>{.tif} \n"
   "     (4) <GREEN_GEOTIFF>{.tif} \n"
   "     (5) <BLUE_GEOTIFF>{.tif}\n\n"
   "   Example: \n"
   "     $ ./pansharpen pan.tif NIR.tif red.tif green.tif blue.tif \n"
   " \n "
   " AUTHOR: \n"
  "   Gerasimos \"Geri\" Michalitsianos\n"
  "   gerasimosmichalitsianos@gmail.com\n"
  "   10 February 2018\n\n"
  );
  exit(1);
}

/* ***************************************************************************
 * int checkarg(char*):
 * Function to make sure command-line argument is at least 5
 * characters in length and has a .tif extension.
 *
 * Args:
 *   char* : input command-line string passed from main().
 * Returns:
 *   int: 1 or 0 (True or False) if command-line arg. ends with .TIF or .tif.
 */

int checkarg(char* input) {

  /* check to make sure command-line arg. has at least 5 characters  */
  size_t arglen = strlen(input);
  if(arglen<5) return 0;

  /* get extension of input (last 4 characters) */
  char extension[4];
  memcpy(extension,&input[arglen-4],4);
  extension[4] = '\0';

  /* check extension against .tif or .TIF  */
  if(!((strcmp(extension,".tif") == 0) || (strcmp(extension,".TIF") == 0))) {
    return 0;
  }
  return 1;
}

int main(int argc, char* argv[]) {

  /* Check number of command-line arguments. The user should pass-in 6
   * command-line arguments, which are the Geotiff filename strings for
   * the panchromatic (high-res.), and red, green, blue, and NIR Geotiff
   * image files.
   */

  if(argc != 6) {
    usage();
    exit(1);
  }

  /* iterate through command-line arguments, make sure all of them
   * have a .tif extension.
   */

  int argumentCounter;
  int rv;
  argumentCounter=1;
  while(argumentCounter<argc) {
    rv = checkarg(argv[argumentCounter]);
    if(rv == 0) {
      printf("\n All command-line args. should have .tif or .TIF extension \n");
      usage();
    }
    argumentCounter++;
  }

  /* Get filenames as command-line arguments. The user should pass in
   * filenames for 1-band Geotiffs. Each Geotiff should hold 1 single
   * band (raster). They should be passed in the following order:
   *
   *  (1) panchromatic geotiff image file (high resolution)
   *  (2) NIR (near-infrared) geotiff image file (low resolution)
   *  (3) red (near-infrared) geotiff image file (low resolution)
   *  (4) green (near-infrared) geotiff image file (low resolution)
   *  (5) blue (near-infrared) geotiff image file (low resolution)
   */

  char* filenameBlue = argv[5];
  char* filenameGreen = argv[4];
  char* filenameRed = argv[3];
  char* filenameNIR = argv[2];
  char* filenamePan = argv[1];

  /* resample imagery data in 1-band Geotiff image files for
   * the Red, Green, Blue, and NIR
   */

  std::string filenameResampledNIR = resample(filenameNIR,filenamePan);
  std::string filenameResampledRed = resample(filenameRed,filenamePan);
  std::string filenameResampledGreen = resample(filenameGreen,filenamePan);
  std::string filenameResampledBlue = resample(filenameBlue,filenamePan);

  /* read Geotiffs for the resampled NIR, Green, Blue, and NIR bands.
   * Also read the panchromatic Geotiff image file.
   */

  Geotiff geotiffNIR, geotiffRed, geotiffGreen, geotiffBlue,geotiffPan;
  geotiffNIR = readGeotiff(filenameResampledNIR.c_str());
  geotiffRed = readGeotiff(filenameResampledRed.c_str());
  geotiffGreen = readGeotiff(filenameResampledGreen.c_str());
  geotiffBlue = readGeotiff(filenameResampledBlue.c_str());
  geotiffPan = readGeotiff(filenamePan);

  pansharpen(geotiffNIR,geotiffRed,geotiffGreen,geotiffBlue,geotiffPan);
  return 0 ;
}
