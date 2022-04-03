#include "Pansharpen.h"
typedef std::string String;

// include external C source file. This is how
// we can call C from C++
// *******************************************
extern "C" {
  #include "GeotiffUtil.c"
};

// constructor with no arguments
// *****************************
Pansharpen::Pansharpen() {};

// constructor that takes in std::map<String,String>
// *************************************************
Pansharpen::Pansharpen( std::map<String,String> Imagery ) {
  ImageryFileNames = Imagery;	  
}

bool Pansharpen::ImageryHasOneDataType( std::map<String,String>& Imgs ) {
  /* ******************************************************************************
   * bool Pansharpen::ImageryHasOneDataType( std::map<String,String>& ):
   *
   * This function returns a boolean True or False to determine if
   * each of the geotiffs for the Red,Green,Blue, NIR, and 
   * panchromatic bands all have the same data-type (as indicated
   * by a GDAL data-type integer). To this end, the data-types of
   * each of these 5 images is stored into a C++ integer array.
   *
   * Then, the standard deviation of this array is computed.
   * If the standard deviation is grater than zero, then the imput
   * imagery (geotiffs) have more than one data-type, so this 
   * function returns false; otherwise, this function returns
   * true if the standard deviation is zero. 
   *
   * Args:
   *   std::map<std::string,std::string>& : reference to map with image filenames.
   * Returns:
   *   bool: true or false.
   */

  // 5 element array. because there are 5 images:
  //   red,green,blue,NIR, and panchromatic images (5 files)
  // *******************************************************
  GDALAllRegister();
  long ImageDataTypes[N_IMAGES];

  // iterate through imagery and get the GDAL
  // data type
  // ****************************************
  int ImgCounter  = 0;
  GDALDataset *ds = nullptr;
  GDALDataType dtype;
  
  for( auto const& [FileNameKey,ImgFileName] : Imgs ) {
    // open up the dataset as GDAL dataset
    // ***********************************	 
    ds = (GDALDataset*) GDALOpen( ImgFileName.c_str(),GA_ReadOnly );
    dtype = GDALGetRasterDataType( ds->GetRasterBand(1));
    ImageDataTypes[ImgCounter] = dtype;
    GDALClose(ds);
    ++ImgCounter;
  }

  // compute the standard deviation.
  //   if std. dev > 0: return false, else return true
  // *************************************************
  float stdDev = 0.0;
  float total  = 0.0;
  float mean   = 0.0;
  ImgCounter   = 0;

  // append total
  // ************
  while( ImgCounter<N_IMAGES ) {
    total += ImageDataTypes[ImgCounter];
    ImgCounter++;
  }

  // calculate total. reset counter.
  // *******************************
  mean = total/((float)N_IMAGES);
  ImgCounter = 0;

  // compute standard deviation
  // **************************
  while( ImgCounter<N_IMAGES ){
    stdDev += pow( ImageDataTypes[ImgCounter]-mean,2 );
    ImgCounter++;
  }

  // calculate final value of standard deviation
  // *******************************************
  stdDev = sqrt( stdDev / (float)N_IMAGES );
  
  // if standard deviation of data types > 0, then we have
  // more than one data type. return false. else, return true.
  // *********************************************************
  if( stdDev>0.0 ) {
    return false;
  } else {
    return true;
  }
}

