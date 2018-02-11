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
 
        Original (RGB) and pan-sharpened (RGB,NIR) Landsat 8 imagery over Argostoli and Lixouri, Kefalonia, Greece. 
   
   ![Alt text](https://lh3.googleusercontent.com/-jcQNJhK7Uuy--VJ12tXpNMSteAkHtigf5B1u5G6BsZ0FteC2NeCACuEewKqT82A2fbQoMsgAOVmBOrjOzG_i-WOndY97S_3ZgFS0QWfqUWu_xnE00UUNOjkUvAefFNbrtTlEZ9JUcFqCNo0FfU-2sQbehlmIG2bR0GmEaBRh3PvDmzOXQAAqDMKgg3glzCTzxTm_NiM7iGx9HECUdRe2dmf38t-5J6oakUTpCV_D1Bo2_frSvP6cV7tR2rVrfOtcvde-bJNBHip5931YPAeW1sx9Lf1rblu372bxuXmRcLLaL-85n97RzAyZJW3CtjupxH6-Ypl90iDiCmI08sCOr8YrE4XyALRj7RCgayozDe1NorQg6T25t7RJ4_f0R6Jo8qfi1oLDnMD4ifoZPge_JNcCkMWit3-fdLScWm_hEb9Y91pxYd7YDUomI02829TqJhYol2ls41MrNwgfv4aEkTFpMMEDao-FKdjtDC1XS-j55ZkNWsK7yzK27zT53HPowAxzqDmGJ3CN3d7oA-Puhyo2yCP1kmt_011lBFlsUVizgosmLkcEu6e7ZhFYhhZINwizz3uYBxZMBjfQlsOedmv8JB_c_vPgsg8sxE=w807-h531-no)
   
   ![Alt text](https://lh3.googleusercontent.com/yRRFObIFi_KoUWHAU3wzESSS1Rd6YZh7k170heuUAJ46WoZM9iG5_IdFwrr0jUi93Y9E2qWzEq54WcliVlT-gBkQma6X7-DUDmote7qA4I9SIlVST6gHpLNWdzGpnsJoQRNvtC2HWS0KCjKw1_JHeQr3J66NVUEFoHqn93bc9xfnJ-BkwbjHM1kiSsHU6kChgMEEZJQQjNYwVBPaMr-dp19JCZxB48_22axVGZRfiu9Y6wEgEeuc62zdlpqSqIWl1t37HVeaAY8MbQf74TE32hxi5Q5w9m4CwmLpxis7fztfQqKPp4EMuVn7kxehCk2RenEMImy25enttn8Ujv-pJYuccSShj8GqQgNVsuV5-BH_0Y-rhZukeiF_QRqgUlQbTR8LL7etzT_Qdl06MrKf2anG-DPasbbCdNqUo3OJSvV0X0MAqxyOxp7ClpEkrUzSNuatjhEzrdEUNkdTzZsiZAaizAtqULMVsERmVNTuja8O59DyrNGS05VzlSq8SzQrnvT8jKHPvNjPCId6ytNl2TNosZ1vhimT5-iJ_DCNR8Rbm-NRJu1VUDr4QREilPMeqmzOAftBg79r6mOhKSVDpWRkMatdyr2zzld0wCE=w437-h288-no)
   
   
   
   
   
   
   
   
   
   

