 ###### NAME:
 
     pansharpen
   
 ###### DESCRIPTION:
 
     This C/C++ program is meant to pansharpen a set of 4 
     one-band Geotiff files holding low-resolution imagery for 
     a set of NIR, red, green, and blue bands. This is done by 
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
   
  ###### USAGE: 
 
    $ make 
    $ ./pansharpen  
     (1) <PANCHROMATIC_GEOTIFF>{.tif} 
     (2) <NIR_GEOTIFF>{.tif} 
     (3) <RED_GEOTIFF>{.tif} 
     (4) <GREEN_GEOTIFF>{.tif} 
     (5) <BLUE_GEOTIFF>{.tif}

###### EXAMPLE USAGE: 
   
     $ ./pansharpen pan.tif NIR.tif red.tif green.tif blue.tif 
 
 ######  AUTHOR: 
  
     Gerasimos "Geri" Michalitsianos
     gerasimosmichalitsianos@gmail.com
     SGT, Inc. 
     February 2018 
   
 ###### EXAMPLE OUTPUT IMAGES: 
 
        Original (RGB) & pan-sharpened (RGB,NIR) Landsat 8 imagery, Argostoli & Lixouri, Kefalonia, Greece. 
        
   
   ![Alt text](https://lh3.googleusercontent.com/-p8HiA4RuEJ8/VYmh_ttK-uI/AAAAAAAAADc/1220R530qfM/w800-h800/pansharpeningExamples.jpg)

   
   
   
   
   
   
   
   
   
   

