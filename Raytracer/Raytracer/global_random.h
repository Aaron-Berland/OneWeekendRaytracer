#pragma once
#ifndef GLOBALRANDOMH
#define GLOBALRANDOMH
#include <random>
#include <chrono>

std::uniform_real_distribution<float> unif(0, 1);
std::mt19937_64 rng;

void initializeRandom() {
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(unif(rng), unif(rng), unif(rng)) - vec3(1.0, 1.0, 1.0);

	} while (p.squared_length() >= 1.0);
	return p;
}
float get_random() {
	
	return unif(rng);
}

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0*vec3(unif(rng), unif(rng), 0.0) - vec3(1.0, 1.0, 0.0);
	} while (dot(p, p) >= 1.0);
	return p;
}
#endif // !GLOBALRANDOMH
