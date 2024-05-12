# POV-Ray gems rendering extension
 [SpectralRenderer](https://www.lilysoft.org/CGI/SR/Spectral%20Render.htm) extension to [POV-Ray](https://www.povray.org/), written by Ive of LILYsoft has few excelent examples of gems rendering and set of mineral materials.
 This repository contains few additions, improving gems rendering techniques and gems geomety in _DXF_ and _SDL_ _mesh_ formats.

I am also included in this repository [CousinRicky’s Modifications to SpectralRender](https://github.com/CousinRicky/POV-SpectralRender-mods).

## _Preparing data_

Few gems designs are present in _gems_ directory.
In addition, great source of gems geometry can be found here:
http://www.facetdiagrams.org/database

To convert _ASC_ format from this database to DXF [GemCad](https://www.gemcad.com) (which is free now) or more modern tool - [GemcutStudio](https://gemcutstudio.com) can be used. Last also has _OBJ_ export function.<br>

Then, under Windows for _DXF_ / _OBJ_ to _SDL_ convertion you can use [PoseRay](https://sites.google.com/site/poseray/home-1). 
If you prefere Unix, use [dxf2pov](https://github.com/syanenko/dxf2pov), which also can be compiled under Windows.

## _Rendering_

Edit _'Control center'_ section in _GemsFactory.pov_ file: uncomment design you want to render and appropriate material.<br>
Play with other setting in this section to adjust lighting, environment, etc.

To render preview image under Unix run _GemsPreview.sh_ or
```
povray +W640 +H480 +A0.1 +AM2 +R3 +FN -iGemsFactory.pov -o./out/gem
```
and rendered image will wait for you in _out_ directory. If you under Windows, use keys from this command line in editor settigs. 

For final spectral rendering under Unix run _GemsRender.sh_ or
```
povray +W320 +H240 +A0.1 +AM2 +R3 +FE +KI1 +KF36 +KFI38 +KFF73 -iGemsFactory.pov -o./out/gem
povray +W320 +H240 +FN -iGemsComposer.pov -o./out/gem.png
```

And, again, for Windows, use keys from this command lines in editor settigs.

Happy rendering !<br>
![image](https://github.com/syanenko/povgems/assets/6688301/04b72d04-9603-446f-9aca-3e90d2a1d554)

<em>Contact: [LinkedIn](https://www.linkedin.com/in/sergey-yanenko-57b21a96/)<em>
