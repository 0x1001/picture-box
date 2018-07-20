# Picture-Box

**Toy for small kids that shows new picture every time button is pressed**

Pictures are stored on SD card, each time button is pressed new picture is being loaded to E-Paper screen.
Picture stays on the screen even when power supply is turned off.
Pictures are not included in this repository. You can download any pictures you wish from the Internet.
Python converter script (convert.py) converts any 4:3 image to *.bin file that represents E-Paper screen SRAM contents.
All *.bin files should be stored directly on SD card without any folder.
Each image should be named number.bin. Example: 1.bin, 2.bin, 3.bin, ..., 10.bin, 110.bin, 1001.bin.

## Components
- Display: Waveshare E-Paper Display 4.2 inc [Wiki page](https://www.waveshare.com/wiki/4.2inch_e-Paper_Module)
- SD card
- Arduino Pro Mini: [Link](https://store.arduino.cc/arduino-pro-mini)
- SD card reader that can be connected to Arduino
- Plastic case with transparent top
- Three batteries AA or AAA and battery holder.

## Image converter
This python script requires Python 3.x [Link](https://www.python.org/)
```sh
$ install.bat
$ start.bat
$ python convert.py --help
usage: convert.py [-h] --path PATH

Converts any 4:3 image to Waveshare 4.2inc screen SRAM format.

optional arguments:
  -h, --help            show this help message and exit
  --path PATH, -p PATH  path to image file or to folder with images
```

## Schematic
![Picture Box schematic](/docs/schematic_bb.png?raw=true "Picture Box schematic")

## Photos
![Picture Box Photos](/docs/photo1.jpg?raw=true "Picture Box Photos")
![Picture Box Photos](/docs/photo2.jpg?raw=true "Picture Box Photos")
