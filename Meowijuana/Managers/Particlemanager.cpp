#include "ParticleManager.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

// Ctors and Dtor
ParticleSystem::ParticleSystem(size_t maxCount)
	: maxParticles(maxCount), squareMesh(nullptr), diamondMesh(nullptr), ellipseMesh(nullptr) {
	particles.resize(maxParticles);
	// Just a note to anyone who encounters this in the future
	// DON'T call createMeshes() here - AEEngine isn't ready yet
}

ParticleSystem::~ParticleSystem() {
	freeMeshes();
}

// Initialization - call this AFTER AESysInit and AESysReset
void ParticleSystem::init() {
	// Check if already initialized
	if (squareMesh && diamondMesh && ellipseMesh) return;

	createMeshes();
}

// Mesh creation
void ParticleSystem::createMeshes() {
	// Square mesh
	AEGfxMeshStart();
	AEGfxTriAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
	squareMesh = AEGfxMeshEnd();

	// TODO: change this to the diamond I had in mind (similar to the crit particles from minecraft)
	// Diamond mesh
	AEGfxMeshStart();
	AEGfxTriAdd(0.0f, 0.5f, 0xFFFFFFFF, 0.5f, 0.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 1.0f, 0.5f,
		-0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.5f);
	AEGfxTriAdd(0.0f, -0.5f, 0xFFFFFFFF, 0.5f, 1.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 1.0f, 0.5f,
		-0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.5f);
	diamondMesh = AEGfxMeshEnd();

	// Ellipse mesh
	const int NUM_SEGMENTS = 32;
	AEGfxMeshStart();

	for (int i = 0; i < NUM_SEGMENTS; i++) {
		float angle0 = (2.0f * PI * i) / NUM_SEGMENTS;
		float angle1 = (2.0f * PI * (i + 1)) / NUM_SEGMENTS;

		float x0 = 0.5f * cosf(angle0);
		float y0 = 0.5f * sinf(angle0);
		float x1 = 0.5f * cosf(angle1);
		float y1 = 0.5f * sinf(angle1);

		// with texture mapping
		AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.5f, 0.5f,
			x0, y0, 0xFFFFFFFF, (x0 / 0.5f + 1.0f) * 0.5f, (y0 / 0.5f + 1.0f) * 0.5f,
			x1, y1, 0xFFFFFFFF, (x1 / 0.5f + 1.0f) * 0.5f, (y1 / 0.5f + 1.0f) * 0.5f);
	}

	ellipseMesh = AEGfxMeshEnd();
}

void ParticleSystem::freeMeshes() {
	if (squareMesh) {
		AEGfxMeshFree(squareMesh);
		squareMesh = nullptr;
	}

	if (diamondMesh) {
		AEGfxMeshFree(diamondMesh);
		diamondMesh = nullptr;
	}

	if (ellipseMesh) {
		AEGfxMeshFree(ellipseMesh);
		ellipseMesh = nullptr;
	}
}

// Helper functions

float ParticleSystem::randomFloat(float min, float max) {
	return min + (static_cast<float>(rand()) / RAND_MAX) * (max - min);
}

Particle* ParticleSystem::getInactiveParticle() {
	for (auto& particle : particles) {
		if (!particle.active) {
			return &particle;
		}
	}
	return nullptr;
}

// Core functions

void ParticleSystem::update(float dt) {
	for (auto& particle : particles) {
		if (!particle.active) continue;

		// update lifetime
		particle.lifetime += dt;
		if (particle.lifetime >= particle.maxLifetime) {
			particle.active = false;
			continue;
		}

		// update position
		particle.x += particle.vx * dt;
		particle.y += particle.vy * dt;

		// update rotation
		particle.rotation += particle.rotationSpeed * dt;

		// Calculate alpha based on lifetime (if fading)
		particle.alpha = 1.0f - (particle.lifetime / particle.maxLifetime);
	}
}

