import argparse
from PIL import Image
import os

WIDTH = 400
HEIGHT = 300


def convert_dir(path):
    for root, dirs, files in os.walk(path):
        for file_name in files:
            if file_name.lower().endswith(".png") or file_name.lower().endswith(".jpg") or file_name.lower().endswith(".jpeg"):
                convert(os.path.join(root, file_name))


def convert(path):
    print("Processing: " + path)
    im = Image.open(path)
    im = _resize(im)
    im = im.convert('1')
    pixels = im.load()

    im.save(os.path.join(os.path.dirname(path), "_" + os.path.splitext(os.path.basename(path))[0] + os.path.splitext(os.path.basename(path))[1]))

    binary_image = bytearray(int(WIDTH*HEIGHT/8))

    for i in range(WIDTH):
        for j in range(HEIGHT):
            if pixels[i, j] == 255:
                binary_image[int((i + j * WIDTH) / 8)] |= 0x80 >> (i % 8)
                #image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
            else:
                binary_image[int((i + j * WIDTH) / 8)] &= ~(0x80 >> (i % 8))
                #image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));

    with open(os.path.splitext(path)[0] + ".bin", "wb") as fp:
        fp.write(binary_image)


def _resize(im):
    width, height = im.size

    if 1.3 < width/height < 1.4:
        raise Exception("Width or height of the picture is not supported: {0}x{1}".format(width, height))

    return im.resize((400,300))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Converts any 4:3 image to Waveshare 4.2inc screen SRAM format.')
    parser.add_argument('--path', "-p",
                        required=True,
                        type=str,
                        dest="path",
                        help='path to image file or to folder with images')

    args = parser.parse_args()
    if os.path.isfile(args.path):
        convert(args.path)
    elif os.path.isdir(args.path):
        convert_dir(args.path)
