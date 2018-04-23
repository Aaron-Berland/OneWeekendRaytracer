#include <iostream>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define CHANNEL_NUM 4
int main() {
	int width = 200;
	int height = 100;
	unsigned char* output_image;
	output_image = (unsigned char*) malloc(width * height * CHANNEL_NUM* sizeof(unsigned char));

	// Write your code to populate rgb_image here
	int j = 0;
	for (int i = 0; i < width * height; i++) {
		output_image[j++] = char((float((i % width)) / width) * 255.99);
		output_image[j++] = 255 - char((float((i / width)) / height) * 255.99);
		output_image[j++] = char(255*.2);
		output_image[j++] = 255;

	}
	stbi_write_png("image.png", width, height, CHANNEL_NUM, output_image, width*CHANNEL_NUM);

}