//#ifndef ATTACK_HPP
//#define ATTACK_HPP
//
//#include "Graphics.hpp"
//#include "Collision.hpp"
//#include "Entity.hpp"
//
//namespace Weapon {
//	class Sword {
//		private:
//			float width;
//			float height;
//			float damage;
//			float speed;
//			float rotation;
//			Shapes::SHAPE_MODE drawMode;
//
//		public:
//
//			float x;
//			float y;
//
//			// Getters
//			float getX() const { return x; }
//			float getY() const { return y; }
//			float getWidth() const { return width; }
//			float getHeight() const { return height; }
//			float getDamage() const { return damage; }
//
//			// Setters
//			void setPosition(float posX, float posY) { x = posX; y = posY; }
//			void setDamage(float dmg) { damage = dmg; }
//
//			// Constructors
//			Sword(float posX, float posY, float w, float h, float dmg, float spd, float rot, Shapes::SHAPE_MODE mode = Shapes::CORNER)
//				: x(posX), y(posY), width(w), height(h), damage(dmg), speed(spd), rotation(rot), drawMode(mode) {
//			}
//			Sword() : x(0), y(0), width(10), height(10), damage(10), speed(0), rotation(0), drawMode(Shapes::CORNER) {
//			}
//
//			void draw(void) {
//				Shapes::Quad weaponQuad = { {x, y}, width, height };
//				Color::fill(150, 75, 0); // Brown color for the weapon
//				Shapes::rect(weaponQuad.position.x, weaponQuad.position.y, weaponQuad.width, weaponQuad.height, drawMode);
//			}
//
//			bool attack(Entity::Entity& target) {
//
//				bool hit = Collision::collidedWith(
//					x, y, 
//					target.getX(), target.getY(), 
//					width, target.getWidth(), target.getHeight()
//				);
//
//				if (hit) {
//					float effectiveDamage = damage - target.getArmor();
//					if (effectiveDamage < 0) effectiveDamage = 0;
//					target.setHp(target.getHp() - effectiveDamage);
//					return true;
//				}
//				return false;
//			}
//
//
//	};
//}
//
//#endif