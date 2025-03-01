//================================================================================
// POV-Ray spectral rendering composer V0.2
//
// Ive, September 2013
//
//================================================================================
// Modified version RC3-0.22-3 by Richard Callwood III:
// 2013-Nov-26  Converted to parameter-driven .inc file.
// 2013-Dec-02  A separate version of this file is created with gamut mapping (CIE.inc's method 2).
// 2023-Feb-14  Converted npow() to NPow().
// 2023-Mar-21  White points are accepted down to 2000 K.
// 2024-Dec-22  Luminance-based gamut mapping is implemented.
//              Method 2 gamut mapping is refactored to increase the speed (almost 3X).
//              The gamut mapping implementations are merged into this file.

#version 3.7;

//================================================================================
//
// User settings:
// **************
//
// You'll have to set *FName* and then you can play around with the various user
// settings until the image looks fine to you. No need for antialiasing until you
// use "extreme" user settings.
//
// And make sure that +W +H does match the image dimension of the previously
// calculated spectral file set and also make sure that this file does not run as
// animation - happened to me all the time ;)
//
//================================================================================


// FName: change this to match the name of your set of 36 spectral files
//#declare FName = "";
#ifndef (FName)
  #error "String FName must be set to the name of your 36 .exr files (without the frame number and \".exr\" extension)."
#end


// Exposure: range: 0 .. 2.0+ / default: 1.0
// Change this to adjust for the global lighting condition.
// Set it to 1.0 to keep original lighting data (e.g. rendering the
// GretagMcbeth ColorChecker Chart).
#ifndef (Exposure) #declare Exposure = 1.0; #end


// Luminance: range: 0 .. 2.0+ / default: 1.0
// linear luminance channel within the CIE xyz color space
// similar to Exposure but useful when Exponent != 1.0
#ifndef (Luminance) #declare Luminance = 1.0; #end


// Exponent: range: 1.0 .. 2.0+ / default: 1.0
// Raise this above 1.0 to adjust for the non-linearity of human vision, photo-paper etc.
// Set it to 1.0 to keep original lighting data (e.g. rendering the
// GretagMacbeth ColorChecker Chart).
// But try it with e.g. 1.2 to give your image more depth.
#ifndef (Exponent) #declare Exponent = 1.0; #end


// Saturation: range: -1.0 .. 2.0+ / default: 1.0
// Saturation = 0.0 will create an elegant monochrome image ;)
// 0 < Saturation < 1 will desaturate the image
// Saturation = 1 is the default normal saturation
// Saturation > 1 will over-saturate the image
// Saturation < 0 will create a photo negative
#ifndef (Saturation) #declare Saturation = 1.0; #end


// Whitepoint: range: 4000 .. 9300 / default: 6504 (D65)
// Change the white balance to adapt to the global lighting condition.
#ifndef (Whitepoint) #declare Whitepoint = 6504; #end


// HDR_Max: range: 1.0 .. 100.0+ / default: 25
// Change this to cover the full dynamic range when needed, otherwise
// values get clipped at HDR_max.
#ifndef (HDR_max) #declare HDR_max = 25; #end


// HDR_Min: range: -1.0 .. 0.0 / default: -1.0
// Cover out-of-gamut values.
#ifndef (HDR_min) #declare HDR_min = -1.0; #end


// AdobeRGB: on/off  default: off (sRGB)
// Change this only when you know what you are doing ;)
#ifndef (Use_AdobeRGB) #declare Use_AdobeRGB = off; #end


// GamutMap: 1 = clipping, 2 = CIE.inc's method 2, 4 = luminance-based
// Note: As convenient as it would be to reuse the name GamutMapFunction from CIE.inc,
// I am avoiding this because a comment in that file warns against direct access. --RC3
#ifndef (GamutMap) #declare GamutMap = 1; #end



//================================================================================
// Internal stuff, no need to touch anything below.
//================================================================================

global_settings { assumed_gamma 1 }

#include "CIE.inc"

#debug "\n*****  Spectral Composer  *****\n"


//================================================================================
// sanity checks

#if (Exposure <= 0.0)
  #declare Exposure = 1.0;
  #warning "(Spectral Composer) Exposure should be greater than 0.0, set to default 1.0"
#end

#if (Luminance <= 0.0)
  #declare Luminance = 1.0;
  #warning "(Spectral Composer) Luminance should be greater than 0.0, set to default 1.0"
#end

#if (Exponent <= 0.0)
  #declare Exponent = 1.0;
  #warning "(Spectral Composer) Exponent should be greater than 0.0, set to default 1.0"
#end

