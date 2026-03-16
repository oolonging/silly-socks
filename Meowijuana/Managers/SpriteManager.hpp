#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include "../pch.h"

namespace SpriteManager {
	// SpriteSheet struct - holds the textuer and the grid information for the spritesheet
	typedef struct SpriteSheet {
		AEGfxTexture* texture;		// Pointer to the loaded texture
		float sheetWidth;			// Total width of the sprite sheet (in pixels)
		float sheetHeight;			// Total height of the sprite sheet (also in pixels)
		float spriteWidth;			// Width of each individual sprite
		float spriteHeight;			// Height of each individual sprite

		// derived values
		int columns;				// Number of sprites horizontally 
		int rows;					// Number of sprites vertically

		// Ctor
		SpriteSheet(AEGfxTexture* pTex, float sheetWidth, float sheetHeight, float spriteWidth, float spriteHeight)
			: texture(pTex), sheetWidth(sheetWidth), sheetHeight(sheetHeight), spriteWidth(spriteWidth), spriteHeight(spriteHeight) {

			// calculate columns and rows
			this->columns = static_cast<int>(this->sheetWidth / this->spriteWidth);
			this->rows = static_cast<int>(this->sheetHeight / this->spriteHeight);
		}

		SpriteSheet(void) : texture(nullptr), sheetWidth(0.0f), sheetHeight(0.0f), spriteWidth(0.0f), spriteHeight(0.0f), columns(0), rows(0) {}
	} SpriteSheet;

	typedef struct Sprite {
		SpriteSheet* spriteSheet;		// Pointer to the parent sprite sheet
		int x;							// X coordinate in the sprite grid (col index, starts from 0)
		int y;							// Y coordinate in the sprite grid (row index, starts from 0)

		// Ctors
		Sprite(SpriteSheet* spriteSheet, int x, int y)
			: spriteSheet(spriteSheet), x(x), y(y) {}

		Sprite(void) : spriteSheet(nullptr), x(0), y(0) {}

		// Safety function to ensure sprite is valid
		bool isValid() const {
			// checks if spritesheet is pointing to an actual spritesheet
			if (this->spriteSheet == nullptr) return false;

			// Checks if the spritesheet that is being pointed to itself is pointing to a valid texture
			if (this->spriteSheet->texture == nullptr) return false;

			// checks if the sprite coordinates are within range
			if (
				this->x >= 0 && this->x < spriteSheet->columns &&
				this->y >= 0 && this->y < spriteSheet->rows
				) {
				return true;
			}

			// If you're reaching this point it means that the coordinates are invalid
			return false;
		}
	} Sprite;

	// Animation struct - stores a sequence of sprites and timing info
	typedef struct Animation {
		std::vector<Sprite> frames;		// Sequence of sprites that make up the animation
		float frameDuration;			// Duration of each frame in seconds
		bool loop;						// Should the animation loop ? 

		// Ctors
		Animation(const std::vector<Sprite>& frames, float frameDuration, bool loop = true) :
			frames(frames), frameDuration(frameDuration), loop(loop) {}

		Animation(void) : frames(), frameDuration(0.0f), loop(true) {}

		// Safety functions to ensure animation is valid
		bool isValid(void) const {
			// Checks if there is something in the frames vector
			if (frames.empty()) return false;

			// Check if all frames are valid sprites
			for (const auto& frame : frames) {
				if (!frame.isValid()) return false;
			}

			return true;
		}

		// Get the current frame based on elapsed time
		const Sprite& getFrame(float elapsedTime) const {
			if (frames.empty()) {
				// TODO: understand whats happening here
				static Sprite invalidSprite;
				return invalidSprite;
			}

			int totalFrames = static_cast<int>(frames.size());
			float totalDuration = totalFrames * this->frameDuration;

			if (loop) {
				// Loop the animation using fmod. Alternatively (TODO: change the frameduration to miliseconds so I can use regular int mod probably)
				float loopedTime = fmod(elapsedTime, totalDuration);
				int frameIndex = static_cast<int>(loopedTime / frameDuration);
				if (frameIndex >= totalFrames) {
					frameIndex = totalFrames - 1; // TODO: technically in the unlikely event that frameIndex is greater than totalFrames by more than one, id have to reduce it by that amount instead of one
				}
				return frames[frameIndex];
			}
		}

		// Get current frame index based on elapsed time
		int getFrameIndex(float elapsedTime) const {
			if (frames.empty()) return 0;

			int totalFrames = static_cast<int>(frames.size());
			float totalDuration = totalFrames * frameDuration;

			if (loop) {
				float loopedTime = fmod(elapsedTime, totalDuration);
				int frameIndex = static_cast<int>(loopedTime / frameDuration);
				return frameIndex % totalFrames;
			}
			else {
				int frameIndex = static_cast<int>(elapsedTime / frameDuration);
				if (frameIndex >= totalFrames) {
					frameIndex = totalFrames - 1;
				}
				return frameIndex;
			}
		}

		// Check if an animation has finished (only relevant in non-looping animations)
		bool hasFinished(float elapsedTime) const {
			if (loop) return false;
			float totalDuration = frames.size() * frameDuration;
			return (elapsedTime >= totalDuration);
		}

	} Animation;