void ParticleSystem::draw() {
	for (const auto& particle : particles) {
		if (!particle.active) continue;

		// Set render mode
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(particle.alpha);

		// Set color using the Color struct (values are 0-255, so normalize to 0-1)
		AEGfxSetColorToMultiply(
			particle.color.red / 255.0f,
			particle.color.green / 255.0f,
			particle.color.blue / 255.0f,
			particle.alpha
		);

		// Create transformation matrix
		AEMtx33 scale, rotate, translate, transform;
		AEMtx33Scale(&scale, particle.size, particle.size);
		AEMtx33Rot(&rotate, particle.rotation);
		AEMtx33Trans(&translate, particle.x, particle.y);

		// Combine transformations: Scale -> Rotate -> Translate
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		// Set transform and draw
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Reset render state
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
}

void ParticleSystem::clear() {
	for (auto& particle : particles) {
		particle.active = false;
	}
}

// spawn functions
void ParticleSystem::spawnParticle(const ParticleConfig& config) {
	Particle* particle = getInactiveParticle();
	if (!particle) return; // no available particles

	// Position
	float angle = randomFloat(0.0f, 2.0f * PI);
	float radius = randomFloat(0.0f, config.spawnRadius);
	particle->x = config.spawnX + radius * cosf(angle);
	particle->y = config.spawnY + radius * sinf(angle);

	// Velocity
	float speed = randomFloat(config.minSpeed, config.maxSpeed);
	float velAngle = randomFloat(config.minAngle, config.maxAngle);
	particle->vx = speed * cosf(velAngle);
	particle->vy = speed * sinf(velAngle);

	// Lifetime
	particle->lifetime = 0.0f;
	particle->maxLifetime = randomFloat(config.minLifetime, config.maxLifetime);

	// Rotation
	particle->rotation = randomFloat(0.0f, 2.0f * PI);
	particle->rotationSpeed = randomFloat(config.minRotationSpeed, config.maxRotationSpeed);

	// Visual
	particle->color = config.color;
	particle->alpha = 1.0f;

	// Activate particle
	particle->active = true;
}

void ParticleSystem::spawnParticles(const ParticleConfig& config, int count) {
	for (int i = 0; i < count; i++) {
		spawnParticle(config);
	}
}

// Preset spawners TODO: figure out the text rendering, but add graphical rendering first as a workaround
void ParticleSystem::spawnExplosion(float x, float y, int count) {
	ParticleConfig config;
	config.shape = ParticleShape::SQUARE;
	config.spawnX = x;
	config.spawnY = y;
	config.spawnRadius = 5.0f;
	config.minSpeed = 100.0f;
	config.maxSpeed = 300.0f;
	config.minAngle = 0.0f;
	config.maxAngle = 2.0f * PI;
	config.minLifetime = 0.3f;
	config.maxLifetime = 1.0f;
	config.minSize = 5.0f;
	config.maxSize = 15.0f;
	config.gravity = -200.0f;
	config.color = Color::Color(255.0f, 68.0f, 0.0f, 255.0f); // Orange
	config.fadeOut = true;

	spawnParticles(config, count);
}

void ParticleSystem::spawnTrail(float x, float y, int count) {
	ParticleConfig config;
	config.shape = ParticleShape::ELLIPSE;
	config.spawnX = x;
	config.spawnY = y;
	config.spawnRadius = 3.0f;
	config.minSpeed = 10.0f;
	config.maxSpeed = 30.0f;
	config.minLifetime = 0.2f;
	config.maxLifetime = 0.5f;
	config.minSize = 3.0f;
	config.maxSize = 8.0f;
	config.color = Color::Color(255.0f, 255.0f, 255.0f, 255.0f); // White
	config.fadeOut = true;

	spawnParticles(config, count);
}

void ParticleSystem::spawnDamageNumbers(float x, float y, int count) {
	ParticleConfig config;
	config.shape = ParticleShape::DIAMOND;
	config.spawnX = x;
	config.spawnY = y;
	config.spawnRadius = 10.0f;
	config.minSpeed = 50.0f;
	config.maxSpeed = 150.0f;
	config.minAngle = PI / 4.0f;
	config.maxAngle = 3.0f * PI / 4.0f;
	config.minLifetime = 0.5f;
	config.maxLifetime = 1.5f;
	config.minSize = 8.0f;
	config.maxSize = 12.0f;
	config.gravity = -100.0f;
	config.color = Color::Color(255.0f, 0.0f, 0.0f, 255.0f); // Red
	config.fadeOut = true;

	spawnParticles(config, count);
}


// Getters
int ParticleSystem::getActiveParticleCount() const {
	int count = 0;
	for (const auto& particle : particles) {
		if (particle.active) count++;
	}
	return count;
}