#if (HDR_min > 0.0)
  #declare HDR_min = -1.0;
  #warning "(Spectral Composer) HDR_min should  be lower or equal than 0.0, set to 0.0"
#end

#if (HDR_max < 1.0)
  #declare HDR_max = 1.0;
  #warning "(Spectral Composer) HDR_max should be greater or equal than 1.0, set to 1.0"
#end

#switch (GamutMap)
  #case (1)
    #local s_GamutMap = "clipped"
    #break
  #case (2)
    #local s_GamutMap = "projected toward white point"
    #break
  #case (3)
    #local s_GamutMap = "unimplemented"
    #warning "Gamut mapping method 3 is unimplemented."
    #break
  #case (4)
    #local s_GamutMap =  "luminance preserved"
    #break
  #else
    #local s_GamutMap =  "unrecognized"
    #warning concat ("Gamut mapping method ", str (GamutMap, 0, -1), " is unrecognized.")
#end


//================================================================================
// same output

#if (Use_AdobeRGB | (Whitepoint != 6504))

  #local WP = min(max(2000,Whitepoint),20000);

 #if (Use_AdobeRGB & WP < 4000)
  CIE_ColorSystemWhitepoint(Adobe_ColSys, Blackbody2Whitepoint(WP))
  #debug "using AdobeRGB primaries with whitepoint "
 #elseif (Use_AdobeRGB)
  CIE_ColorSystemWhitepoint(Adobe_ColSys, Daylight2Whitepoint(WP))
  #debug "using AdobeRGB primaries with whitepoint D "
 #elseif (WP < 4000)
  CIE_ColorSystemWhitepoint(sRGB_ColSys, Blackbody2Whitepoint(WP))
  #debug "using sRGB primaries with whitepoint "
 #else
  CIE_ColorSystemWhitepoint(sRGB_ColSys, Daylight2Whitepoint(WP))
  #debug "using sRGB primaries with whitepoint D "
 #end

 #debug str(WP,4,0) #debug "K"

#else
 #debug "using sRGB primaries with D65"
#end

#debug "\n"
#debug "Exposure:    " #debug str(Exposure,1,2)    #debug "\n"
#debug "Luminance:   " #debug str(Luminance,1,2)   #debug "\n"
#debug "Exponent:    " #debug str(Exponent,1,2)    #debug "\n"
#debug "Saturation:  " #debug str(Saturation,1,2)  #debug "\n"
#debug "Whitepoint:  " #debug str(Whitepoint,4,0)  #debug "K\n"
#debug "HDR minimum: " #debug str(HDR_min, 1,2)    #debug "\n"
#debug "HDR maximum: " #debug str(HDR_max, 1,2)    #debug "\n"
#debug concat ("GamutMap:    ", str(GamutMap, 1,2), " (", s_GamutMap, ")\n")

#debug "*******************************\n\n"


//================================================================================
// CMF

// array to hold the 36 pigment functions representing the wavelengths from
// 380nm to 730nm
#declare F = array[36];

// CIE color match 380nm to 730nm
#declare CMF = array[36][3];

// the original spline from CIE.inc is only converted to an array to make
// the CMF functions below faster.
#for (I, 0, 35)
  #local W = 380 + I*10;

  #declare F[I] = function { pigment {image_map {exr concat(FName, str(I+38,0,0))} translate -0.5} }

  #declare CMF[I][0] = CMF_xyz(W).x * 0.1;
  #declare CMF[I][1] = CMF_xyz(W).y * 0.1;
  #declare CMF[I][2] = CMF_xyz(W).z * 0.1;
#end


// color matching functions
// CMF for the CIE 1931 2� observer

