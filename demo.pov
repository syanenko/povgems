//================================================================================
// POV-Ray spectral gems rendering example with geometry extention
// Preview mode only
//
// - Gems (gems/*.inc) by Sergey N. Yanenko (Yesbird), May 2024
// - Ive, September 2012
//================================================================================
#version 3.7;
#include "spectral.inc"
//================================================================================
// control center
//================================================================================
//
// Select design
// 
// #include "gems/Asashi.inc"
// #include "gems/Maya_drop.inc"
// #include "gems/Bugbarion.inc"
// #include "gems/Bugbarionegg.inc"
// #include "gems/pc01035.inc"
// #include "gems/pc15011.inc"
#include "gems/inc/pc01024.inc"
// #include "gems/pc08049.inc"
// #include "gems/Trilled.inc"

// 
// Select material
//
#macro Mat()
M_Emerald (0.25)
// M_Sapphire (0.85)
// M_Amethyst (0.25)
// M_Diamond_NaturalYellow(0.45)
// M_Topaz(0.15)
// M_Ruby (0.45)
#end

// 
// Set transformations
//
#macro Trans()
  scale 1
  rotate -x * 90
  rotate y * 15
  translate <50.1, 81.2, 175>
#end

//
// Lighting / Environment
//
#declare MaxTrace    = 15;
#declare Radio       = 1;
// #declare Photons     = 10000000; // Enable for best quality
#declare Sunlight    = 3.5;
#declare SkyEmission = 1;
#declare RoomDesign  = 6; // 6 available

//
// Camera
//
#declare Cam_Pos     = < 51, 85.0, 165>;
#declare Cam_Look    = < 50.1, 81, 175.3>; 
#declare Cam_Angle   = 15;
//================================================================================

//================================================================================
// Sky, room ...
//================================================================================
#include "world.inc"
object { Room }
/*
object { Sky }
object { Table
  scale <0.6, 1, 0.6>
  rotate y*25
  translate <60,0,175>
}
*/
//================================================================================
// Light
//================================================================================
#declare Lamp = union {
  light_source {0, SpectralEmission(E_Blackbody(5500)) * 60
  fade_power 2
  fade_distance 15

   #if (AreaLight)
    area_light z*5, y*5, 9,9 adaptive 1 circular orient
   #end 

    photons {reflection on refraction on }
  }

  sphere { 0, 5
    pigment { SpectralEmission(E_Blackbody(5500)) }    
    finish {emission 3 ambient 0 diffuse 0}
    no_shadow
    no_radiosity
  }

  translate <90, 110, 220>  
}  
    
object {Lamp}    

//================================================================================
// Gem
//================================================================================
object {
  Geom
  Mat()  
  Trans()
  photons {target refraction on reflection on}
}