void Pansharpen::PansharpenImagery( int n_out_bands,const char* OutDir ) {
  /* ******************************************************
   * Pansharpen::PansharpenImagery( int n_out_bands ):
   * 
   * This function uses a C++ switch{} statement to pass the 
   * approprate C++ data type to the template function 
   * WritePansharpenedImagery (see below). This latter
   * function writes out a pair of geotiffs containing
   * pansharpened imagery (with either 3 or 4 bands for
   * RGB or RGB/NIR).
   *
   * Args:
   *   n_out_bands (int): Integer , should be 3 or 4.
   * Returns:
   *   None. Void.
   */

  // open up panchromatic dataset ... get GDAL data-type.
  // ****************************************************
  GDALDataset *Pan = nullptr;
  Pan = (GDALDataset*) GDALOpen( 
    this->ImageryFileNames[ "pan" ].c_str(),GA_ReadOnly );
  GDALDataType GDAL_DataType  = GDALGetRasterDataType(Pan->GetRasterBand(1));
  GDALClose(Pan);

  switch( GDAL_DataType )
  { // check each of different GDAL imagery data types.
    case 0:
      printf("image file has unknown pixel data type: %s. Exiting ...\n",
        ImageryFileNames["pan"]);
      exit(1);
    case 1:
      // GDAL GDT_Byte (-128 to 127) - unsigned  char
      WritePansharpenedImagery<unsigned char>( n_out_bands,OutDir );
      break; 
    case 2:
      // GDAL GDT_UInt16 - short
      WritePansharpenedImagery<unsigned short>( n_out_bands,OutDir );
      break;
    case 3:
      // GDT_Int16
      WritePansharpenedImagery<short>( n_out_bands,OutDir );
      break;
    case 4:
      // GDT_UInt32
      WritePansharpenedImagery<unsigned int>( n_out_bands,OutDir );
      break;
    case 5:
      // GDT_Int32
      WritePansharpenedImagery<int>( n_out_bands,OutDir );
      break;
    case 6:
      // GDT_Float32
      WritePansharpenedImagery<float>( n_out_bands,OutDir );
      break;
    case 7:
      // GDT_Float64
      WritePansharpenedImagery<double>( n_out_bands,OutDir );
      break;
    default:     
      printf("ERROR (fatal): unknown GDAL imagery data type. Exiting ...");
      exit(1);
  }

  // clean up resampled imagery as it is no longer needed
  // ****************************************************
  for( auto const& [FileNameKey,ImgFileName] : ImageryFileNames ) {
    if( FileNameKey == "pan" ) {
      continue;
    } else {
      // remove the bicubic resampled image file ... no longer needed
      // ************************************************************
      std::remove( ImgFileName.c_str() );
    }
  }

}