#declare CMF_x = function {
  CMF[0][0]  * F[0](x,y,z).red  + // 380
  CMF[1][0]  * F[1](x,y,z).red  + // 390
  CMF[2][0]  * F[2](x,y,z).red  + // 400
  CMF[3][0]  * F[3](x,y,z).red  + // 410
  CMF[4][0]  * F[4](x,y,z).red  + // 420
  CMF[5][0]  * F[5](x,y,z).red  + // 430
  CMF[6][0]  * F[6](x,y,z).red  + // 440
  CMF[7][0]  * F[7](x,y,z).red  + // 450
  CMF[8][0]  * F[8](x,y,z).red  + // 460
  CMF[9][0]  * F[9](x,y,z).red  + // 470
  CMF[10][0] * F[10](x,y,z).red + // 480
  CMF[11][0] * F[11](x,y,z).red + // 490
  CMF[12][0] * F[12](x,y,z).red + // 500
  CMF[13][0] * F[13](x,y,z).red + // 510
  CMF[14][0] * F[14](x,y,z).red + // 520
  CMF[15][0] * F[15](x,y,z).red + // 530
  CMF[16][0] * F[16](x,y,z).red + // 540
  CMF[17][0] * F[17](x,y,z).red + // 550
  CMF[18][0] * F[18](x,y,z).red + // 560
  CMF[19][0] * F[19](x,y,z).red + // 570
  CMF[20][0] * F[20](x,y,z).red + // 580
  CMF[21][0] * F[21](x,y,z).red + // 590
  CMF[22][0] * F[22](x,y,z).red + // 600
  CMF[23][0] * F[23](x,y,z).red + // 610
  CMF[24][0] * F[24](x,y,z).red + // 620
  CMF[25][0] * F[25](x,y,z).red + // 630
  CMF[26][0] * F[26](x,y,z).red + // 640
  CMF[27][0] * F[27](x,y,z).red + // 650
  CMF[28][0] * F[28](x,y,z).red + // 660
  CMF[29][0] * F[29](x,y,z).red + // 670
  CMF[30][0] * F[30](x,y,z).red + // 680
  CMF[31][0] * F[31](x,y,z).red + // 690
  CMF[32][0] * F[32](x,y,z).red + // 700
  CMF[33][0] * F[33](x,y,z).red + // 710
  CMF[34][0] * F[34](x,y,z).red + // 720
  CMF[35][0] * F[35](x,y,z).red   // 730
}

#declare CMF_y = function {
  CMF[0][1]  * F[0](x,y,z).red   + // 380
  CMF[1][1]  * F[1](x,y,z).red   + // 390
  CMF[2][1]  * F[2](x,y,z).red   + // 400
  CMF[3][1]  * F[3](x,y,z).red   + // 410
  CMF[4][1]  * F[4](x,y,z).red   + // 420
  CMF[5][1]  * F[5](x,y,z).red   + // 430
  CMF[6][1]  * F[6](x,y,z).red   + // 440
  CMF[7][1]  * F[7](x,y,z).red   + // 450
  CMF[8][1]  * F[8](x,y,z).red   + // 460
  CMF[9][1]  * F[9](x,y,z).red   + // 470
  CMF[10][1] * F[10](x,y,z).red  + // 480
  CMF[11][1] * F[11](x,y,z).red  + // 490
  CMF[12][1] * F[12](x,y,z).red  + // 500
  CMF[13][1] * F[13](x,y,z).red  + // 510
  CMF[14][1] * F[14](x,y,z).red  + // 520
  CMF[15][1] * F[15](x,y,z).red  + // 530
  CMF[16][1] * F[16](x,y,z).red  + // 540
  CMF[17][1] * F[17](x,y,z).red  + // 550
  CMF[18][1] * F[18](x,y,z).red  + // 560
  CMF[19][1] * F[19](x,y,z).red  + // 570
  CMF[20][1] * F[20](x,y,z).red  + // 580
  CMF[21][1] * F[21](x,y,z).red  + // 590
  CMF[22][1] * F[22](x,y,z).red  + // 600
  CMF[23][1] * F[23](x,y,z).red  + // 610
  CMF[24][1] * F[24](x,y,z).red  + // 620
  CMF[25][1] * F[25](x,y,z).red  + // 630
  CMF[26][1] * F[26](x,y,z).red  + // 640
  CMF[27][1] * F[27](x,y,z).red  + // 650
  CMF[28][1] * F[28](x,y,z).red  + // 660
  CMF[29][1] * F[29](x,y,z).red  + // 670
  CMF[30][1] * F[30](x,y,z).red  + // 680
  CMF[31][1] * F[31](x,y,z).red  + // 690
  CMF[32][1] * F[32](x,y,z).red  + // 700
  CMF[33][1] * F[33](x,y,z).red  + // 710
  CMF[34][1] * F[34](x,y,z).red  + // 720
  CMF[35][1] * F[35](x,y,z).red    // 730
}

