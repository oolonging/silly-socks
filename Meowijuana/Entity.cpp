#include "pch.h"
#include "Entity.hpp"
#include "Collision.hpp"
#include "Tiles.hpp"
#include "Managers/ParticleManager.hpp"
#include "AudioManager.hpp"
#include "Settings.hpp"

namespace Entity {

	// -------------------------------------------------------------------------
	// Entity Base Class Implementation
	// -------------------------------------------------------------------------

	Entity::Entity(float posX, float posY, float w, float h, float health, float spd, float arm)
		: x(posX), y(posY), width(w), height(h), hp(health), maxHp(health), speed(spd), armor(arm), sprite(nullptr),
		healthBar(0.0f, 0.0f, w, 10.0f, &health, 0.0f, health, Shapes::CORNER), isDead(false) {

	}

	Entity::Entity(void)
		: Entity(0.0f, 0.0f, 50.0f, 50.0f, 100.0f, 1.0f, 0.0f) {}

	void Entity::setPosition(float posX, float posY) {
		x = posX;
		y = posY;
	}

	void Entity::setHp(float health) {
		hp = (health > maxHp) ? maxHp : ((health < 0) ? 0 : health);
		updateHealthBar();
	}

	void Entity::setSpeed(float spd) {
		speed = spd;
	}

	void Entity::setArmor(float arm) {
		armor = arm;
	}

	void Entity::setSprite(AEGfxTexture* tex) {
		sprite = tex;
	}

	void Entity::setAtkSpd(float spd) {
		atkSpd = spd;
	}

	void Entity::setAtkCD(float CD) {
		atkCD = CD;
	}

	void Entity::updateHealthBar() {
		healthBar.setValue(hp);
	}

	void Entity::drawHealthBar() {
		// set render mode to color
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		// Update health bar value
		healthBar.setValue(hp);

		// Position health bar above entity
		float barX = x - width / 2.0f;
		float barY = y + height / 2.0f + 10.0f;

		// ProgressBar::draw() now uses its internal position, 
		// but we need to update position dynamically
		// We'll call the overloaded version that accepts position

		// Calculate filled width based on HP percentage
		float fillPercentage = (maxHp > 0) ? (hp / maxHp) : 0.0f;
		float filledWidth = width * fillPercentage;


		// Draw background
		Color::stroke(0, 0, 0, 255);
		Color::strokeWeight(1.0f);
		Color::fill(100, 100, 100, 255);
		Shapes::rect(barX, barY, width, 10.0f, Shapes::CORNER);

		// Draw foreground (health)
		Color::noStroke();
		if (fillPercentage > 0.5f) {
			Color::fill(0, 255, 0, 255); // Green
		}
		else if (fillPercentage > 0.25f) {
			Color::fill(255, 255, 0, 255); // Yellow
		}
		else {
			Color::fill(255, 0, 0, 255); // Red
		}
		Shapes::rect(barX, barY, filledWidth, 10.0f, Shapes::CORNER);
	}

	void Entity::takeDamage(float rawDamage) {
		float effectiveDamage = rawDamage - armor;
		if (effectiveDamage < 0) effectiveDamage = 0;

		hp -= effectiveDamage;
		if (hp < 0) hp = 0;

		updateHealthBar();

	}


	void Entity::tickAttackTimer() {
		if (atkCD > 0.0f)
			atkCD -= (float)AEFrameRateControllerGetFrameTime();
	}

	bool Entity::canAttack() {
		return atkCD <= 0.0f;
	}

	void Entity::resetAttackTimer() {
		atkCD = 1.0f / atkSpd; // i set enemy spd to 0.8 and carrot sword is 2 so umm
	}


