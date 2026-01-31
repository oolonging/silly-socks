#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include <string>
#include "Graphics.hpp"

namespace UI_Elements {
	class Button {
		private:
			float x{};
			float y{};
			float width{};
			float height{};
			Shapes::SHAPE_MODE drawMode;
			char const* msg{};

		public:
			bool isHovering(void) {
				int32_t mx = 0, my = 0;
				AEInputGetCursorPosition(&mx, &my);

				int32_t ww = AEGfxGetWindowWidth();
				int32_t wh = AEGfxGetWindowHeight();
				float worldX = mx - ww * 0.5f;
				float worldY = wh * 0.5f - my;

				bool xOverlap{}, yOverlap{};

				if (Button::drawMode == Shapes::CORNER) {
					xOverlap = ((worldX >= Button::x) && (worldX <= (Button::x + Button::width)));
					yOverlap = ((worldY <= Button::y) && (worldY >= (Button::y - Button::height)));
				}
				else if (Button::drawMode == Shapes::CENTER) {
					xOverlap = ((worldX >= (Button::x - Button::width / 2)) && (worldX <= (Button::x + Button::width / 2)));
					yOverlap = ((worldY >= (Button::y - Button::height / 2)) && (worldY <= (Button::y + Button::height / 2)));
				}

				return (xOverlap && yOverlap);
			}

			void draw(void) {
				bool isHovering = Button::isHovering();

				if (isHovering) {
					Color::fill(255, 0, 0);
				}
				else {
					Color::fill(0, 255, 0);
				}

				Shapes::rect(Button::x, Button::y, Button::width, Button::height, Button::drawMode);


				if (isHovering) {
					Color::textFill(0, 0, 0);
				}
				else {
					Color::textFill(255, 255, 255);
				}


				if (drawMode == Shapes::CORNER) {
					//float textX = Button::x + width * 0.5f;
					//float textY = Button::y - height * 0.5f;
					//Text::text(msg, textX, textY);

					Text::text(msg, 0, 0);
				}
				else {
					float textX = Button::x;
					float textY = Button::y;
					Text::text(msg, textX, textY);

				}

			}

			// Constructors

			Button(float x, float y, float width, float height, char const* msg, Shapes::SHAPE_MODE mode = Shapes::CORNER)
				: x(x), y(y), width(width), height(height), drawMode(mode), msg(msg) {
			}

			Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode = Shapes::CORNER)
				: x(quad.position.x), y(quad.position.y), width(quad.width), height(quad.height), drawMode(mode), msg(msg) {
			}

			Button(void)
				: x(0), y(0), width(100), height(50), drawMode(Shapes::CORNER) {
			}
	};
}


#endif // UI_ELEMENTS_HPP