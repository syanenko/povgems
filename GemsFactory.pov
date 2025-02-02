//================================================================================
// POV-Ray spectral gems rendering example with geometry extention
//
// Command line for spectral calculation ('GemsFactory.pov' - produces 36 b/w '*.exr' images):
// +W640 +H480 +A0.05 +AM2 +R4 +FE +KI1 +KF36 +KFI38 +KFF73
//
// Command line for spectral composer ('Compose.pov' produces final '*.png' image):
// +W640 +H480 +FN
//
// - Gems (gems/*.inc) by Sergey N. Yanenko (Yesbird), May 2024
// - Ive, September 2012
//================================================================================
#version 3.7; 
#include "spectral.inc"

//
// Gem design - 2270 designs available
//
// #include "gems/inc/Asashi.inc"
// #include "gems/inc/Maya_drop.inc"
// #include "gems/inc/Bugbarion.inc"
// #include "gems/inc/Bugbarionegg.inc"
// #include "gems/inc/Trilled.inc"

// #include "gems/inc/pc01035.inc"
// #include "gems/inc/pc15011.inc"
// #include "gems/inc/pc01024.inc"
// #include "gems/inc/pc08049.inc"
#include "gems/inc/pc45149.inc"

// 
// Mineral
//
#macro Mineral()
  M_Diamond_NaturalYellow(0.35)
  // M_Diamond_LemonYellow(1.2)
  // M_Amethyst (0.5)
  // M_Ruby (0.6)
  // M_Emerald (0.4)
  // M_Sapphire (10.85)
  // M_Topaz(0.23)
#end

// 
// Transformations
//
#macro Transform()
  scale 1
  rotate -x * 90
  rotate -y * 90
  translate <50.1, 81.2, 175>
#end

//
// Lighting
//
#declare LampPower   = 60;
#declare Sunlight    = 0.4;
#declare AreaLight   = true;
#declare SkyEmission = 1;


#declare MaxTrace    = 60;
#declare Radio       = 1;
#declare Photons     = 5000000; // Enable for best quality

//
// Camera
//
#declare Cam_Pos     = < 51, 85.0, 165>;
#declare Cam_Look    = < 50.1, 81, 175>; 
#declare Cam_Angle   = 15;
  
//
// Environmet
//
#declare RoomDesign = 1; // 6 available
#include "world.inc"

object { Sky }        
object { Room scale 0.91}

object { Table  
  scale <0.6, 1, 0.6>
  rotate y*25
  translate <60, 0.2, 175>
}

//
// Light
//
#declare Lamp = union {
  light_source {0, SpectralEmission(E_Blackbody(5500)) * LampPower
  fade_power 2
  fade_distance 5

   #if (AreaLight)
    area_light z*5, y*5, 9,9 adaptive 1 circular orient
   #end
  }

  translate <50, 130, 220>
}  
object {Lamp}    

//
// Gem object
//
object {
  Gem
  Mineral()  
  Transform()
  photons {target refraction on reflection on}
}
