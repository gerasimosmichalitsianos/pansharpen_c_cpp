 ###### NAME:
     bin/pansharpen
 ###### DESCRIPTION:
     This C/C++ program is meant to pansharpen a set of 3 or 4 
     one-band Geotiff files holding low-resolution imagery for 
     a set of red, green, blue, and optionally, NIR bands. This is done by 
     resampling this imagery using bicubic resampling and applying 
     the Brovey and FIHS image pan-sharpening algorithms and 
     transformations. Imagery provided by a 1 band, higher-resolution, panchromatic Geotiff 
     image file are also necessary. 
 ###### INPUTS:
     Data from five 1-band Geotiffs are necessary to run this program.
     These Geotiffs are a panchromatic image file (high-res.) and the 
     Geotiffs holding pixel data for the red, green, blue, and NIR bands.
  ###### OUTPUTS:
     Two 3 or 4-band Geotiff image files holding the pan-sharpened image bands 
     for the pan-sharpened red, green, blue, and (optionally) NIR bands. Their 
     geotransform and projection information reflects the input panchromatic Geotiff image
     file passed-in. One 3 or 4-band output file is a result of using the Brovey 
     transform pan-sharpening, and the other using the Fast Intensity Hue 
     Saturation pan-sharpening technique (FIHS).
  ###### STANDARD USAGE WITHOUT DOCKER
      If not building a Docker image and running as a Docker container, 
      then it is expected that you will have the appropriate C/C++ GDAL header 
      and include files installed onto your system. In a Debian Linux system
      (e.g. Mint, Ubuntu), these files may be installed simply with
      (assuming you are root):
      
      $ apt-get install gdal-bin -y
      $ apt-get install libgdal-dev -y
      
      It is also expected you will have a C++ compiler installed (e.g. g++),
      as well as make:
      
      $ apt-get update
      $ apt-get install -y apt-utils
      $ apt-get -y install make
      $ apt install g++ -y

      Then, after adjusting the paths as appropriate in the makefile, this program
      can be compiled and run. Here is an example using the Linux command-line:
      
      $ git clone https://github.com/gerasimosmichalitsianos/pansharpen_c_cpp
      $ cd pansharpen_c_cpp/
      $ ls sample_data
      BLUE.TIF GREEN.TIF NIR.TIF  PAN.TIF RED.TIF
      $ make clean
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
      
      Use the -p for the panchromatic geotiff, -r for the red geotiff, -b for the blue geotiff,
      -n for the NIR (near-infrared) geotiff, and -g for the green geotiff.
      
      There will be 2 outputs: sharpened_Brovey.tif and sharpened_FIHS.tif. These will be 3 or 4
      band geotiffs written to the output directory (-o flag). If not supplying a -o flag, then the
      current working directory $(pwd) is used instead. If z is 3, then the outputs will contain the
      pan-sharpened RGB bands; if z is 4, then they will contain the pan-sharpened RGB and NIR bands
      (4 bands).
      
      Note that the -z flag should be 3 or 4 for the number of bands in the output pan-sharpened 
      Geotiffs. It must be 3 or 4 (an integer).
  
  ###### USAGE WITH DOCKER: 

      This software can be built as a Docker image and run as a Docker container. See the example
      below. It is, as usual, expected that you will be using a UNIX-like and/or Linux command-line,
      or something similar (e.g. Ubuntu subsystem for Windows).
      
      $ git clone https://github.com/gerasimosmichalitsianos/pansharpen_c_cpp
      $ cd pansharpen_c_cpp/
      $ ls
      bin  Dockerfile  makefile  README.md  src  sample_data

      Now build and run the Docker container:
      $ which docker
      /usr/bin/docker
      $ docker build -t pansharpen .

      Then use your 3 or 4 input data files (.TIF, .tif) to run the Docker image
      "pansharpen" as a Docker container:

      $ ls sample_data
      BLUE.TIF  GREEN.TIF  NIR.TIF  PAN.TIF  RED.TIF
      $ DIR=$(pwd)/sample_data
      $ docker run -v $DIR:$DIR pansharpen 
        -p $DIR/PAN.TIF
        -b $DIR/BLUE.TIF
        -r $DIR/RED.TIF
        -n $DIR/NIR.TIF
        -g $DIR/GREEN.TIF
        -z 3
        -o $DIR

 ######  AUTHOR: 
  
     Gerasimos "Geri" Michalitsianos
     lakithra@protonmail.com
     21 March 2022
   
 ###### EXAMPLE OUTPUT IMAGES: 
 
     Right: Original (RGB) image over Lakithra, island of Kefalonia, Greece
     Center: Pan-sharpened RGB using Fast Intensity Hue Saturation pansharpening method (FIHS) 
     Left: Pan-sharpened RGB using Brovey algorithm and/or technique.
   
   ![Alt text](https://i.imgur.com/LgkEZAC.png)