	void Entity::attack(Entity& target) {
		
		if (!equippedWeapon) {
			std::cout << "No weapon\n";
			return;
		}

		if (isPlayer() && !holdingWeapon) {
			std::cout << "No weapon\n";
			return;
		}
		
		std::cout << "Attack called\n";

		float range = equippedWeapon->getRange();
		float dx = target.getX() - x;
		float dy = target.getY() - y;
		float dist = sqrt(dx * dx + dy * dy);

		std::cout << "Distance: " << dist << " | Range: " << range << "\n";

		bool hit = Collision::collidedWith(x, y, target.getX(), target.getY(), range, target.getWidth(), target.getHeight());

		if (!hit)
			return;

		std::cout << "Collision non-issue\n";
		std::cout << "Target HP before onAttack: " << target.getHp() << "\n";
		equippedWeapon->onAttack(*this, target);
		gParticles.spawnExplosion(target.getX(), target.getY(), 20);

		AudioManager::audio.playSFX(1.0f);
		std::cout << "Target HP after onAttack: " << target.getHp() << "\n";
	}

	void Entity::draw() {
		// Default implementation (can be overridden)
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		Color::stroke(0, 0, 0, 255);
		Color::strokeWeight(2.0f);
		Color::fill(128, 128, 128, 255);
		Shapes::rect(x, y, width, height, Shapes::CENTER);
	}

	// -------------------------------------------------------------------------
	// Player Implementation
	// -------------------------------------------------------------------------

	Player::Player(float posX, float posY, float w, float h, float health, float spd, float arm)
		: Entity(posX, posY, w, h, health, spd, arm), movingDirections{ false, false, false, false }, selectedInventorySlot(0) {
		// Initialize inventory slots to nullptr
		for (int i = 0; i < 9; ++i) {
			inventory[i] = nullptr;
		}
	}

	Player::Player()
		: Entity(), movingDirections{ false, false, false, false }, selectedInventorySlot(0) {
		// Initialize inventory slots to nullptr
		for (int i = 0; i < 9; ++i) {
			inventory[i] = nullptr;
		}
	}

	// -------------------------------------------------------------------------
	// Player Inventory
	// -------------------------------------------------------------------------

	Inventory::Item* Player::getInventoryItem(int slot) const {
		if (slot >= 0 && slot < 9) {
			return inventory[slot];
		}
		return nullptr;
	}

	void Player::setInventoryItem(int slot, Inventory::Item* item) {
		if (slot >= 0 && slot < 9) {
			inventory[slot] = item;
		}
	}

	int Player::getSelectedInventorySlot() const {
		return selectedInventorySlot;
	}

	void Player::setSelectedInventorySlot(int slot) {
		if (slot >= 0 && slot < 9) {
			selectedInventorySlot = slot;
		}
	}

	void Player::clearInventorySlot(int slot)
	{
		if (slot < 0 || slot >= 9) return;
		if (inventory[slot] != nullptr)
		{
			inventory[slot]->setIcon(nullptr); // null the icon first
			delete inventory[slot];            // then delete the item
			inventory[slot] = nullptr;
		}
	}

	void Player::freeInventory()
	{
		// Loop through all inventory and delete everything
		for (int i = 0; i < 9; i++)
		{
			if (inventory[i] != nullptr)
			{
				delete inventory[i];
				inventory[i] = nullptr;
			}
		}
	}

