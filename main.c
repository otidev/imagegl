#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ImageGL.h"
#include "stb_image_write.h"

int main() {
	Image image = LoadImage("example.tga");
	stbi_write_png("example.png", image.width, image.height, 4, image.pixels, image.width * 4);
}