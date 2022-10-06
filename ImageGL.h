#ifndef IMAGEGL_H
#define IMAGEGL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://stackoverflow.com/questions/60705649/how-to-reverse-a-16-bit-hex-value-in-c
#define Reverse(X) ((X << 24) | (((X>>16)<<24)>>16) | (((X<<16)>>24)<<16) | (X>>24))

// john from https://stackoverflow.com/questions/61645259/conversion-from-argb-to-rgba
uint32_t ToRGB(uint32_t argb)
{
	return
		// Source is in format: 0xAARRGGBB
		((argb & 0x00FF0000) >> 16)  | //______RR
		((argb & 0x0000FF00))        | //____GG__
		((argb & 0x000000FF) << 16)  | // ___BB____
		((argb & 0xFF000000));         //AA______
		// Return value is in format:  0xAABBGGRR
}

typedef struct Image {
	int width;
	int height;
	uint32_t* pixels;
} Image;

// Padding hurts
// typedef struct TGAColourMapSpec {
// 	uint16_t firstEntryIndex;
// 	uint16_t colourMapLength;
// 	uint8_t colourMapEntSize;
// } TGACMS;

typedef struct TGAImageSpec {
	uint16_t originX;
	uint16_t originY;
	uint16_t width;
	uint16_t height;
	uint8_t bitsPerPixel;
	uint8_t imageDsc;
} TGAImageSpec;

typedef struct TGAHeader {
	uint8_t idLength;
	uint8_t colourMapType; // Unused
	uint8_t imageType;
	uint8_t colourMapSpec[5]; // Unused (most of the time, also padding hurts)
	TGAImageSpec imageSpec;
} TGAHeader;

Image InitImage(int width, int height, uint32_t* pixels) {
	return (Image){width, height, pixels};
}

Image LoadImage(char* filename) {
	// Copied from Moon's Map.c
	Image image;
	// Load file
	int fileSize;

	FILE* file = fopen(filename, "rb");

	// Getting file size
	fseek(file, 0l, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0l, SEEK_SET);

	// Put it in a buffer.
	char fileBuf[fileSize];
	memset(fileBuf, 0, fileSize);
	fread(fileBuf, sizeof(char), fileSize, file);

	fclose(file);

	// Make image data
	TGAHeader* header = (TGAHeader*)fileBuf;
	image.width = header->imageSpec.width;
	image.height = header->imageSpec.height;
	uint32_t srcPixels[image.width * image.height * header->imageSpec.bitsPerPixel / 4];
	image.pixels = srcPixels;

	uint32_t* imgData = (uint32_t*)(fileBuf + sizeof(TGAHeader) + header->idLength + (uint16_t)header->colourMapSpec[3]);

	for (int y = 0; y < image.height; y++) {
		for (int x = 0; x < image.width; x++) {
			image.pixels[(y * image.width) + x] = ToRGB((uint32_t)*(imgData + (image.height - 1 - y) * image.width + x));
			printf("%x\n", *(imgData + (image.height - 1 - y) * image.width + x));
		}
	}

	return image;
}

void FillImage(Image* image, uint32_t colour) {
	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			image->pixels[(y * image->width) + x] = colour;
		}
	}
}

void DrawPixel(Image* image, int x, int y, uint32_t colour) {
	image->pixels[(y * image->width) + x] = colour;
}

#endif