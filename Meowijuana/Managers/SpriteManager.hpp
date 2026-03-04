#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include <unordered_map>
#include <string>
#include "AEEngine.h"

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

	// Spritesheet storage
	extern std::unordered_map<std::string, SpriteSheet> spriteSheets;

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

	// Unload a specific sprite sheet
	void unload(const std::string& name);

	// Initialize the manager
	void init(void);

	// cleanup all the sprite sheets
	void clear(void);
}

#endif // SPRITE_MANAGER_HPP