	// Spritesheet storage
	extern std::unordered_map<std::string, SpriteSheet> spriteSheets;

	// Animation storage
	extern std::unordered_map<std::string, Animation> animations;

	// Global animation timer (updated every frame)
	extern float gAnimationTime;

	// Load a sprite sheet from file (NOTE: PNG ONLY, AT LEAST FOR NOW)
	SpriteSheet* loadSpriteSheet(const std::string& name, const std::string& filepath,
								float sheetWidth, float sheetHeight,
								float spriteWidth, float spriteHeight);

	// Get a sprite sheet by name
	SpriteSheet* getSpriteSheet(const std::string& name);

	// Check if a sprite sheet exists for safety reasons
	bool exists(const std::string& name);

	// Create a sprite from a sprite sheet
	Sprite createSprite(const std::string& sheetName, int x, int y);
	Sprite createSprite(SpriteSheet* spriteSheet, int x, int y); // This one is just in case its not in the storage

	// Draw a sprite at a specific position with the given dimensions
	void drawSprite(const Sprite& sprite, float x, float y, float width, float height, float rotation = 0.0f);

	// Draw a sprite with custom transparency
	void drawWithAlpha(const Sprite& sprite, float x, float y, float width, float height, float alpha, float rotation = 0.0f);

	// ----- Animation functions -----

	// Create an animation from a sequence of sprites
	Animation* createAnimation(const std::string& name, const std::vector<Sprite>& frames, float frameDuration, bool loop = true);

	// Create an animation from a horizontal row in a sprite sheet
	Animation* createAnimationFromRow(const std::string& name, const std::string& sheetName, int row, int startCol, int frameCount, float frameDuration, bool loop = true);

	// Create an animation from a range of sprites in a sprite sheet (reading from left to right then top to bottom. Probably wont use it based on the sprites we have so far but its good to have)
	Animation* createAnimationFromRange(const std::string& name, const std::string& sheetName, int startX, int startY, int frameCount, float frameDuration, bool loop = true);

	// Get animation by name
	Animation* getAnimation(const std::string& name);

	// check if an animation exists
	bool animationExists(const std::string& name);

	// Draw an animation at the current global time
	void drawAnimation(const std::string& name, float x, float y, float width, float height, float rotation = 0.0f);
	void drawAnimation(const Animation& animation, float x, float y, float width, float height, float rotation = 0.0f);

	// Draw an animation with custom transparency at the current global time
	void drawAnimationWithAlpha(const std::string& name, float x, float y, float width, float height, float alpha, float rotation = 0.0f);
	void drawAnimationWithAlpha(const Animation& animation, float x, float y, float width, float height, float alpha, float rotation = 0.0f);


	// Draw an animation at a specific elapsed time (in case they have a custom timer divorced from the world time)
	void drawAnimationAtTime(const Animation& animation, float elapsedTime, float x, float y, float width, float height, float rotation = 0.0f);
	void drawAnimationAtTimeWithAlpha(const Animation& animation, float elapsedTime, float x, float y, float width, float height, float alpha, float rotation = 0.0f);

	// Update the global animation timer (call this once per frame in the game loop)
	void updateAnimationTime(float deltaTime);

	// Reset the global animation timer
	void resetAnimationTime(void);

	// Unload a specific animation
	void unloadAnimation(const std::string& name);

	// Unload a specific sprite sheet
	void unload(const std::string& name);

	// Initialize the manager
	void init(void);

	// cleanup all the sprite sheets
	void clear(void);
}

#endif // SPRITE_MANAGER_HPP
