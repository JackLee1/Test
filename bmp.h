#ifndef _BMP_H_
#define _BMP_H_

//#include "DataType.h"

#pragma pack(1)

/*

typedef struct tagBITMAPFILEHEADER {
        unsigned short    bfType;
        unsigned int   bfSize;
        unsigned short    bfReserved1;
        unsigned short    bfReserved2;
        unsigned int   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
        unsigned int      biSize;
        int       biWidth;
        int       biHeight;
        unsigned short       biPlanes;
        unsigned short       biBitCount;
        unsigned int      biCompression;
        unsigned int      biSizeImage;
        int       biXPelsPerMeter;
        int       biYPelsPerMeter;
        unsigned int      biClrUsed;
        unsigned int      biClrImportant;
} BITMAPINFOHEADER,*PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
        unsigned char    rgbBlue;
        unsigned char    rgbGreen;
        unsigned char    rgbRed;
        unsigned char    rgbReserved;
} RGBQUAD;
*/
typedef struct tagRGB
{
	int row;
	int col;
	unsigned char *r_color;
	unsigned char *g_color;
	unsigned char *b_color;
}RGBchannel;

typedef struct tagGREY
{
	int row;
	int col;
	unsigned char *grey_color;
}GREYchannel;


//void ocr_block_recog(char *ocr_name);
void ocr_block_recog(GREYchannel ocr);//
void block_col_min(GREYchannel grey);

char recog_symbol(GREYchannel bin_symbol, GREYchannel single_grey);

RGBchannel read_color_image(char *file_name);
GREYchannel read_red_image(char *file_name);
GREYchannel read_grey_image(char *file_name);

void write_grey_image(char *file_name, GREYchannel);

void write_color_image(char *file_name, RGBchannel rgb);
void write_grey8_image(char *file_name, GREYchannel grey);

GREYchannel read_binary_image(char *filename);
void write_binary_image(char *filename,GREYchannel image_symbol);

GREYchannel rgb_transform_grey(RGBchannel rgb);


#endif
