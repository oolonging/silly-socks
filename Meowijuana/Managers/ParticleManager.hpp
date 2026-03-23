#ifndef PARTICLE_MANAGER_HPP
#define PARTICLE_MANAGER_HPP

#include "../GraphicsTypes.hpp"

enum class ParticleShape {
	SQUARE,
	DIAMOND,
	ELLIPSE,
	TEXT
};

enum class ParticleRenderMode {
	SOLID_COLOR,
	TEXTURED
};

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

// Particle system configuration
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

// Main particle system class
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
	// Ctor
	ParticleSystem(size_t maxCount = 1000);		// defaults to 1000 particles which hopefully wont be too laggy

	// Dtor
	~ParticleSystem(void);

	// Initialization - MUST call this after AESysInit and AESysReset
	void init(void);

	// core functions
	void update(f64 dt = AEFrameRateControllerGetFrameTime());
	void draw(void);
	void clear(void);

	// Spawn function
	void spawnParticle(const ParticleConfig& config);	// TODO: potentially add a default config
	void spawnParticles(const ParticleConfig& config, int count);

	// Preset spawners (TODO: test later)
	void spawnExplosion(float x, float y, int count = 50);
	void spawnTrail(float x, float y, int count = 5);
	void spawnDamageNumbers(float x, float y, int count = 20);

	// Getters
	int getActiveParticleCount(void) const;
	int getMaxParticles(void) const {
		return static_cast<int>(this->maxParticles);
	}
};

// extern particles
extern ParticleSystem gParticles;


#endif // PARTICLE_MANAGER_HPP