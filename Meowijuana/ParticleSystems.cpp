#define _USE_MATH_DEFINES
#include <vector>
#include <numbers>
#include "ParticleSystems.hpp"


// placeholder 
// eeeee i'll have to work on this and possibly new enemy types or something after i finish first level

std::vector<DamageParticle> damageParticles;

void spawnDamageParticles(float x, float y) {
	const int PARTICLES{ 10 };

	for (int i = 0; i < PARTICLES; i++) {
		float angle = (float)rand() / RAND_MAX * 2.0f * M_PI;
	}
}