//================================================================================
// POV-Ray spectral gems rendering example with geometry extention
//
// Command line for spectral calculation ('GemsFactory.pov' - produces 36 b/w '*.exr' images):
// +W640 +H480 +A0.05 +AM2 +R4 +FE +KI1 +KF36 +KFI38 +KFF73
//
// Command line for spectral composer ('GemsCompose.pov' produces final '*.png' image):
// +W640 +H480 +FN
//
// - Gems (gems/*.inc) by Sergey N. Yanenko (Yesbird), May 2024
// - Ive, September 2012
//================================================================================

#declare FName = "./out/gem"
#include "SpectralComposer.inc"
// #include "SpectralComposer-gm2.inc"