	void Player::handleMovement() {
		movingDirections[0] = AEInputCheckCurr(AEVK_W); // up
		movingDirections[1] = AEInputCheckCurr(AEVK_S); // down
		movingDirections[2] = AEInputCheckCurr(AEVK_A); // left
		movingDirections[3] = AEInputCheckCurr(AEVK_D); // right

		float deltaX = 0.0f;
		float deltaY = 0.0f;

		if (movingDirections[0]) deltaY += speed;
		if (movingDirections[1]) deltaY -= speed;
		if (movingDirections[2]) deltaX -= speed;
		if (movingDirections[3]) deltaX += speed;

		// Normalize diagonal movement
		if (deltaX != 0.0f && deltaY != 0.0f) {
			float length = sqrtf(deltaX * deltaX + deltaY * deltaY);
			deltaX = (deltaX / length) * speed;
			deltaY = (deltaY / length) * speed;
		}

		float newPositionX = x + deltaX;
		float newPositionY = y + deltaY;

		// Entity is rendered from center, so check all four corners
		float halfWidth = width / 2.0f;
		float halfHeight = height / 2.0f;

		// Check all four corners of the entity's bounding box
		bool canMove = true;

		// Top-left corner
		World::Tile& tlTile = World::getTileFromWorld(newPositionX - halfWidth, newPositionY + halfHeight);
		if (tlTile.hasTileObject() && !tlTile.getTileObject()->traversable) {
			canMove = false;
		}

		// Top-right corner
		if (canMove) {
			World::Tile& trTile = World::getTileFromWorld(newPositionX + halfWidth, newPositionY + halfHeight);
			if (trTile.hasTileObject() && !trTile.getTileObject()->traversable) {
				canMove = false;
			}
		}

		// Bottom-left corner
		if (canMove) {
			World::Tile& blTile = World::getTileFromWorld(newPositionX - halfWidth, newPositionY - halfHeight);
			if (blTile.hasTileObject() && !blTile.getTileObject()->traversable) {
				canMove = false;
			}
		}

		// Bottom-right corner
		if (canMove) {
			World::Tile& brTile = World::getTileFromWorld(newPositionX + halfWidth, newPositionY - halfHeight);
			if (brTile.hasTileObject() && !brTile.getTileObject()->traversable) {
				canMove = false;
			}
		}

		// Only move if all corners are on traversable tiles or empty tiles
		if (canMove) {
			setPosition(newPositionX, newPositionY);
		}

	}

	void Player::giveItem(int itemID, int itemCount)
	{
		for (int i = 0; i < this->getInventorySize(); i++)
		{
			if (this->getInventoryItem(i) != nullptr)
			{
				if (this->getInventoryItem(i)->getID() == itemID)
				{
					int count = this->getInventoryItem(i)->getCount();
					this->getInventoryItem(i)->setCount(count + itemCount);
					return;
				}
			}
			else
			{
				Inventory::Item* item = Inventory::ItemRegistry::createItem(itemID);
				item->setCount(itemCount);
				this->setInventoryItem(i, item);
				return;
			}
		}
	}

	// -------------------------------------------------------------------------
	// Player Movement
	// -------------------------------------------------------------------------

	// Movement using GRID to check for collision
	void Player::handleMovement(World::worldGrid& Griddy)
	{
		movingDirections[0] = AEInputCheckCurr(AEVK_W);
		movingDirections[1] = AEInputCheckCurr(AEVK_S);
		movingDirections[2] = AEInputCheckCurr(AEVK_A);
		movingDirections[3] = AEInputCheckCurr(AEVK_D);

		float deltaX = 0.0f;
		float deltaY = 0.0f;
		if (movingDirections[0]) deltaY += speed;
		if (movingDirections[1]) deltaY -= speed;
		if (movingDirections[2]) {
			deltaX -= speed;
			this->facingDirection = 0;
		}
		if (movingDirections[3]) {
			deltaX += speed;
			this->facingDirection = 1;
		}
		
		// set iswalking var
		if (!deltaX && !deltaY) {
			this->isWalking = false;
		}
		else {
			this->isWalking = true;
		}

		if (deltaX != 0.0f && deltaY != 0.0f) {
			float length = sqrtf(deltaX * deltaX + deltaY * deltaY);
			deltaX = (deltaX / length) * speed;
			deltaY = (deltaY / length) * speed;
		}

		const float epsilon = 18.f;
		float halfWidth = (width / 2.0f) - epsilon;
		float halfHeight = (height / 2.0f) - epsilon;

		// Test X independently
		float newX = x + deltaX;
		std::pair<int, int> Tile1 = Griddy.getIndex(newX - halfWidth, y + halfHeight);
		std::pair<int, int> Tile2 = Griddy.getIndex(newX + halfWidth, y + halfHeight);
		std::pair<int, int> Tile3 = Griddy.getIndex(newX - halfWidth, y - halfHeight);
		std::pair<int, int> Tile4 = Griddy.getIndex(newX + halfWidth, y - halfHeight);

		bool canMoveX =
			Griddy.getTileID(Tile1.first, Tile1.second) != World::Wall &&
			Griddy.getTileID(Tile2.first, Tile2.second) != World::Wall &&
			Griddy.getTileID(Tile3.first, Tile3.second) != World::Wall &&
			Griddy.getTileID(Tile4.first, Tile4.second) != World::Wall;
		if (canMoveX) x = newX;

		// Test Y independently
		float newY = y + deltaY;
		std::pair<int, int> TileA = Griddy.getIndex(x - halfWidth, newY + halfHeight);
		std::pair<int, int> TileB = Griddy.getIndex(x + halfWidth, newY + halfHeight);
		std::pair<int, int> TileC = Griddy.getIndex(x - halfWidth, newY - halfHeight);
		std::pair<int, int> TileD = Griddy.getIndex(x + halfWidth, newY - halfHeight);

		bool canMoveY =
			Griddy.getTileID(TileA.first, TileA.second) != World::Wall &&
			Griddy.getTileID(TileB.first, TileB.second) != World::Wall &&
			Griddy.getTileID(TileC.first, TileC.second) != World::Wall &&
			Griddy.getTileID(TileD.first, TileD.second) != World::Wall;
		if (canMoveY) y = newY;

		setPosition(x, y);

		//// update holding sword status
		//if (inventory[selectedInventorySlot] != nullptr) {
		//	int itemId = (*inventory)->getID();
		//	Settings::gHoldingCarrot = itemId == Inventory::ItemID::CARROT_SWORD_INV;
		//	printf("Holding: %d\n", itemId);
		//}
	}

