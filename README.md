 NAME:
 
   pansharpen
   
 DESCRIPTION:
 
   This C/C++ program is meant to pansharpen a set of 4 
   one-band Geotiff files holding low-resolution imagery for 
   a set of NIR, red, green, and blue bands. This is done by 
   resampling this imagery using bicubic resampling and applying 
   the Brovey and FIHS image pan-sharpening algorithms and 
   transformations. Imagery provided by a 1 band panchromatic Geotiff 
   image file are also necessary. 
   
 INPUTS:
 
   Data from five 1-band Geotiffs are necessary to run this program.
   These Geotiffs are a panchromatic image file (high-res.) and the 
   Geotiffs holding pixel data for the red, green, blue, and NIR bands.
   
 OUTPUTS:
 
   Two 4-band Geotiff image files holding the pan-sharpened image bands 
   for the pan-sharpened red, green, blue, and NIR bands. Their geotransform 
   and projection information reflects the input panchromatic Geotiff image
   file passed-in. One 4-band output file is a result of using the Brovey 
   transform pan-sharpening, and the other using the Fast Intensity Hue 
   Saturation pan-sharpening technique (FIHS).
   
 USAGE: 
 
    $ make 
    $ ./pansharpen  
     (1) <PANCHROMATIC_GEOTIFF>{.tif} 
     (2) <NIR_GEOTIFF>{.tif} 
     (3) <RED_GEOTIFF>{.tif} 
     (4) <GREEN_GEOTIFF>{.tif} 
     (5) <BLUE_GEOTIFF>{.tif}

   Example: 
   
     $ ./pansharpen pan.tif NIR.tif red.tif green.tif blue.tif 
 
  AUTHOR: 
  
   Gerasimos "Geri" Michalitsianos
   gerasimosmichalitsianos@gmail.com
   
   ![Alt text](https://lh3.googleusercontent.com/-jcQNJhK7Uuy--VJ12tXpNMSteAkHtigf5B1u5G6BsZ0FteC2NeCACuEewKqT82A2fbQoMsgAOVmBOrjOzG_i-WOndY97S_3ZgFS0QWfqUWu_xnE00UUNOjkUvAefFNbrtTlEZ9JUcFqCNo0FfU-2sQbehlmIG2bR0GmEaBRh3PvDmzOXQAAqDMKgg3glzCTzxTm_NiM7iGx9HECUdRe2dmf38t-5J6oakUTpCV_D1Bo2_frSvP6cV7tR2rVrfOtcvde-bJNBHip5931YPAeW1sx9Lf1rblu372bxuXmRcLLaL-85n97RzAyZJW3CtjupxH6-Ypl90iDiCmI08sCOr8YrE4XyALRj7RCgayozDe1NorQg6T25t7RJ4_f0R6Jo8qfi1oLDnMD4ifoZPge_JNcCkMWit3-fdLScWm_hEb9Y91pxYd7YDUomI02829TqJhYol2ls41MrNwgfv4aEkTFpMMEDao-FKdjtDC1XS-j55ZkNWsK7yzK27zT53HPowAxzqDmGJ3CN3d7oA-Puhyo2yCP1kmt_011lBFlsUVizgosmLkcEu6e7ZhFYhhZINwizz3uYBxZMBjfQlsOedmv8JB_c_vPgsg8sxE=w807-h531-no)
   
   ![Alt text](https://lh3.googleusercontent.com/G6C-OVwRq1PXHPsyceGSi6jaHRtefDfA4dy51cvhiZ8Q_LxQSPDPttK8xIB8VpQTEL9m3loq5lJm68WbzpP2hSXpSd1DodRfowWEQNqIcPIUii4ymQUmi71wDSuqham4k2Zs7oZPhqUv8JwGSFj4m8P7BftIjrNk9SusYFveW21yLpN1g52jhgbkrsKibc0sB1jAjKFkPQQv4noQdL5MfXNv5BCX6JT1VD2F0FzZMy1zSlkk6EPJXWsICI59AWETWDlerrsJUWHmmFXkyispaIMgVmJc5loO_3WNWcYV1erDtkgyOeiHkMQJkqjHy1RgbhWZZ9sx555RD0rPuZ1tL-bvrhtRpCUPrSngwM0D3Y5qZcXAcY--Fq-Vr7jhO46sOP3CEne-aK-dO5KdBvqxsliCisFTcVEevCHoyPtYEhjZ0PekSMrg6lddZ-TwgKHmVm7ea9d5yWE_GCtgW9uUkN711p9SHiEajGpE8ms5fR0xd8FY68WiEk6LCRsweeP0KRC4y2DZ1F1VjWnmopiUNOsKfGv_Qjiwojf5yWkNEQMDdiHoKUueR6qmQWOgg3OKWdgfvproGHWnOJIyl_ci4g6zs3ySzWnPF8s2sKg=w334-h220-no)
   
   
   
   
   
   
   
   
   
   

