
#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include "Graphics.hpp"

namespace UI_Elements {
	class Button {
	private:
		float x{};
		float y{};
		float width{};
		float height{};
		Shapes::SHAPE_MODE drawMode = Shapes::CORNER;
		// void function pointer for onClick event

	public:
		bool hovering(void) {
			int32_t mx = 0, my = 0;
			AEInputGetCursorPosition(&mx, &my);

			int32_t ww = AEGfxGetWindowWidth();
			int32_t wh = AEGfxGetWindowHeight();
			float worldX = mx - ww * 0.5f;
			float worldY = wh * 0.5f - my;

			bool xOverlap{}, yOverlap{};

			if (Button::drawMode == Shapes::CORNER) {
				xOverlap = ((worldX >= Button::x) && (worldX <= (Button::x + Button::width)));
				yOverlap = ((worldY >= Button::y) && (worldY <= (Button::y + Button::height)));
			}
			else if (Button::drawMode == Shapes::CENTER) {
				xOverlap = ((worldX >= (Button::x - Button::width / 2)) && (worldX <= (Button::x + Button::width / 2)));
				yOverlap = ((worldY >= (Button::y - Button::height / 2)) && (worldY <= (Button::y + Button::height / 2)));
			}

			return (xOverlap && yOverlap);
		}

		void draw(void) {
			bool isHovering = Button::hovering();

			if (isHovering) {
				Color::fill(255, 0, 0);
			}
			else {
				Color::fill(0, 255, 0);
			}

			Shapes::rect(Button::x, Button::y, Button::width, Button::height, Button::drawMode);
		}

		Button(float x, float y, float width, float height) {
			Button::x = x;
			Button::y = y;
			Button::width = width;
			Button::height = height;
		}

		Button(Shapes::Quad button) {
			Button::x = button.position.x;
			Button::y = button.position.y;
			Button::width = button.width;
			Button::height = button.height;
		}
	};
}

#endif // UI_ELEMENTS_HPP