	// ==================================== additional (for collision)

	void Player::handleMovement(LevelSystem::Level& level) {
		movingDirections[0] = AEInputCheckCurr(AEVK_W); // up
		movingDirections[1] = AEInputCheckCurr(AEVK_S); // down
		movingDirections[2] = AEInputCheckCurr(AEVK_A); // left
		movingDirections[3] = AEInputCheckCurr(AEVK_D); // right

		float deltaX = 0.0f;
		float deltaY = 0.0f;

		if (movingDirections[0]) deltaY += speed;
		if (movingDirections[1]) deltaY -= speed;
		if (movingDirections[2]) deltaX -= speed;
		if (movingDirections[3]) deltaX += speed;

		// Normalize diagonal movement
		if (deltaX != 0.0f && deltaY != 0.0f) {
			float length = sqrtf(deltaX * deltaX + deltaY * deltaY);
			deltaX = (deltaX / length) * speed;
			deltaY = (deltaY / length) * speed;
		}

		// --------------------- Collision stuffs ------------------------

		int colX = level.checkBinaryCollision(x + deltaX, y, width, height);
		if (!(colX & (LevelSystem::Level::COLLISION_LEFT | LevelSystem::Level::COLLISION_RIGHT))) {
			x += deltaX;
		}

		int colY = level.checkBinaryCollision(x, y + deltaY, width, height);
		if (!(colY & (LevelSystem::Level::COLLISION_TOP | LevelSystem::Level::COLLISION_BOTTOM))) {
			y += deltaY;
		}

		setPosition(x, y);

	}


	void Player::update() {
		handleMovement();
	}

	void Player::update(World::worldGrid& Griddy)
	{
		if(!this->isDead)
		handleMovement(Griddy);
	}

	void Player::update(LevelSystem::Level& level) {
		handleMovement(level);
	}

	void Player::draw() {
		// this + override made me add a player update im so sorry
		// handleMovement();

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		// Apply stroke and fill
		Color::stroke(0, 0, 0, 255);
		Color::strokeWeight(2.0f);
		Color::fill(0, 0, 255, 255);

		//Shapes::rect(x, y, width, height, Shapes::CENTER);

		if (this->isWalking)
		{
			if (movingDirections[0])
			{
				this->setWalkAnimation("playerUp", "playerSpritesheet", 0, 3, 4, 0.4f, true);
			}
			else if (movingDirections[1])
			{
				this->setWalkAnimation("playerDown", "playerSpritesheet", 0, 0, 4, 0.4f, true);
			}

			else if (movingDirections[2])
			{
				this->setWalkAnimation("playerLeft", "playerSpritesheet", 0, 1, 4, 0.4f, true);
			}
			else if (movingDirections[3])
			{
				this->setWalkAnimation("playerRight", "playerSpritesheet", 0, 2, 4, 0.4f, true);
			}

			SpriteManager::drawAnimation(*(this->walkAnimation), this->x, this->y, this->width, this->height);
		}

		else
			SpriteManager::drawSprite(
				SpriteManager::createSprite(this->spriteSheetName, 0, 0),
				this->x, this->y, this->width, this->height
			);


		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			int currSlot = this->getSelectedInventorySlot(); // make sure you're actually calling this
			Inventory::Item* selectedItem = this->getInventoryItem(currSlot);
			
			// Check if currently selected item is carrot sword
			if (selectedItem != nullptr && selectedItem->getID() == Inventory::ItemID::CARROT_SWORD_INV)
			{
				if (!isAttacking) {
					isAttacking = true;
					attackTimer = 0.0f; // reset timer when attack starts
				}
			}
		}

