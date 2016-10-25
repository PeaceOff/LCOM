#ifndef _BMP_H
#define _BMP_H

typedef struct {
    unsigned short type; // specifies the file type
    unsigned int size; // specifies the size in bytes of the bitmap file
    unsigned short reserved; // reserved; must be 0
    unsigned short reserved2;
    unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} __attribute__((packed)) BitmapFileHeader;

typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bits; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} __attribute__((packed)) BitmapInfoHeader;


unsigned char *LoadBitmapFile(char *filename, BitmapInfoHeader *bitmapInfoHeader);

#endif
