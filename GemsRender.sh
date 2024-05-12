#
# Spectral rendering
#
rm -fr ./out && mkdir ./out

# Rendering 36 b/w '*.exr' images
povray +W320 +H240 +A0.1 +AM2 +R3 +FE +KI1 +KF36 +KFI38 +KFF73 -iGemsFactory.pov -o./out/gem

# Producing final colored '*.png' image  
povray +W320 +H240 +FN -iGemsComposer.pov -o./out/gem.png