		float deltaTime = (float)AEFrameRateControllerGetFrameTime();

		// Making it so the animation plays for 3 seconds
		if (isAttacking)
		{
			attackTimer += deltaTime;

			SpriteManager::drawAnimationAtTime(*(this->attackAnimation), attackTimer,
				this->x + (this->facingDirection ? this->width : -this->width) * 0.5f, this->y,
				this->facingDirection ? this->width : -this->width, this->height);

			if (attackTimer >= attackDuration) {
				isAttacking = false; // stop after full duration
			}
		}
;
		// Draw health bar if HP is less than max
		if (hp < maxHp) {
			drawHealthBar();
		}
	}



	// -------------------------------------------------------------------------
	// Enemy Implementation
	// -------------------------------------------------------------------------

	Enemy::Enemy(float posX, float posY, float w, float h, float health, float spd, float arm)
		: Entity(posX, posY, w, h, health, spd, arm),
		fov(300.0f), deltaX(0.0f), deltaY(0.0f),
		walking(false), endAtX(0.0f), endAtY(0.0f), wait(2.0f), waited(0.0f) {

		this->isDead = false;

		// default skin
		this->sprite = AEGfxTextureLoad("Assets/Images/Entities/Enemies/crab.png");

	}

	Enemy::Enemy(void)
		: Enemy(0.0f, 0.0f, 50.0f, 50.0f, 100.0f, 1.0f, 0.0f) {}

	void Enemy::setFov(float newFov) {
		fov = newFov;
	}

	float Enemy::getFov() const {
		return fov;
	}


	void Enemy::movement(const Player& player, float deltaTime, World::worldGrid& Griddy)
	{
		bool playerSpotted = Collision::collidedWith(
			x, y,
			player.getX(), player.getY(),
			fov,
			player.getWidth(), player.getHeight()
		);

		float deltaX = 0.0f;
		float deltaY = 0.0f;

		if (!playerSpotted) {
			// Wander behavior
			if (!walking) {
				Path::Vect set = {0,0};
				endAtX = x + set.x;
				endAtY = y + set.y;
				walking = true;
				waited = 0.0f;
			}
			float dirX = endAtX - x;
			float dirY = endAtY - y;
			float dist = sqrtf(dirX * dirX + dirY * dirY);
			if (dist > speed) {
				deltaX = (dirX / dist) * speed;
				deltaY = (dirY / dist) * speed;
			}
			else {
				waited += deltaTime;
				if (waited >= wait) {
					walking = false;
					waited = 0.0f;
				}
			}
		}
		else {
			// Chase player
			float diffX = player.getX() - x;
			float diffY = player.getY() - y;
			float dist = sqrtf((diffX * diffX) + (diffY * diffY));
			if (dist > 1.0f) {
				diffX /= dist;
				diffY /= dist;
			}
			const float stopDistance = 70.0f;
			if (dist >= stopDistance) {
				deltaX = diffX * speed;
				deltaY = diffY * speed;
			}
		}

		// Grid collision — same corner-check pattern as Player
		float halfWidth = width / 2.0f;
		float halfHeight = height / 2.0f;

		// Test X independently
		float newX = x + deltaX;
		std::pair<int, int> Tile1 = Griddy.getIndex(newX - halfWidth, y + halfHeight);
		std::pair<int, int> Tile2 = Griddy.getIndex(newX + halfWidth, y + halfHeight);
		std::pair<int, int> Tile3 = Griddy.getIndex(newX - halfWidth, y - halfHeight);
		std::pair<int, int> Tile4 = Griddy.getIndex(newX + halfWidth, y - halfHeight);
		bool canMoveX =
			Griddy.getTileID(Tile1.first, Tile1.second) != World::Wall &&
			Griddy.getTileID(Tile2.first, Tile2.second) != World::Wall &&
			Griddy.getTileID(Tile3.first, Tile3.second) != World::Wall &&
			Griddy.getTileID(Tile4.first, Tile4.second) != World::Wall;

		if (canMoveX) {
			x = newX;
		}

		else {
			// Wall hit on X — flip target and pick new wander point next frame
			endAtX = x - (endAtX - x); // mirror target
			walking = false;            // force new random point next frame
		}

		// Test Y independently
		float newY = y + deltaY;
		std::pair<int, int> TileA = Griddy.getIndex(x - halfWidth, newY + halfHeight);
		std::pair<int, int> TileB = Griddy.getIndex(x + halfWidth, newY + halfHeight);
		std::pair<int, int> TileC = Griddy.getIndex(x - halfWidth, newY - halfHeight);
		std::pair<int, int> TileD = Griddy.getIndex(x + halfWidth, newY - halfHeight);
		bool canMoveY =
			Griddy.getTileID(TileA.first, TileA.second) != World::Wall &&
			Griddy.getTileID(TileB.first, TileB.second) != World::Wall &&
			Griddy.getTileID(TileC.first, TileC.second) != World::Wall &&
			Griddy.getTileID(TileD.first, TileD.second) != World::Wall;

		if (canMoveY) {
			y = newY;
		}

		else {
			// Wall hit on Y — same treatment
			endAtY = y - (endAtY - y); // mirror target
			walking = false;            // force new random point next frame
		}

		setPosition(x, y);
	}




	void Enemy::draw(Player& player, World::worldGrid& Griddy, bool pause) {
		
		// Pause movement when on pause screen
		if (!pause)
		{
			movement(player, static_cast<float>(AEFrameRateControllerGetFrameTime()), Griddy);
		}

		if (this->isDead)
		{
			return;
		}

		static bool giveItem = false;

		// Apply stroke and fill
		if (this->sprite == nullptr) {
			Color::stroke(0, 0, 0, 255);
			Color::strokeWeight(2.0f);
			Color::fill(255, 0, 0, 255);

			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			Shapes::rect(x, y, width, height, Shapes::CENTER);
		}

		else {
			Color::fill(Color::Preset::White);
			Graphics::image(this->x, this->y, this->width, this->height, this->sprite, Shapes::CENTER);
		}

		if (this->getHp() <= 0 && !this->isDead)
		{
			this->isDead = true;
			gParticles.spawnExplosion(this->getX(), this->getY(), 40);
			AudioManager::audio.playSFX(1.0f);

			player.giveItem(Inventory::ItemID::CARROT_SEEDS, 1);  // drop item once on death
		}

		// Always draw health bar for enemies
		drawHealthBar();
	}

	// -------------------------------------------------------------------------
	// NPC Implementation
	// -------------------------------------------------------------------------

	NPC::NPC(float posX, float posY, float w, float h, float health, float spd, float arm)
		: Entity(posX, posY, w, h, health, spd, arm) {
	}

	NPC::NPC()
		: Entity() {
	}


	// Dialogue lines
	const std::vector<std::string>& NPC::getDialogLines() const {
		return dialogLines;
	}

	void NPC::setDialogLines(const std::vector<std::string>& lines) {
		dialogLines = lines;
	}

	void NPC::addDialogLine(const std::string& line) {
		dialogLines.push_back(line);
	}


	// Idle lines
	const std::vector<std::string>& NPC::getIdleLines() const {
		return idleLines;
	}

	void NPC::setIdleLines(const std::vector<std::string>& lines) {
		idleLines = lines;
	}

	void NPC::addIdleLine(const std::string& line) {
		idleLines.push_back(line);
	}


	size_t NPC::getLineNum() {
		return linenum;
	}

	size_t NPC::getIdleNum() {
		return idlenum;
	}

	bool NPC::getIsPaused() {
		return isPaused;
	}

	bool NPC::getIsIdling() {
		return idling;
	}

	void NPC::setIdling(bool value) {
		idling = value;
	}

	bool NPC::getConditionTrue() {
		return conditionTrue;
	}

	void NPC::setConditionTrue() {
		conditionTrue = true;
	}


	void NPC::resumeDialogue(UI_Elements::DialogueBox& dialogueBox) {
		if (!isPaused) {
			return;
		}

		isPaused = false;
		++linenum;

		if (linenum >= dialogLines.size()) {
			return;
		}

		dialogueBox.setSpeaker(this->charName);
		dialogueBox.setCharacterSprite(this->sprite);
		dialogueBox.setText(dialogLines[linenum]);
		dialogueBox.activate();
		
	}

	// sorry for the mess, i'll change it and fix the code duplication after if i have time (speak & idleSpeak)
	void NPC::speak(UI_Elements::DialogueBox& dialogueBox) {

		if (dialogLines.empty()) {
			return;
		}

		if (dialogueBox.getIsActive()) {
			if (AEInputCheckTriggered(AEVK_E) || AEInputCheckTriggered(AEVK_LBUTTON)) {
				++linenum;

				if (linenum >= dialogLines.size()) {

					dialogueBox.dismiss();
					linenum = 0;
					started = false;
					return;
				}

				if (dialogLines[linenum] == "@") {
					isPaused = true;
					dialogueBox.dismiss();
					return;
				}

				dialogueBox.setText(dialogLines[linenum]);
				
			}

			return;
		}

		if (!started) {
			started = true;
			linenum = 0;

			dialogueBox.setSpeaker(this->charName);
			dialogueBox.setCharacterSprite(this->sprite);
			dialogueBox.setText(dialogLines[linenum]);
			dialogueBox.activate();
		}
	}

	// code modularity might just be my biggest enemy :heartbroken:
	void NPC::idleSpeak(UI_Elements::DialogueBox& dialogueBox) {
		if (idleLines.empty()) {
			return;
		}

		if (dialogueBox.getIsActive()) {
			if (AEInputCheckTriggered(AEVK_E) || AEInputCheckTriggered(AEVK_LBUTTON)) {
				++idlenum;

				if (idlenum >= idleLines.size()) {

					dialogueBox.dismiss();
					idlenum = 0;
					idling = false;
					return;
				}

				if (idleLines[idlenum] == "#") {
					dialogueBox.dismiss();
					++idlenum;
					return;
				}

				dialogueBox.setText(idleLines[idlenum]);

			}

			return;
		}

		if (idling && idlenum < idleLines.size()) {
			idling = true;

			dialogueBox.setSpeaker(this->charName);
			dialogueBox.setCharacterSprite(this->sprite);
			dialogueBox.setText(idleLines[idlenum]);
			dialogueBox.activate();
			return;
		}

		if (!idling) {
			idling = true;
			idlenum = 0;

			dialogueBox.setSpeaker(this->charName);
			dialogueBox.setCharacterSprite(this->sprite);
			dialogueBox.setText(idleLines[idlenum]);
			dialogueBox.activate();
		}
	}

	bool NPC::dialogueDone()
	{
		return started && linenum >= dialogLines.size();
	}

	void NPC::draw() {
		bool graphicalDraw = (this->sprite != nullptr);

		if (graphicalDraw) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			Graphics::image(x, y, width, height, sprite, Shapes::CENTER);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			// Apply stroke and fill
			Color::stroke(0, 0, 0, 255);
			Color::strokeWeight(2.0f);
			Color::fill(0, 255, 0, 255);

			Shapes::rect(x, y, width, height, Shapes::CENTER);
		}


		// NPCs typically don't show health bars unless damaged
		if (hp < maxHp) {
			drawHealthBar();
		}
	}
}