#declare CMF_z = function {
  CMF[0][2]  * F[0](x,y,z).red   + // 380
  CMF[1][2]  * F[1](x,y,z).red   + // 390
  CMF[2][2]  * F[2](x,y,z).red   + // 400
  CMF[3][2]  * F[3](x,y,z).red   + // 410
  CMF[4][2]  * F[4](x,y,z).red   + // 420
  CMF[5][2]  * F[5](x,y,z).red   + // 430
  CMF[6][2]  * F[6](x,y,z).red   + // 440
  CMF[7][2]  * F[7](x,y,z).red   + // 450
  CMF[8][2]  * F[8](x,y,z).red   + // 460
  CMF[9][2]  * F[9](x,y,z).red   + // 470
  CMF[10][2] * F[10](x,y,z).red  + // 480
  CMF[11][2] * F[11](x,y,z).red  + // 490
  CMF[12][2] * F[12](x,y,z).red  + // 500
  CMF[13][2] * F[13](x,y,z).red  + // 510
  CMF[14][2] * F[14](x,y,z).red  + // 520
  CMF[15][2] * F[15](x,y,z).red  + // 530
  CMF[16][2] * F[16](x,y,z).red  + // 540
  CMF[17][2] * F[17](x,y,z).red  + // 550
  CMF[18][2] * F[18](x,y,z).red  + // 560
  CMF[19][2] * F[19](x,y,z).red  + // 570
  CMF[20][2] * F[20](x,y,z).red  + // 580
  CMF[21][2] * F[21](x,y,z).red  + // 590
  CMF[22][2] * F[22](x,y,z).red  + // 600
  CMF[23][2] * F[23](x,y,z).red  + // 610
  CMF[24][2] * F[24](x,y,z).red  + // 620
  CMF[25][2] * F[25](x,y,z).red  + // 630
  CMF[26][2] * F[26](x,y,z).red    // 640
                                   // 650 and above is 0.0
}

// as the input images are grayscale the used color channel does not matter.
// "red" was just shorter to type than "green" or "blue"


//================================================================================
// pigment calculation

#local H = HDR_max * 3;  // * 3 to adjust for the average map
#local L = HDR_min * 3;
#local E = Exposure / (HDR_max-HDR_min);
#local M = -HDR_min / (HDR_max-HDR_min);
#local Y = Luminance;

#local RL = 0.3086;
#local GL = 0.6094;
#local BL = 0.0820;


#local ColorProcessMat = array[3][3] {
  {((1.0-Saturation)*RL+Saturation)*Y, (1.0-Saturation)*GL,               (1.0-Saturation)*BL},
  { (1.0-Saturation)*RL,              ((1.0-Saturation)*GL+Saturation)*Y, (1.0-Saturation)*BL},
  { (1.0-Saturation)*RL,               (1.0-Saturation)*GL,              ((1.0-Saturation)*BL+Saturation)*Y}
}


// csMatRGB has already been calculated by CIE.inc. Multiplication with the color
// processing matrix from above gives the final xyz -> rgb color conversion matrix.
#local CM = MultMat33(ColorProcessMat,csMatRGB)


// The exponent function (for Exponent > 1.0) is composed of two functions (linear and a power function)
// as otherwise out-of-gamut values would be shifted in an unwanted non-linear way making them de facto
// useless.
// For an Exponent of 2.4 this is close to the sRGB transfer function but without its notorious discontinuity
// at the junction point. This is important as we are dealing with floating point precision here and the
// exported image might very well support more than 8bit/channel!

#if (Exponent > 1)

  #local Limit = 0.055 * ((Exponent-0.8) / 1.6);

  // junction point
  #local K = Limit / (Exponent - 1);

  // calculate the tangent
  #local Delta = (pow(Limit, Exponent-1) * pow(Exponent, Exponent)) /
                 (pow(1+Limit, Exponent) * pow(Exponent-1, Exponent-1));


  // transfer function
  #local NPow = function(x) { select (x-K, x*Delta, pow((x+Limit)/(1+Limit), Exponent) ) }


#elseif (Exponent < 1)

  #local NPow = function(x) { pow(x, Exponent) }

#else

  #local NPow = function(x) {x}

#end


// color channels at full dynamic range and saturation
#local fn_Red = function
{ ( NPow
    (   CM[0][0] * CMF_x(x,y,z)
      + CM[0][1] * CMF_y(x,y,z)
      + CM[0][2] * CMF_z(x,y,z)
    ) * E + M
  ) * (HDR_max - HDR_min) + HDR_min
}

#local fn_Green = function
{ ( NPow
    (   CM[1][0] * CMF_x(x,y,z)
      + CM[1][1] * CMF_y(x,y,z)
      + CM[1][2] * CMF_z(x,y,z)
    ) * E + M
  ) * (HDR_max - HDR_min) + HDR_min
}

#local fn_Blue = function
{ ( NPow
    (   CM[2][0] * CMF_x(x,y,z)
      + CM[2][1] * CMF_y(x,y,z)
      + CM[2][2] * CMF_z(x,y,z)
    ) * E + M
  ) * (HDR_max - HDR_min) + HDR_min
}


