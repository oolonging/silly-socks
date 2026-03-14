#include "../../pch.h"
#include "AEEngine.h"
#include "../../GameStateManager.hpp"

#include "../../Managers/ParticleManager.hpp"
#include "../../InputManager.hpp"

ParticleSystem particles(1000);

void testingParticles_Load() {
}

void testingParticles_Initialize() {
	particles.init(); 
	particles.spawnExplosion(0.0f, 0.0f, 50);
}

void testingParticles_Update() {
	particles.update(AEFrameRateControllerGetFrameTime());
}

void testingParticles_Draw() {
	// reset background
	Color::background(Color::Preset::Black);

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		particles.spawnDamageNumbers(Input::getMouseX(), Input::getMouseY(), 50);
		printf("SPAWNING PARTICLES");
	}

	particles.spawnTrail(Input::getMouseX(), Input::getMouseY(), 5);

	particles.draw();

}

void testingParticles_Free() {
	particles.clear();
}

void testingParticles_Unload() {

}

