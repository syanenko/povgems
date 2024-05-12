#
# Preview rendring
#
rm -fr ./out && mkdir ./out
povray +W640 +H480 +A0.1 +AM2 +R3 +FN -iGemsFactory.pov -o./out/gem
