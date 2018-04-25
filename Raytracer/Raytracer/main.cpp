#include <iostream>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define CHANNEL_NUM 4
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
vec3 color(const ray& r, hitable *world) {
	hit_record rec;
	if(world->hit(r,0.0,FLT_MAX,rec)){
		return 0.5*vec3(rec.normal.x()+1.0, rec.normal.y() + 1.0, rec.normal.z() + 1.0);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = .5*(unit_direction.y() + 1.0);
		//std::cout << t << " El float \n";
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}
int main() {
	int width = 200;
	int height = 100;
	unsigned char* output_image;
	vec3 lower_left_origin(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	hitable  *list[2];
	list[0] = new sphere(vec3(0.0, 0.0, -1.0), .5);
	list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100);
	hitable* world = new hitable_list(list, 2);

	output_image = (unsigned char*) malloc(width * height * CHANNEL_NUM* sizeof(unsigned char));

	int j = 0;
	for (int i = 0; i < width * height; i++) {
		float u = float((i%width)) / float(width);
		float v = 1 - float((i / width)) / float(height);
		ray r(origin, lower_left_origin + u*horizontal + v*vertical);
		vec3 col = color(r,world);
		//std::cout << "Direction : " << r.direction() << std::endl;
		//std::cout << "Direction Normalized: " << unit_vector(r.direction()) << std::endl;
		//std::cout <<"Color : " << col << std::endl;
		//std::cout << int(255.99*col[0]) << " " << 255-int(255.99*col[1])<< " " << int(255.99*col[2]) << std::endl;
		output_image[j++] = int(255.99*col[0]);
		output_image[j++] = int(255.99*col[1]);
		output_image[j++] = int(255.99*col[2]);
		/*
		output_image[j++] = 255;
		output_image[j++] = 0;
		output_image[j++] = 0;
		*/
		output_image[j++] = 255;

	}
	stbi_write_png("image.png", width, height, CHANNEL_NUM, output_image, width*CHANNEL_NUM);

}