# bmpprinter
Print monochrome bmp to unicode text console.

It uses the pseudo-graphics from Symbols for Legacy Computing Unicode block, not ascii art. Make sure your font supports that code block.

Could be used to print QR code on text console that will actually be scannable by a QR scanner.

# background

The [Symbols for Legacy Computing](https://www.unicode.org/charts/nameslist/c_1FB00.html) <sup>[pdf](https://www.unicode.org/charts/PDF/U1FB00.pdf)</sup> have poor support in commodity fonts. There is a [font on github](https://github.com/dokutan/legacy_computing-font) implementing the code block.
Moreover, the pseudographic characters used in this project are 'sextants' and the unicode org has left out 4 of the 64 possible sextants that are already implemented in another code block (the [Block Elements](https://www.unicode.org/charts/nameslist/c_2580.html) - which are much more widely implemented) from the Legacy Computing code block. The result is that the pseudo-graphic composed of sextants will most likely be composed from glyphs from 2 different fonts with likely incompatible metrics and the corresponding alignment problems - if your [terminal emulator implements font fallback](https://github.com/microsoft/terminal/issues/13215) in the first place.
It seems that recent terminal emulators using [libvte](https://gitlab.gnome.org/GNOME/vte) implement these pseudo graphics in the application instead of relying on fonts to supply the glyphs. This is an effective way to avoid such alignment problems, because I didn't have the slightest problem initially until trying the application on older systems.
See also the [discussion](https://github.com/dokutan/legacy_computing-font/issues/1) on the above mentioned font project on github.
