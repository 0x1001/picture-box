import argparse
from PIL import Image
import os

WIDTH = 400
HEIGHT = 300


def convert(path):
    im = Image.open(path)
    im = im.convert('1')
    pixels = im.load()

    width, height = im.size

    if width != WIDTH or height != HEIGHT:
        raise Exception("Width or height of the picture is not supported: {0}x{1}".format(width, height))

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


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Converts 400x300 images for Waveshare 4.2inc screen.')
    parser.add_argument('--path', "-p", required=True, type=str, dest="path", help='path to image file')

    args = parser.parse_args()
    convert(args.path)