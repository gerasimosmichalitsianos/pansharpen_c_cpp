 ###### NAME:
     pansharpen
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
  
  ###### STANDAR USAGE
      If not using building a Docker image, then it is expected that you will
      have the GDAL header and include files for C/C++. In a Debian Linux system
      (e.g. Mint, Ubuntu), these files may be installed simply with:
      
      $ 
  
  ###### USAGE WITH DOCKER: 
 
    $ make 
    $ 


 ######  AUTHOR: 
  
     Gerasimos "Geri" Michalitsianos
     gerasimosmichalitsianos@gmail.com
     21 March 2022
   
 ###### EXAMPLE OUTPUT IMAGES: 
 
        Original (RGB) & pan-sharpened (RGB,NIR) Landsat 8 imagery, Argostoli, Kefalonia, Greece. 
   
   ![Alt text](https://i.imgur.com/LgkEZAC.png)

   
   
   
   
   
   
   
   
   
   

