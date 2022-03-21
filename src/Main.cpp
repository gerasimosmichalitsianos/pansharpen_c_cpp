#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include <map>
#include <algorithm>
#include <string>
#include "gdal.h"
#include "cpl_conv.h"
#include "Resample.h"
#include "Pansharpen.h"

void Usage() {
  printf("                                                                         \n "
   " ***************************************************************************** \n "
   "                                                                               \n "
   " NAME:                                                                         \n "
   "  pansharpen                                                                   \n "
   " DESCRIPTION:                                                                  \n "
   "  This C/C++ program is meant to pansharpen a set of 4                         \n "
   "  one-band Geotiff files holding low-resolution imagery for                    \n "
   "   a set of NIR, red, green, and blue bands. This is done by                   \n "
   "  resampling this imagery using bicubic resampling and applying                \n "
   "   the Brovey and FIHS image pan-sharpening algorithms and                     \n "
   "  transformations. Imagery provided by a 1band panchromatic Geotiff            \n "
   "   image file are also necessary.                                              \n "
   " INPUTS:                                                                       \n "
   "   Data from five 1-band Geotiffs are necessary to run this program.           \n "
   "   These Geotiffs are a panchromatic image file (high-res.) and the            \n "
   "   Geotiffs holding pixel data for the red, green, blue, and NIR bands.        \n "
   "   All input images should have same data-type (e.g. short).                   \n "
   " OUTPUTS:                                                                      \n "
   "   Two 4-band Geotiff image files holding the pan-sharpened image bands        \n "
   "   for the pan-sharpened red, green, blue, and NIR bands. Their geotransform   \n "
   "   and projection information reflects the input panchromatic Geotiff image    \n "
   "   file passed-in. One 4-band output file is a result of using the Brovey      \n "
   "  transform pan-sharpening, and the other using the Fast Intensity Hue         \n "
   "   Saturation pan-sharpening technique (FIHS).                                 \n "
   " USAGE:                                                                        \n "
   "   $ bin/pansharpen                                                            \n "
   "     -p pan.tif                                                                \n "
   "     -n NIR.tif                                                                \n "
   "	 -r red.tif                                                                \n "
   "	 -g green.tif                                                              \n "
   "     -b blue.tif                                                               \n "
   "     -z 3                                                                      \n "
   "     -o $(pwd)                                                                 \n "
   "                                                                                \n"
   " AUTHOR:                                                                        \n"
  "   Gerasimos 'Geri'  Michalitsianos                                              \n"
  "   gerasimosmichalitsianos@gmail.com                                             \n"
  "   20 March 2022                                                               \n\n"
  "                                                                                 \n"
  " ******************************************************************************* \n"
  "                                                                                 \n"
  );
  exit(1);
}

/* ***************************************************************************
 * bool checkarg(char*):
 *   Function to make sure command-line argument is at least 5
 *   characters in length and has a .tif extension.
 *
 * Args:
 *   char* : input command-line string passed from main().
 * Returns:
 *   int: 1 or 0 (True or False) if command-line arg. ends with .TIF or .tif.
 */
bool CheckImageFileName( const char* input) {

  // any command line arg. should have at least 5 characters. 
  // ********************************************************
  if( strlen(input)<6 ) {
    return false;
  }

  // get extension of input (last 4 characters)
  // ******************************************
  size_t arglen = strlen(input);
  char file_extension[4];
  memcpy( file_extension,&input[arglen-4],4);
  file_extension[4] = '\0';

  // convert extension to std::string and check it against
  // any form of .TIF, .tif, .Tif, and so on and so forth.
  // *****************************************************
  std::string Extension(file_extension); 
  transform(Extension.begin(),
    Extension.end(),Extension.begin(),::tolower );
  if( !(Extension.compare(".tif") == 0 )) {
    return false;
  }
  return true;
}

