 ###### NAME:
     bin/pansharpen
 ###### DESCRIPTION:
     This C/C++ program is meant to pansharpen a set of 3 or 4 
     one-band Geotiff files holding low-resolution imagery for 
     a set of red, green, blue, and optionally, NIR bands. This is done by 
     resampling this imagery using bicubic resampling and applying 
     the Brovey and FIHS image pan-sharpening algorithms and 
     transformations. Imagery provided by a 1 band panchromatic Geotiff 
     image file are also necessary. 
 ###### INPUTS:
     Data from five 1-band Geotiffs are necessary to run this program.
     These Geotiffs are a panchromatic image file (high-res.) and the 
     Geotiffs holding pixel data for the red, green, blue, and NIR bands.
     These 1-band files should contain 2-byte unsigned integer 
     ("short" data-type in C) pixel array data. 
  ###### OUTPUTS:
     Two 4-band Geotiff image files holding the pan-sharpened image bands 
     for the pan-sharpened red, green, blue, and NIR bands. Their geotransform 
     and projection information reflects the input panchromatic Geotiff image
     file passed-in. One 4-band output file is a result of using the Brovey 
     transform pan-sharpening, and the other using the Fast Intensity Hue 
     Saturation pan-sharpening technique (FIHS).
  ###### INSTRUCTIONS: 
     In makefile, be sure to update the directory strings as appropriate 
     for your computer. These directory strings should reflect the 
     installation location of your GDAL header (.h) and library (.a) files. 
  ###### STANDARD USAGE WITHOUT DOCKER
      If not building a Docker image and running as a Docker container, 
      then it is expected that you will have the appropriate C/C++ GDAL header 
      and include files installed onto your system. In a Debian Linux system
      (e.g. Mint, Ubuntu), these files may be installed simply with:
      
      $ apt-get install gdal-bin -y
      $ apt-get install libgdal-dev -y

      Then, after adjusting the paths as appropriate in the makefile, this program
      can be compiled and run. Here is an example using the Linux command-line:
      
      $ git clone https://github.com/gerasimosmichalitsianos/pansharpening_cpp
      $ cd pansharpening_cpp/
      $ ls sample_data
      BLUE.TIF GREEN.TIF NIR.TIF  PAN.TIF RED.TIF
      $ make
      $ # now there should be an executable called pansharpen inside of bin/
      $ mkdir outputs
      $ ./bin/pansharpen 
        -p sample_data/PAN.TIF 
        -b sample_data/BLUE.TIF 
        -r sample_data/RED.TIF 
        -n sample_data/NIR.TIF 
        -g sample_data/GREEN.TIF 
        -z 3 
        -o outputs
        
      So the inputs should be Geotiffs for the red, green, blue, and panchromatic bands (required).
      The panchromatic band should be the higher resolution geotiff, with the red, green, blue, and NIR
      bands as the lower-resolution inputs.
      
      There will be 2 outputs: sharpened_Brovey.tif and sharpened_FIHS.tif. These will be 3 or 4
      band geotiffs written to the output directory (-o flag). If not supplying a -o flag, then the
      current working directory $(pwd) is used instead.
      
      Note that the -z flag should be 3 or 4 for the number of bands in the output pan-sharpened 
      Geotiffs. It must be 3 or 4 (an integer).
  
  ###### USAGE WITH DOCKER: 


 ######  AUTHOR: 
  
     Gerasimos "Geri" Michalitsianos
     gerasimosmichalitsianos@gmail.com
     21 March 2022
   
 ###### EXAMPLE OUTPUT IMAGES: 
 
        Right: Original (RGB) image over Lakithra, island of Kefalonia, Greece
        Center: Pan-sharpened RGB using Fast Intensity Hue Saturation pansharpening method (FIHS) 
        Left: Pan-sharpened RGB using Brovey algorithm and/or technique.
   
   ![Alt text](https://i.imgur.com/LgkEZAC.png)

   
   
   
   
   
   
   
   
   
   

