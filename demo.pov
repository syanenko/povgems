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
#include "gems/inc/pc01024.inc"
// #include "gems/inc/pc01035.inc"
// #include "gems/inc/Bugbarion.inc"
// #include "gems/inc/Asashi.inc"
// #include "gems/inc/Maya_drop.inc"

// #include "gems/inc/Bugbarionegg.inc"
// #include "gems/inc/pc15011.inc"
// #include "gems/inc/pc08049.inc"
// #include "gems/inc/Trilled.inc"

// 
// Material
//
#macro Mat()
M_Emerald (0.15)
// M_Sapphire (10.85)
// M_Amethyst (0.18)
// M_Diamond_NaturalYellow(0.35)
// M_Topaz(0.05)
// M_Ruby (0.25)
#end

// 
// Transformations
//
#macro Trans()
  scale 1      
  rotate <-90, 22, 0>
  translate <50.1, 81.35, 175>
#end

//
// Lighting
//
#declare LightFadePower = 1;
#declare LightFadeDistance = 27;
#declare Sunlight    = 1.2;
#declare AreaLight   = 1;
#declare MaxTrace    = 7;

#declare Radio       = 0;
// #declare Photons     = 10000000; // Enable for best quality

//
// Environment
//
#declare UseRoom  = 1;
#declare UseSky   = 0;
#declare UseTable = 0;

#declare RoomDesign  = 6; // of 6
#declare SkyEmission = 1;

//
// Camera
//
#declare Cam_Pos     = <51, 84.0, 165>;
#declare Cam_Look    = <50.08, 81.2, 175.3>; 
#declare Cam_Angle   = 13;
//================================================================================

//================================================================================
// Sky, room ...
//================================================================================
#include "world.inc"

#if (UseRoom)
  object { Room }
#end 

#if (UseSky)
  object { Sky }
#end 

#if (UseTable)
  object { Table
    scale <0.6, 1, 0.6>
    rotate y*25
    translate <60,0,175>
  }
#end 

//================================================================================
// Light
//================================================================================
#declare Lamp = union {
  light_source {0, SpectralEmission(E_Blackbody(5500)) * 60
  fade_power LightFadePower * 1.8
  fade_distance LightFadeDistance

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
}                                                            
    
//object {Lamp translate Cam_Pos * 0.8}
//object {Lamp translate <0, 0, 100>}

object {Lamp translate <-100, 0, 0>}
object {Lamp translate <10, 0, 0>}

object {Lamp translate <0, -100, 0>}
object {Lamp translate <0,  150, 0>}

object {Lamp translate <0, 0, -100>} // Vert
object {Lamp translate <0, 0, 15>}

//================================================================================
// Gem
//================================================================================
object {
  Geom
  Mat()  
  Trans()
  photons {target refraction on reflection on}
}
