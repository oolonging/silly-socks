/**
 * @file ParticleManager.hpp
 * @author Saahil
 * @brief Particle system manager for spawning and managing visual particle effects.
 */

#ifndef PARTICLE_MANAGER_HPP
#define PARTICLE_MANAGER_HPP

#include "../GraphicsTypes.hpp"

 /**
  * @enum ParticleShape
  * @brief Defines the available shapes for rendering particles.
  */
enum class ParticleShape {
	SQUARE,
	DIAMOND,
	ELLIPSE,
	TEXT
};

/**
 * @enum ParticleRenderMode
 * @brief Defines how a particle is rendered.
 */

enum class ParticleRenderMode {
	SOLID_COLOR,
	TEXTURED
};

/**
 * @struct Particle
 * @brief Represents a single particle with position, velocity, lifetime, and visual properties.
 */
struct Particle {
	float x, y;				// Position
	float vx, vy;			// Velocity 
	float lifetime;			// Lifetime in seconds
	float maxLifetime;		// Total lifetime before death (also in seconds)
	float size;				// Base size
	float rotation;			// Rotation angle (radians)
	float rotationSpeed;	// Rotation speed (radians / secdond)
	Color::Color color;		// What color should the particle be (RGBA floats 0-255)
	float alpha;			// How transparent it is (0.0f - 1.0f)
	bool active;			// Determines if particle is alive

	// Ctors
	Particle(void) : x(0.0f), y(0.0f), vx(0.0f), vy(0.0f), lifetime(0.0f), maxLifetime(1.0f),
		size(10.0f), rotation(0.0f), rotationSpeed(0.0f), color(255.0f, 255.0f, 255.0f, 255.0f), alpha(1.0f),
		active(false) {
	}
};

/**
 * @struct ParticleConfig
 * @brief Configuration for spawning particles, including shape, velocity, lifetime, and visual parameters.
 */
struct ParticleConfig {
	ParticleShape shape;
	ParticleRenderMode renderMode;
	AEGfxTexture* pTex;					// Texture if using textured mode

	// Spawn parameters
	float spawnX;					// Spawn position (x axis)
	float spawnY;					// Spawn position (y axis)
	float spawnRadius;				// Radius in which the particles spawn

	// Velocity params
	float minSpeed;
	float maxSpeed;
	float minAngle;
	float maxAngle;

	// Lifetime parameters
	float minLifetime;
	float maxLifetime;

	// Size parameters
	float minSize;
	float maxSize;

	// Rotation parameters
	float minRotationSpeed;
	float maxRotationSpeed;

	// Physics
	float gravity;			// Gravitational acceleration (px/sec^2)
	float damping;			// Velocity damping (0.0f to 1.0f);

	// Visual
	Color::Color color;		// Base color
	bool fadeOut;			// Fade alpha over lifetime (y/n)

	ParticleConfig() :
		shape(ParticleShape::SQUARE),
		renderMode(ParticleRenderMode::SOLID_COLOR),
		pTex(nullptr),
		spawnX(0.0f), spawnY(0.0f), spawnRadius(0.0f),
		minSpeed(50.0f), maxSpeed(100.0f),
		minAngle(0.0f), maxAngle(6.28318f), // 0 to 2*PI
		minLifetime(0.5f), maxLifetime(2.0f),
		minSize(5.0f), maxSize(15.0f),
		minRotationSpeed(-3.14159f), maxRotationSpeed(3.14159f),
		gravity(0.0f), damping(1.0f),
		color(255.0f, 255.0f, 255.0f, 255.0f), fadeOut(true) {
	}
};

/**
 * @class ParticleSystem
 * @brief Manages a pool of particles, handling spawning, updating, and rendering.
 */
class ParticleSystem {
public:
	// temporary while i rewire some stuff
	std::vector<Particle> particles;
private:
	size_t maxParticles;

	// Cached meshes
	AEGfxVertexList* squareMesh;
	AEGfxVertexList* diamondMesh;
	AEGfxVertexList* ellipseMesh;

	// Helper functions
	void createMeshes(void);					// Just call this once to init the meshes same as how it works in graphics
	void freeMeshes(void);						// Same as the exit for graphics
	float randomFloat(float min, float max);	// returns a float value between the 2 numbers
	Particle* getInactiveParticle(void);

public:
	/**
	 * @brief Constructs a ParticleSystem with the given pool size.
	 * @param maxCount Maximum number of particles (defaults to 1000).
	 */
	ParticleSystem(size_t maxCount = 1000);

	/**
	 * @brief Destructor. Frees all cached meshes.
	 */
	~ParticleSystem(void);

	/**
	 * @brief Initializes the particle system. MUST be called after AESysInit and AESysReset.
	 */
	void init(void);

	/**
	 * @brief Updates all active particles (lifetime, position, rotation, alpha).
	 * @param dt Delta time in seconds. Defaults to AEFrameRateControllerGetFrameTime().
	 */
	void update(f64 dt = AEFrameRateControllerGetFrameTime());

	/**
	 * @brief Draws all active particles to the screen.
	 */
	void draw(void);

	/**
	 * @brief Deactivates all particles in the pool.
	 */
	void clear(void);

	/**
	 * @brief Spawns a single particle using the given configuration.
	 * @param config The ParticleConfig describing spawn parameters.
	 */
	void spawnParticle(const ParticleConfig& config);

	/**
	 * @brief Spawns multiple particles using the given configuration.
	 * @param config The ParticleConfig describing spawn parameters.
	 * @param count Number of particles to spawn.
	 */
	void spawnParticles(const ParticleConfig& config, int count);

	/**
	 * @brief Spawns an explosion burst of particles at the given position.
	 * @param x X-coordinate of the explosion center.
	 * @param y Y-coordinate of the explosion center.
	 * @param count Number of particles to spawn (defaults to 50).
	 */
	void spawnExplosion(float x, float y, int count = 50);

	/**
	 * @brief Spawns a trail of particles at the given position.
	 * @param x X-coordinate of the trail origin.
	 * @param y Y-coordinate of the trail origin.
	 * @param count Number of particles to spawn (defaults to 5).
	 */
	void spawnTrail(float x, float y, int count = 5);

	/**
	 * @brief Spawns damage number particles at the given position.
	 * @param x X-coordinate of the damage location.
	 * @param y Y-coordinate of the damage location.
	 * @param count Number of particles to spawn (defaults to 20).
	 */
	void spawnDamageNumbers(float x, float y, int count = 20);

	/**
	 * @brief Returns the number of currently active particles.
	 * @return Count of active particles.
	 */
	int getActiveParticleCount(void) const;

	/**
	 * @brief Returns the maximum number of particles in the pool.
	 * @return Maximum particle count.
	 */
	int getMaxParticles(void) const {
		return static_cast<int>(this->maxParticles);
	}
};

/// @brief Global particle system instance.
extern ParticleSystem gParticles;


#endif // PARTICLE_MANAGER_HPP