int main( int argc, char* argv[] ) 
{
  /* **************************************************************
   * 
   * This is the main method of this C/C++ program. The following
   * arguments are necessary for this program to run:
   *   -p for geotiff containing panchromatic band.
   *   -n for geotiff containing NIR band.
   *   -r for geotiff for red band.
   *   -g for geotiff for green band.
   *   -b for geotiff with blue band.
   *
   * ************************************************************ */

  // use getopt to get command-line args. These can be applied
  // to either the docker container OR this C++ executable itself.
  // *************************************************************
  const char* pan_filename   = "";
  const char* nir_filename   = "";
  const char* red_filename   = "";
  const char* blue_filename  = "";
  const char* green_filename = "";  
  const char* N_out_bands    = ""; // default value: RGB
  const char* OutDir         = ""; // output directory

  // get option arguments
  // ********************
  int opt = 0;  
  while((opt=getopt(argc,argv,":h:p:n:r:g:b:z:o:"))!=-1) {
    switch(opt) {
      case 'h':
        Usage();
      case 'p':
	pan_filename   = optarg;
	break;
      case 'n':
	nir_filename   = optarg;
	break;
      case 'r':
	red_filename   = optarg;
	break;
      case 'g':
	green_filename = optarg;
	break;
      case 'b':
	blue_filename  = optarg;
	break;
      case 'z':
	N_out_bands    = optarg;
	break;
      case 'o':
	OutDir         = optarg;
	break;
      default:
	Usage();
    }    
  }

  // check to see if directory was passed-in. If it was,
  // then make sure directory exists. If not, then just set the
  // output directory to the current working directory (pwd)
  // **********************************************************
  if( strlen(OutDir)>0 ) {
    if(!std::filesystem::is_directory(OutDir)){
      printf("  \n WARNING: output directory (-o flag) %s does not exist. Using current directory.\n");
      OutDir = std::filesystem::current_path().c_str();
    }
  }

  // set number of output bands (3 will be default)
  // only 3 (RGB) or 4 bands (RGB,NIR) will be acceptable
  // ****************************************************
  int n_bands = 3;
  if(strlen(N_out_bands)) 
  { 
    try {
      n_bands = atoi(N_out_bands);
    } catch (...) {
      printf("  \n ERROR (fatal) -z argument (number of output bands) should be integer 3 or 4. Exiting ... \n");
      exit(1);
    } 
  }

  // make sure bands is 3 or 4
  // *************************
  if( (n_bands!=3) && (n_bands!=4) ) {
    printf("  \n WARNING: -z flag for number of output bands should be 3 or 4. Using default value 3.\n");
    n_bands = 3;
  }

  // make sure for each input argument (name of geotiffs
  // for panchromatic, NIR, red, green, blue bands ... that a file
  // was indeed passed-in
  // *************************************************************
  if( !strlen( pan_filename )) {
    printf( "  \n  ERROR (fatal): panchromatic image file not passed in (-p flag). \n"        );
    printf( "   \n    Exiting ... \n"                                                         );
    Usage();
  } else if( !strlen( nir_filename )) {
    printf( "  \n  ERROR (fatal): NIR (near-infrared) image file not passed in (-p flag). \n" );
    printf("   \n    Exiting ... \n"                                                          );
    Usage();
  } else if( !strlen( red_filename )) {
    printf( "  \n  ERROR (fatal): red image file not passed in (-p flag). \n"                 );
    printf("   \n    Exiting ... \n"                                                          );
    Usage();
  } else if( !strlen( green_filename )) {
    printf( "  \n  ERROR (fatal): green image file not passed in (-p flag). \n"               );
    printf("   \n    Exiting ... \n"                                                          );
    Usage();
  } else if( !strlen( blue_filename )) {
    printf( "  \n  ERROR (fatal): blue image file not passed in (-p flag). \n"                );
    printf("   \n    Exiting ... \n"                                                          );
    Usage();
  } else {;}

  // put the filenames into array of strings
  // or rather ... let's use a hash-map
  // ***************************************
  std::map<std::string,std::string> Imagery;
  Imagery[ "pan"   ] = (std::string)pan_filename;
  Imagery[ "red"   ] = (std::string)red_filename;
  Imagery[ "green" ] = (std::string)green_filename;
  Imagery[ "blue"  ] = (std::string)blue_filename;
  Imagery[ "nir"   ] = (std::string)nir_filename;

  // verify filenames as Geotiff files (e.g. .tif, .TIF extension
  // ************************************************************
  for( auto const& [ImgKey,ImgFileName] : Imagery ) {
    if(!CheckImageFileName( ImgFileName.c_str() )) {
      printf( "  \n  ERROR (fatal): following file should be geotiff              \n" );
      printf( "  (e.g. .TIF,.tif): %s ... Exiting ... \n",ImgFileName.c_str() );
      Usage();
    } 
  } 

  // make sure all images have the same data-type
  // ********************************************
  bool ImagesHaveOneDataType = Pansharpen::ImageryHasOneDataType( Imagery );
  if(!ImagesHaveOneDataType){
    printf( "  \n  ERROR (fatal): all images should have ONE data type ... "                );
    printf("   \n    Exiting ... \n"                                                        );
    Usage();
  }  

  // use image filename-hash to resample each RGB,NIR Geotiff
  // to the same dimensions as the panchromatic image
  // ********************************************************
  std::map<std::string,std::string> ResampledImagery;
  ResampledImagery = ResampleImageGeotiffs( Imagery );

  // perform the pansharpening of the various resampled 
  // image files
  // **************************************************
  Pansharpen PansharpenObj( ResampledImagery  );
  PansharpenObj.PansharpenImagery( n_bands,OutDir );

  // return success of 0 to the operating system
  // *******************************************
  return 0;
}