// gamut mapping method 2 (see MapGamut() in CIE.inc)
#local WtPt = xyz2RGB (ColSys2xyz (3));
#local WtR = WtPt.red;
#local WtG = WtPt.green;
#local WtB = WtPt.blue;

#local fn_P = function
{ select
  ( 0.5 - (x <= y & x <= z),
    WtR / (WtR - x),
    select
    ( 0.5 - (y <= x & y <= z),
      WtG / (WtG - y),
      WtB / (WtB - z)
    )
  )
}

#local fn_GamutMap2 = function (x, y, z, P0_W)
{ select (min(x,y,z), min (max (P0_W + fn_P(x,y,z) * (x - P0_W), 0), 1), min (x, 1))
}

// luminance-based gamut mapping
// Theoretically, max() shouldn't be needed here, but FP fuzz leads to things like -0.00001.
#local fn_GamutMapY = function (x, y, z, P0_Y)
{ min (1, max (0, P0_Y + P0_Y / (P0_Y - min (x, y, z, 0)) * (x - P0_Y) ))
}



// our canvas
plane {-z, 0

  pigment {
    average

    pigment_map {

      #switch (GamutMap)

        #case (2) // CIE.inc method 2
          [ function { fn_GamutMap2 (fn_Red(x,y,z), fn_Green(x,y,z), fn_Blue(x,y,z), WtR) }
            color_map { [0 rgb 0] [1 red 3] }
          ]
          [ function { fn_GamutMap2 (fn_Green(x,y,z), fn_Red(x,y,z), fn_Blue(x,y,z), WtG) }
            color_map { [0 rgb 0] [1 green 3] }
          ]
          [ function { fn_GamutMap2 (fn_Blue(x,y,z), fn_Red(x,y,z), fn_Green(x,y,z), WtB) }
            color_map { [0 rgb 0] [1 blue 3] }
          ]
          #break

        #case (4) // luminance-based
          [ function { fn_GamutMapY (fn_Red(x,y,z), fn_Green(x,y,z), fn_Blue(x,y,z), CMF_y(x,y,z)) }
            color_map { [0 rgb 0] [1 red 3] }
          ]
          [ function { fn_GamutMapY (fn_Green(x,y,z), fn_Red(x,y,z), fn_Blue(x,y,z), CMF_y(x,y,z)) }
            color_map { [0 rgb 0] [1 green 3] }
          ]
          [ function { fn_GamutMapY (fn_Blue(x,y,z), fn_Red(x,y,z), fn_Green(x,y,z), CMF_y(x,y,z)) }
            color_map { [0 rgb 0] [1 blue 3] }
          ]
          #break

        #case (1) // original "brain dead clipping"
        #else     // also the default for invalid parameter values
          [function { min(max( NPow(CM[0][0]*CMF_x(x,y,z) + CM[0][1]*CMF_y(x,y,z) + CM[0][2]*CMF_z(x,y,z)) * E + M, 0), 1)}
            color_map { [0 rgb <L,0,0>] [1 rgb <H,0,0>] }
          ]

          [function { min(max( NPow(CM[1][0]*CMF_x(x,y,z) + CM[1][1]*CMF_y(x,y,z) + CM[1][2]*CMF_z(x,y,z)) * E + M, 0), 1)}
            color_map { [0 rgb <0,L,0>] [1 rgb <0,H,0>] }
          ]

          [function { min(max( NPow(CM[2][0]*CMF_x(x,y,z) + CM[2][1]*CMF_y(x,y,z) + CM[2][2]*CMF_z(x,y,z)) * E + M, 0), 1)}
            color_map { [0 rgb <0,0,L>] [1 rgb <0,0,H>] }
          ]
          #break

      #end

    }

  }

  finish { ambient 0 diffuse 0 emission 1 }
}


//================================================================================
// orthographic camera

camera {
  orthographic
  location -z
  look_at  0
  right x
  up    y
}


//================================================================================
//
// TODO LIST: (in no particular order)
// **********
//
// A wavelength depended filter multiplied with the CMF for simulating a color
// filter in front of the camera lens could be useful.
//
// Implement a "knee" function to dampen values close and above 1.0 - similar to
// the implementation of the Cineon format for simulating the chemical behaviour
// of  Kodak (RIP) 35mm negative film. This is also recommended by ILM when
// mapping OpenEXR to LDR image formats.
//
// In general *some* gamut mapping function instead of just brain dead clipping.
//
// Make the spectral calculation almost 3 times faster by calculating 3 sequential
// wavelength at once by using the 3 rgb color channels.
//
//================================================================================
