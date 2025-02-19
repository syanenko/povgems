# POV-Ray gems rendering extension

![Gems](https://github.com/user-attachments/assets/f4c5c12f-f026-4cd2-9604-db07991bd9cd)

[SpectralRenderer](https://www.lilysoft.org/CGI/SR/Spectral%20Render.htm) extension to [POV-Ray](https://www.povray.org/), written by Ive of LILYsoft has few excelent examples of gems rendering and set of mineral materials.
This repository contains few additions, improving gems rendering techniques and gems geomety in _DXF_ and _SDL_ _mesh_ formats.

I am also included modifications from this repository [CousinRicky’s Modifications to SpectralRender](https://github.com/CousinRicky/POV-SpectralRender-mods).

## _Preparing data_

Great source of gem designs is [Facetdiagrams](http://www.facetdiagrams.org).<br><br> 
To convert _ASC_ format from this database to DXF [GemCad](https://www.gemcad.com) or more modern tool - [GemcutStudio](https://gemcutstudio.com) can be used.<br>
Last also has _OBJ_ export function. Then, under Windows for _DXF_ / _OBJ_ to _SDL_ convertion you can use [PoseRay](https://sites.google.com/site/poseray/home-1).<br> 
If you prefere Unix, use [dxf2pov](https://github.com/syanenko/dxf2pov), which also can be compiled under Windows.

Directory _gems/inc_ contains 2265 gem designs available for free at [Facetdiagrams](http://www.facetdiagrams.org) and already converted to POV-Ray meshes. You can also browse and download them with [GemView](https://gemview.yesbird.online/) application.

## _Rendering_

Edit _'Control center'_ section in _GemsFactory.pov_ file: uncomment design you want to render and appropriate material.<br>
Play with other setting in this section to adjust lighting, environment, etc.

To render preview image under Unix run _GemsPreview.sh_ or
```
povray +W640 +H480 +A0.1 +AM2 +R3 +FN -iGemsFactory.pov -o./out/gem
```
and rendered image will wait for you in _out_ directory. If you under Windows, use keys from this command line in editor's menu: 'Render->Edit settings/Render->Command line options'.

For final spectral rendering under Unix run _GemsRender.sh_ or
```
povray +W320 +H240 +A0.1 +AM2 +R3 +FE +KI1 +KF36 +KFI38 +KFF73 -iGemsFactory.pov -o./out/gem
povray +W320 +H240 +FN -iGemsComposer.pov -o./out/gem.png
```

And, again, for Windows, use keys from this command lines in editor's menu:<br>
'Render->Edit settings/Render->Command line options'.

Happy rendering !<br><br>
![Asashi_amethyst_320x240](https://github.com/user-attachments/assets/b0ce13aa-2cb0-4186-937c-3c54bb5d879a)

<em>Contact: [LinkedIn](https://www.linkedin.com/in/sergey-yanenko-57b21a96/)<em>
