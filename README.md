# POV-Ray gems rendering extension
 [SpectralRenderer](https://www.lilysoft.org/CGI/SR/Spectral%20Render.htm), extension to [POV-Ray](https://www.povray.org/), written by Ive of LILYsoft has few excelent examples of gems rendering and set of mineral materials.
 This reposotory contains few additions, improving gems rendering techniques and gems geomety in _DXF_ and _SDL_ _mesh_ formats.

## _Preparing data_

Great source of gems geometry can be found here:
http://www.facetdiagrams.org/database/

To convert _ASC_ format from this database to DXF [GemCad](https://www.gemcad.com) (which is free now) or more modern tool -
[GemcutStudio](https://gemcutstudio.com) can be used. Last also has _OBJ_ export function.

Then, under Windows for _DXF_ / _OBJ_ to SDL convertion you can use [PoseRay](https://sites.google.com/site/poseray/home-1) or
if you prefere Unix, use [dxf2pov](https://github.com/syanenko/dxf2pov), which also can be compiled under Windows.

## _Rendering_

Edit _Control center_ section in _GemsFactory.pov_ file: uncomment gem design you want to render and appropriate material.
Use 
```
#declare Photons = ...
```
for better quality.

To render preview image under Unix run _GemsPreview.sh_ or
```
povray +W640 +H480 +A0.1 +AM2 +R3 +FN -iGemsFactory.pov -o./out/gem
```
If you under Windows, use keys from this command line in editor settigs.

For final spectral rendering under Unix run _GemsRender.sh_
```
povray +W320 +H240 +A0.1 +AM2 +R3 +FE +KI1 +KF36 +KFI38 +KFF73 -iGemsFactory.pov -o./out/gem
povray +W320 +H240 +FN -iGemsComposer.pov -o./out/gem.png
```




