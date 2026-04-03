#include "../pch.h"
#include "SpriteManager.hpp"
#include "../../World.hpp"
#include "../Graphics.hpp"


AEGfxVertexList* mesh = nullptr;

void initMesh(float uScale, float vScale) {
	if (mesh != nullptr) return;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, vScale,
		0.5f, -0.5f, 0xFFFFFFFF, uScale, vScale,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, uScale, vScale,
		0.5f, 0.5f, 0xFFFFFFFF, uScale, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	mesh = AEGfxMeshEnd();
}

namespace SpriteManager {
	// For spritesheet
	std::unordered_map<std::string, SpriteSheet> spriteSheets;
	
	// For animations
	std::unordered_map<std::string, Animation> animations;
	f64 gAnimationTime = 0.0f;

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
		AEGfxVertexList* _mesh = AEGfxMeshEnd();

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
		AEGfxMeshDraw(_mesh, AE_GFX_MDM_TRIANGLES);

		// Free the mesh
		AEGfxMeshFree(_mesh);
	}
	


	// basically the same but without mesh creation because i was lagging super hard? might still hae issues though :(
	void drawSpriteFromSheet(const Sprite& sprite, float x, float y, float width, float height, float alpha, float rotation) {
		if (!sprite.isValid()) {
			std::cerr << "[SpriteManager] Error: Cannot draw invalid sprite.\n";
			return;
		}

		SpriteSheet* sheet = sprite.spriteSheet;

		if (mesh == nullptr) {
			float uScale = sheet->spriteWidth / sheet->sheetWidth;
			float vScale = sheet->spriteHeight / sheet->sheetHeight;
			initMesh(uScale, vScale);
		}

		// Calculate UV coordinates for the sprite within the sprite sheet
		float uOffset = (sprite.x * sheet->spriteWidth) / sheet->sheetWidth;
		float vOffset = (sprite.y * sheet->spriteHeight) / sheet->sheetHeight;

		// Set render mode to texture
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(alpha);

		// Set texture with UV offset and scale to render only the specific sprite
		AEGfxTextureSet(sheet->texture, uOffset, vOffset);

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

		//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//AEGfxTextureSet(nullptr, 0, 0);

	}



	// ----- Animation implementations -----

	Animation* createAnimation(const std::string& name, const std::vector<Sprite>& frames, float frameDuration, bool loop) {
		if (frames.empty()) {
			std::cerr << "[SpriteManager] Error: Cannot create animation '" << name << "' with no frames." << std::endl;
			return nullptr;
		}

		// Check if animation already exists
		if (animationExists(name)) {
			std::cout << "[SpriteManager] Warning: Animation '" << name << "' already exists. Returning existing animation." << std::endl;
			return &animations[name];
		}

		// Create and store the animation
		Animation animation(frames, frameDuration, loop);

		if (!animation.isValid()) {
			std::cerr << "[SpriteManager] Error: Animation '" << name << "' contains invalid frames." << std::endl;
			return nullptr;
		}

		animations[name] = animation;
		std::cout << "[SpriteManager] Created animation '" << name << "' with " << frames.size() << " frames." << std::endl;

		return &animations[name];
	}

	Animation* createAnimationFromRow(const std::string& name, const std::string& sheetName, int row, int startCol, int frameCount, float frameDuration, bool loop) {
		SpriteSheet* sheet = getSpriteSheet(sheetName);

		if (sheet == nullptr) {
			std::cerr << "[SpriteManager] Error: Cannot create animation from non-existent sprite sheet '" << sheetName << "'" << std::endl;
			return nullptr;
		}

		std::vector<Sprite> frames;
		for (int i = 0; i < frameCount; i++) {
			Sprite sprite = createSprite(sheet, startCol + i, row);
			if (!sprite.isValid()) {
				std::cerr << "[SpriteManager] Error: Invalid sprite at (" << (startCol + i) << ", " << row << ") while creating animation '" << name << "'" << std::endl;
				return nullptr;
			}
			frames.push_back(sprite);
		}

		return createAnimation(name, frames, frameDuration, loop);
	}

	Animation* createAnimationFromRange(const std::string& name, const std::string& sheetName, int startX, int startY, int frameCount, float frameDuration, bool loop) {
		SpriteSheet* sheet = getSpriteSheet(sheetName);

		if (sheet == nullptr) {
			std::cerr << "[SpriteManager] Error: Cannot create animation from non-existent sprite sheet '" << sheetName << "'" << std::endl;
			return nullptr;
		}

		std::vector<Sprite> frames;
		int currentX = startX;
		int currentY = startY;

		for (int i = 0; i < frameCount; i++) {
			Sprite sprite = createSprite(sheet, currentX, currentY);
			if (!sprite.isValid()) {
				std::cerr << "[SpriteManager] Error: Invalid sprite at (" << currentX << ", " << currentY << ") while creating animation '" << name << "'" << std::endl;
				return nullptr;
			}
			frames.push_back(sprite);

			// Move to next sprite (also account for when you reach the end and have to increment row)
			currentX++;
			if (currentX >= sheet->columns) {
				currentX = 0;
				currentY++;
			}
		}

		return createAnimation(name, frames, frameDuration, loop);
	}

	Animation* getAnimation(const std::string& name) {
		auto it = animations.find(name);
		if (it != animations.end()) {
			return &it->second;
		}

		std::cerr << "[SpriteManager] Warning: Animation '" << name << "' not found." << std::endl;
		return nullptr;
	}

	bool animationExists(const std::string& name) {
		return (animations.find(name) != animations.end());
	}

	// --- draw from here ---
	void drawAnimation(const std::string& name, float x, float y, float width, float height, float rotation) {
		Animation* anim = getAnimation(name);
		if (anim != nullptr) {
			drawAnimation(*anim, x, y, width, height, rotation);
		}
	}

	void drawAnimation(const Animation& animation, float x, float y, float width, float height, float rotation) {
		drawAnimationAtTime(animation, gAnimationTime, x, y, width, height, rotation);
	}

	void drawAnimationWithAlpha(const std::string& name, float x, float y, float width, float height, float alpha, float rotation) {
		Animation* anim = getAnimation(name);
		if (anim != nullptr) {
			drawAnimationWithAlpha(*anim, x, y, width, height, alpha, rotation);
		}
	}

	void drawAnimationWithAlpha(const Animation& animation, float x, float y, float width, float height, float alpha, float rotation) {
		drawAnimationAtTimeWithAlpha(animation, gAnimationTime, x, y, width, height, alpha, rotation);
	}

	void drawAnimationAtTime(const Animation& animation, f64 elapsedTime, float x, float y, float width, float height, float rotation) {
		drawAnimationAtTimeWithAlpha(animation, elapsedTime, x, y, width, height, 1.0f, rotation);
	}

	void drawAnimationAtTimeWithAlpha(const Animation& animation, f64 elapsedTime, float x, float y, float width, float height, float alpha, float rotation) {
		if (!animation.isValid()) {
			std::cerr << "[SpriteManager] Error: Cannot draw invalid animation." << std::endl;
			return;
		}

		const Sprite& currentFrame = animation.getFrame(static_cast<float>(elapsedTime));
		drawWithAlpha(currentFrame, x, y, width, height, alpha, rotation);
	}

	void updateAnimationTime(f64 deltaTime) {
		gAnimationTime += deltaTime;
	}

	void resetAnimationTime(void) {
		gAnimationTime = 0.0f;
	}

	void unloadAnimation(const std::string& name) {
		auto it = animations.find(name);
		if (it != animations.end()) {
			animations.erase(it);
			std::cout << "[SpriteManager] Unloaded animation '" << name << "'" << std::endl;
		}
	}
	// --- draw to here ---

	// unload spritesheet
	void unload(const std::string& name) {
		auto it = spriteSheets.find(name);
		if (it != spriteSheets.end()) {
			if (it->second.texture != nullptr) {
				AEGfxTextureUnload(it->second.texture);
				it->second.texture = nullptr; // technically unecessary since object is being destroyed, I just put it to hunt down a bug
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
				pair.second.texture = nullptr; // same as for the above nullptr assignment
			}
		}
		spriteSheets.clear();
		animations.clear();
		gAnimationTime = 0.0f;
		std::cout << "[SpriteManager] Cleared all sprite sheets and animations" << std::endl;
	}


}



