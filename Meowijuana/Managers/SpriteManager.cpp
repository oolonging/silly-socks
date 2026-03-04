#include "SpriteManager.hpp"
#include <iostream>

#include "../Graphics.hpp"

namespace SpriteManager {
	std::unordered_map<std::string, SpriteSheet> spriteSheets;

	SpriteSheet* loadSpriteSheet(const std::string& name, const std::string& filepath,
								float sheetWidth, float sheetHeight,
								float spriteWidth, float spriteHeight) {

		// check if the spritesheet name already exists
		if (exists(name)) {
			std::cout << "[SpriteManager] Warning: Sprite sheet '" << name << "' is already loaded. Returning existing sheet." << std::endl;
			return &spriteSheets[name];
		}

		// Load the texture
		AEGfxTexture* pTex = AEGfxTextureLoad(filepath.c_str());

		if (pTex == nullptr) {
			std::cerr << "[SpriteManager] Error: Failed to load sprite sheet from '" << filepath << "'" << std::endl;
			return nullptr;
		}

		// Create and store the sprite sheet
		SpriteSheet spriteSheet(pTex, sheetWidth, sheetHeight, spriteWidth, spriteHeight);
		spriteSheets[name] = spriteSheet;

		std::cout << "[SpriteManager] Loaded sprite sheet '" << name << "' ("
			<< spriteSheet.columns << "x" << spriteSheet.rows << " sprites) from '" << filepath << "'\n";

		return &spriteSheets[name];
	}

	SpriteSheet* getSpriteSheet(const std::string& name) {
		auto it = spriteSheets.find(name);
		if (it != spriteSheets.end()) return &it->second;

		// Sprite sheet not found, return nullptr
		std::cerr << "[SpriteManager] Warning: Sprite sheet '" << name << "' not found." << std::endl;
		return nullptr;
	}

	bool exists(const std::string& name) {
		return spriteSheets.find(name) != spriteSheets.end();
	}

	Sprite createSprite(const std::string& sheetName, int x, int y) {
		SpriteSheet* pSpriteSheet = getSpriteSheet(sheetName);
		return createSprite(pSpriteSheet, x, y);
	}

	// Ensure this works: there was a bug here earlier
	Sprite createSprite(SpriteSheet* pSpriteSheet, int x, int y) {
		if (pSpriteSheet == nullptr) {
			std::cerr << "[SpriteManager] Error: Cannot create sprite from a null sprite sheet" << std::endl;
			return Sprite();
		}

		Sprite sprite(pSpriteSheet, x, y);

		if (sprite.isValid()) {
			return sprite;
		}
		else {
			return Sprite();
		}
	}

	void drawSprite(const Sprite& sprite, float x, float y, float width, float height, float rotation) {
		drawWithAlpha(sprite, x, y, width, height, 1.0f, rotation);
	}

	// TODO: hasnt been tested yet
	//void drawWithAlpha(const Sprite& sprite, float x, float y, float width, float height, float alpha, float rotation) {
	//	if (!sprite.isValid()) {
	//		std::cerr << "[SpriteManager] Error: cannot draw invalid sprite. " << std::endl;
	//		return;
	//	}

	//	SpriteSheet* pSpriteSheet = sprite.spriteSheet;

	//	// TODO: arithmetic

	//	// Calculate UV coordinates for the sprite within the sprite sheet
	//	float uOffset = (sprite.x * pSpriteSheet->spriteWidth) / pSpriteSheet->sheetWidth;
	//	float vOffset = (sprite.y * pSpriteSheet->spriteHeight) / pSpriteSheet->sheetHeight;
	//	float uScale = pSpriteSheet->spriteWidth / pSpriteSheet->sheetWidth;
	//	float vScale = pSpriteSheet->spriteHeight / pSpriteSheet->sheetHeight;

	//	// Set render mode and other related AE configs
	//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	//	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f); // TODO: maybe consider adding a way to tint the sprites so we can reuse them and save memory

	//	// Set texture with UV offset and scale to render only the specific sprite
	//	AEGfxTextureSet(pSpriteSheet->texture, uOffset, vOffset);

	//	// Draw a rect for the sprite
	//	// Stick to center for now, theres no reason to have a corner render
	//	Shapes::rect(x, y, width, height, Shapes::CENTER);
	//}

	void drawWithAlpha(const Sprite& sprite, float x, float y, float width, float height, float alpha, float rotation) {
		if (!sprite.isValid()) {
			std::cerr << "[SpriteManager] Error: Cannot draw invalid sprite.\n";
			return;
		}

		SpriteSheet* sheet = sprite.spriteSheet;

		// Calculate UV coordinates for the sprite within the sprite sheet
		float uOffset = (sprite.x * sheet->spriteWidth) / sheet->sheetWidth;
		float vOffset = (sprite.y * sheet->spriteHeight) / sheet->sheetHeight;
		float uScale = sheet->spriteWidth / sheet->sheetWidth;
		float vScale = sheet->spriteHeight / sheet->sheetHeight;

		// Set render mode to texture
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(alpha);

		// Set texture with UV offset and scale to render only the specific sprite
		AEGfxTextureSet(sheet->texture, uOffset, vOffset);

		// Create mesh for the sprite with proper UV coordinates
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, vScale,
			0.5f, -0.5f, 0xFFFFFFFF, uScale, vScale,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFFFF, uScale, vScale,
			0.5f, 0.5f, 0xFFFFFFFF, uScale, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		AEGfxVertexList* mesh = AEGfxMeshEnd();

		// Set transform matrix
		AEMtx33 transform;
		AEMtx33 scale, rotate, translate;

		AEMtx33Scale(&scale, width, height);
		AEMtx33Rot(&rotate, rotation);
		AEMtx33Trans(&translate, x, y);

		// Combine transformations: Scale -> Rotate -> Translate
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

		// Free the mesh
		AEGfxMeshFree(mesh);
	}


	void unload(const std::string& name) {
		auto it = spriteSheets.find(name);
		if (it != spriteSheets.end()) {
			if (it->second.texture != nullptr) {
				AEGfxTextureUnload(it->second.texture);
			}
			spriteSheets.erase(it);
			std::cout << "[SpriteManager] Unloaded sprite sheet '" << name << "'" << std::endl;
		}
	}

	void init() {
		spriteSheets.clear();
		std::cout << "[SpriteManager] Initialized" << std::endl;
	}


	void clear() {
		for (auto& pair : spriteSheets) {
			if (pair.second.texture != nullptr) {
				AEGfxTextureUnload(pair.second.texture);
			}
		}
		spriteSheets.clear();
		std::cout << "[SpriteManager] Cleared all sprite sheets" << std::endl;
	}
}