// template method
template<typename T>
void Pansharpen::WritePansharpenedImagery( int N_bands,const char* OutDir ) {
  /* ************************************************************ 
   * void Pansharpen::WritePansharpenedImagery( int N_bands ):
   * 
   * This function writes out a pair of 3 or 4 band geotiffs
   * for the pan-sharpened FIHS and Brovey imagery. If these
   * pan-sharpened Geotiffs have 3 bands, then they are the
   * Red,Green, and Blue (RGB) pan-sharpened bands. Otherwise,
   * the Geotiffs contain the Red, Green, Blue, and NIR 
   * band (or 4 bands).
   *
   * Args:
   *   N_bands (int): Number of bands, should be 3 or 4.
   * Returns:
   *   None. Void.
   */

  // get the name of the panchromatic geotiff, reads its attributes
  // into a C structure into C++
  // **************************************************************  
  std::string PanFileName = this->ImageryFileNames[ "pan" ];
  Geotiff PanGeotiff = ReadGeotiff( PanFileName.c_str() );
  
  // read attrributes for panchromatic dataset
  // *****************************************
  const char* prj    = (const char*)PanGeotiff.projection;
  double *gt         = PanGeotiff.geotransform;
  double NoDataValue = PanGeotiff.NoDataValue;
  
  // read the 2d dimensions of the band
  // **********************************
  int N_COLS,N_ROWS;
  N_COLS = PanGeotiff.xsize; // number of columns
  N_ROWS = PanGeotiff.ysize; // number of rows

  // initialize GDAL datasets for Red,Green,Blue,NIR, and Panchromatic
  // Geotiff image datasets.
  // *****************************************************************
  GDALDataset *panDataset   = nullptr;
  GDALDataset *redDataset   = nullptr;
  GDALDataset *greenDataset = nullptr;
  GDALDataset *blueDataset  = nullptr;
  GDALDataset *nirDataset   = nullptr;

  // open up all datasets ... including Red,Green,Blue,Panchromatic,and NIR
  // geotiffs. Open them as GDAL datasets.
  // **********************************************************************
  panDataset   = (GDALDataset*) GDALOpen( 
    this->ImageryFileNames[ "pan"   ].c_str(),GA_ReadOnly );
  redDataset   = (GDALDataset*) GDALOpen( 
    this->ImageryFileNames[ "red_resampled"   ].c_str(),GA_ReadOnly );
  greenDataset = (GDALDataset*) GDALOpen( 
    this->ImageryFileNames[ "green_resampled" ].c_str(),GA_ReadOnly );
  blueDataset  = (GDALDataset*) GDALOpen( 
    this->ImageryFileNames[ "blue_resampled"  ].c_str(),GA_ReadOnly );
  nirDataset   = (GDALDataset*) GDALOpen( 
    this->ImageryFileNames[ "nir_resampled"   ].c_str(),GA_ReadOnly );

  // create GDAL driver object for writing geotiffs
  // **********************************************
  GDALDriver *driverGeotiff;
  driverGeotiff = GetGDALDriverManager()->GetDriverByName("GTiff");
 
  // establish output filenames by joining them with the output directory
  // that was passed into this function
  // ********************************************************************
  std::filesystem::path Dir(OutDir);

  std::filesystem::path OutNameFIHS("sharpened_FIHS.tif");
  std::filesystem::path OutNameBrovey("sharpened_Brovey.tif");
  
  std::filesystem::path fullPathFIHS   = Dir / OutNameFIHS;
  std::filesystem::path fullPathBrovey = Dir / OutNameBrovey; 

  // begin to write the FIHS geotiff dataset
  // ***************************************
  GDALDataset *fihsDataset;
  fihsDataset = driverGeotiff->Create( fullPathFIHS.c_str(),N_COLS,N_ROWS,N_bands,GDT_Float32,NULL );
  fihsDataset->SetGeoTransform(gt);
  fihsDataset->SetProjection(prj);

  // begin to write Brovey geotiff dataset
  // *************************************
  GDALDataset *broveyDataset;
  broveyDataset = driverGeotiff->Create( fullPathBrovey.c_str(),N_COLS,N_ROWS,N_bands,GDT_Float32,NULL );
  broveyDataset->SetGeoTransform(gt);
  broveyDataset->SetProjection(prj);

  // get the band data-type as well as number of bytes
  // per pixel
  // *************************************************
  GDALDataType bandType = GDALGetRasterDataType(
    panDataset->GetRasterBand(1));
  int nbytes = GDALGetDataTypeSizeBytes(bandType);

  // allocate row buffers for image scanlines
  // ****************************************
  T *rowBuffPan   = (T*) CPLMalloc( nbytes*N_COLS );
  T *rowBuffRed   = (T*) CPLMalloc( nbytes*N_COLS );
  T *rowBuffGreen = (T*) CPLMalloc( nbytes*N_COLS );
  T *rowBuffBlue  = (T*) CPLMalloc( nbytes*N_COLS );
  T *rowBuffNIR   = (T*) CPLMalloc( nbytes*N_COLS );

  // row buffers for the pan-sharpened, brovey pan-sharpened datasets
  // **************************************************************** 
  float *rowBuffFIHS   = (float*) CPLMalloc( nbytes*N_COLS );
  float *rowBuffBrovey = (float*) CPLMalloc( nbytes*N_COLS );

  // allocate variables for pixel values
  // ***********************************
  float pan_value,red_value,green_value,blue_value,NIR_value;
  float L,sum_pixels;
  float redSharp_FIHS,greenSharp_FIHS,blueSharp_FIHS,nirSharp_FIHS;
  float redSharp_Brovey,greenSharp_Brovey,blueSharp_Brovey,nirSharp_Brovey;

  // initialize errors for reading bands (statuses)
  // **********************************************
  CPLErr e_Pan,e_Red,e_Green,e_Blue,e_NIR;

  // iterate through fields of view and scanlines
  // ********************************************
  int band=1;
  while( band<N_bands+1 ) {
    for( int row=0;row<N_ROWS;row++ ) {
    
      // read the scanline into the dynamically allocated row-buffer       
      // ***********************************************************
      e_Pan = panDataset->GetRasterBand(1)->RasterIO(
        GF_Read,0,row,N_COLS,1,rowBuffPan,N_COLS,1,bandType,0,0   );
      e_Red = redDataset->GetRasterBand(1)->RasterIO(
        GF_Read,0,row,N_COLS,1,rowBuffRed,N_COLS,1,bandType,0,0   );
      e_Green = greenDataset->GetRasterBand(1)->RasterIO(
        GF_Read,0,row,N_COLS,1,rowBuffGreen,N_COLS,1,bandType,0,0 );
      e_Blue = blueDataset->GetRasterBand(1)->RasterIO(
        GF_Read,0,row,N_COLS,1,rowBuffBlue,N_COLS,1,bandType,0,0  );
      e_NIR = nirDataset->GetRasterBand(1)->RasterIO(
        GF_Read,0,row,N_COLS,1,rowBuffNIR,N_COLS,1,bandType,0,0   );

      // check to make sure we are able to read all bands
      // ************************************************
      if(!(e_Pan == 0)){
        printf("  \n ERROR (fatal): Unable to read band from \n");
        printf("      panchromatic image file (e.g. using -p flag). Exiting ... \n");
        exit(1);
      } else if( !(e_Red == 0)) {
        printf("  \n ERROR (fatal): Unable to read band from \n");
        printf("      red image file (e.g. using -r flag). Exiting ...          \n");
        exit(1);
      } else if( !(e_Green == 0)) {
        printf("  \n ERROR (fatal): Unable to read band from \n");
        printf("      green image file (e.g. using -g flag). Exiting ...        \n");
        exit(1);
      } else if( !(e_Blue == 0)) {
        printf("  \n ERROR (fatal): Unable to read band from \n");
        printf("      blue image file (e.g. using -g flag). Exiting ...         \n");
        exit(1);
      } else if( !(e_NIR == 0)) {
        printf("  \n ERROR (fatal): Unable to read band from \n");
        printf("      NIR image file (e.g. using -n flag). Exiting ...          \n");
        exit(1);
      } else {;}

      // iterate through columns
      // ***********************
      for( int col=0; col<N_COLS; col++ ) {
        pan_value         = (float)rowBuffPan[col];
        red_value         = (float)rowBuffRed[col];
        green_value       = (float)rowBuffGreen[col];
        blue_value        = (float)rowBuffBlue[col];
        NIR_value         = (float)rowBuffNIR[col];

	// based on number of bands (3 for RGB or 4 for RGB/NIR) then
	// compute the linear scaling factors for pan-sharpening
	// **********************************************************
	if( N_bands == 4 ) {
          L               = ( red_value+green_value+blue_value+NIR_value )/N_bands;
          sum_pixels      = ( red_value+green_value+blue_value+NIR_value );
        } else {
          L               = ( red_value+green_value+blue_value )/N_bands;
          sum_pixels      = ( red_value+green_value+blue_value );
	}

	// if the panchromatic value is NoData or less than zero, just
	// set the out pixel value(s) to zero for all pan-sharpened bands
	// **************************************************************
        if( !(pan_value<0.0) || pan_value == NoDataValue ) {
          // calculate pan-sharpend FIHS values
          // ********************************** 
          redSharp_FIHS     = red_value   + ( pan_value - L );
          greenSharp_FIHS   = green_value + ( pan_value - L );
          blueSharp_FIHS    = blue_value  + ( pan_value - L );
          nirSharp_FIHS     = NIR_value   + ( pan_value - L );

          // calculate pan-sharpened values using Brovey 
          // *******************************************
          redSharp_Brovey   = ( red_value   / sum_pixels ) * pan_value;
          greenSharp_Brovey = ( green_value / sum_pixels ) * pan_value;
          blueSharp_Brovey  = ( blue_value  / sum_pixels ) * pan_value;
	  nirSharp_Brovey   = ( NIR_value   / sum_pixels ) * pan_value;

        } else {
          // calculate pan-sharpend FIHS values
          // ********************************** 
          redSharp_FIHS      = 0.0; 
          greenSharp_FIHS    = 0.0;
          blueSharp_FIHS     = 0.0;
          nirSharp_FIHS      = 0.0;

          // calculate pan-sharpened values using Brovey 
          // *******************************************
          redSharp_Brovey    = 0.0;
          greenSharp_Brovey  = 0.0;
          blueSharp_Brovey   = 0.0;
	  nirSharp_Brovey    = 0.0;
	}

	// depending on which band we are currently writing, write out
	// the pan-sharpened pixel values to the row memory buffers
	// ***********************************************************
	if( band == 1 ) {
          rowBuffFIHS[col]   = redSharp_FIHS;
          rowBuffBrovey[col] = redSharp_Brovey;
        } else if( band == 2 ) {
          rowBuffFIHS[col]   = greenSharp_FIHS;
          rowBuffBrovey[col] = greenSharp_Brovey;
	} else if( band == 3 ) {
          rowBuffFIHS[col]   = blueSharp_FIHS;
          rowBuffBrovey[col] = blueSharp_Brovey;
	} else {
          rowBuffFIHS[col]   = nirSharp_FIHS;
          rowBuffBrovey[col] = nirSharp_Brovey;
	}
      }

      // write out band/scaneline
      // ************************
      fihsDataset->GetRasterBand(band)->RasterIO(
        GF_Write,0,row,N_COLS,1,rowBuffFIHS,N_COLS,1,GDT_Float32,0,0);
      broveyDataset->GetRasterBand(band)->RasterIO(
        GF_Write,0,row,N_COLS,1,rowBuffBrovey,N_COLS,1,GDT_Float32,0,0);
    }
    band++;
  }

  // close all Geotiff datasets
  // **************************
  GDALClose( panDataset    );
  GDALClose( redDataset    );
  GDALClose( greenDataset  );
  GDALClose( blueDataset   );
  GDALClose( nirDataset    );

  GDALClose( fihsDataset   );
  GDALClose( broveyDataset );

  // ***************************
  // release memory for scanline
  CPLFree( rowBuffPan   );
  CPLFree( rowBuffRed   );
  CPLFree( rowBuffGreen );
  CPLFree( rowBuffBlue  );
  CPLFree( rowBuffNIR   );

  // close GDAL drivers
  // ******************
  GDALDestroyDriverManager();
}
