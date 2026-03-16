#include "../../pch.h"
#include "testing-particles.hpp"

#include "../../Managers/ParticleManager.hpp"
#include "../../InputManager.hpp"

#include "../../Graphics.hpp"

void testingParticles_Load() {
}

void testingParticles_Initialize() {
	gParticles.init(); 
	gParticles.spawnExplosion(0.0f, 0.0f, 50);
}

void testingParticles_Update() {
	gParticles.update(AEFrameRateControllerGetFrameTime());
}

void testingParticles_Draw() {
	// reset background
	Color::background(Color::Preset::Black);

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		gParticles.spawnDamageNumbers(Input::getMouseX(), Input::getMouseY(), 50);
		printf("SPAWNING PARTICLES");
	}

	gParticles.spawnTrail(Input::getMouseX(), Input::getMouseY(), 5);

	gParticles.draw();

}

void testingParticles_Free() {
	gParticles.clear();
}

void testingParticles_Unload() {

}

