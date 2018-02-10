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
     These files should contained unsigned integer (short in C) array data. 
   
  ###### OUTPUTS:
 
     Two 4-band Geotiff image files holding the pan-sharpened image bands 
     for the pan-sharpened red, green, blue, and NIR bands. Their geotransform 
     and projection information reflects the input panchromatic Geotiff image
     file passed-in. One 4-band output file is a result of using the Brovey 
     transform pan-sharpening, and the other using the Fast Intensity Hue 
     Saturation pan-sharpening technique (FIHS).
   
  ###### USAGE: 
 
    $ make 
    $ ./pansharpen  
     (1) <PANCHROMATIC_GEOTIFF>{.tif} 
     (2) <NIR_GEOTIFF>{.tif} 
     (3) <RED_GEOTIFF>{.tif} 
     (4) <GREEN_GEOTIFF>{.tif} 
     (5) <BLUE_GEOTIFF>{.tif}

   Example: 
   
     $ ./pansharpen pan.tif NIR.tif red.tif green.tif blue.tif 
 
 ######  AUTHOR: 
  
     Gerasimos "Geri" Michalitsianos
     gerasimosmichalitsianos@gmail.com
   
   ![Alt text](https://lh3.googleusercontent.com/-jcQNJhK7Uuy--VJ12tXpNMSteAkHtigf5B1u5G6BsZ0FteC2NeCACuEewKqT82A2fbQoMsgAOVmBOrjOzG_i-WOndY97S_3ZgFS0QWfqUWu_xnE00UUNOjkUvAefFNbrtTlEZ9JUcFqCNo0FfU-2sQbehlmIG2bR0GmEaBRh3PvDmzOXQAAqDMKgg3glzCTzxTm_NiM7iGx9HECUdRe2dmf38t-5J6oakUTpCV_D1Bo2_frSvP6cV7tR2rVrfOtcvde-bJNBHip5931YPAeW1sx9Lf1rblu372bxuXmRcLLaL-85n97RzAyZJW3CtjupxH6-Ypl90iDiCmI08sCOr8YrE4XyALRj7RCgayozDe1NorQg6T25t7RJ4_f0R6Jo8qfi1oLDnMD4ifoZPge_JNcCkMWit3-fdLScWm_hEb9Y91pxYd7YDUomI02829TqJhYol2ls41MrNwgfv4aEkTFpMMEDao-FKdjtDC1XS-j55ZkNWsK7yzK27zT53HPowAxzqDmGJ3CN3d7oA-Puhyo2yCP1kmt_011lBFlsUVizgosmLkcEu6e7ZhFYhhZINwizz3uYBxZMBjfQlsOedmv8JB_c_vPgsg8sxE=w807-h531-no)
   
   ![Alt text](https://lh3.googleusercontent.com/PSMnqgMZiC4xhggfgo_yFfyvG6UrR_3qcf7BVpF4LBjbju0KRrgGyiLd-q_jSrP6E_ImRoHIpbzGqmpCDHIi62WgbqHu-4hQjFkv328j81iL19dFwMgqIX9ECgAmnruVRJmy7Id3FmZ6ww2uWUjIZjCC5qEXOMVUnM9r_HRfeUR_tQrZQIlbo9pIs1fmlXckeYi0OlELJGHON56tIkT4S6SjLMfUO43QI48vy4ZTffUPGiE2b-6_Jcm5QgAcnOJhQmfosUn9UVn2lU7okoNJDcs4MSR0V7JTRUvLi8-Puq8wpFbeUNgxZJCMAP_VAf9fTCOI8DU_4O-dcxumo6VBpmpJPLltbSrHj1D8C9-Ek2V7-vZt9L-iWn1WLat6Br1ZOYVaiOeC53d3c9rYskOlIEwNS8vpHCJZHCDTRn79hl4mRvNGToUeE0FF3XFnhrQ9gaMYJteAov4OJo-V7y-Kad53dyZc-_4NtsX-Q_v4zBHW_HeGdJJ8KM9gbucb9Wf-K364SNxTQCSswDQGIx2ChzcvBi_Gq5sLf3ozEngYXyZ-wudBKT0KKbrBfD-kojPliIoaFmdbFu-UM8juH2Z92HgXeVdfD5xY-LmoAVs=w807-h531-no)
   
   
   
   
   
   
   
   
   
   

