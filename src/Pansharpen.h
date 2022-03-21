#ifndef PANSHARPEN_H_
#define PANSHARPEN_H_
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include "unistd.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdalwarper.h"
#include <filesystem>
#include "ogr_spatialref.h"

class Pansharpen {
  private:
    std::map<std::string,std::string> ImageryFileNames;
  public:
    // overloaded constructor functions
    Pansharpen();
    Pansharpen( std::map<std::string,std::string> );
    
    // number of images
    // ****************
    static const int N_IMAGES  = 5;

    // define any static method(s)
    // ***************************
    static bool ImageryHasOneDataType( std::map<std::string,std::string>& ); 

    // define template class function
    // ******************************
    void PansharpenImagery( int,const char* );
    template<typename T>
    void WritePansharpenedImagery( int,const char* );
};
#endif
