#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include <string>
#include "Graphics.hpp"

namespace UI_Elements {
	typedef struct {
		Color::CL_Color primaryColor;
		Color::CL_Color secondaryColor;
		Color::CL_Color strokeColor;
		float strokeWeight;
	} ElementStyle;

	typedef struct {
		Color::CL_Color primaryColor;
		Color::CL_Color secondaryColor;
		int fontSize;
		std::string fontPath;
	} TextStyle;


	class Button {
		private:
			float x{};
			float y{};
			float width{};
			float height{};
			char const* msg{};
			Shapes::SHAPE_MODE drawMode;
			void (*onClick)(void) { nullptr };

		public:
			void setOnClick(void (*func)(void)) {
				Button::onClick = func;
			}

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
					// see if clicked
					if(AEInputCheckTriggered(AEVK_LBUTTON)) {
						if(Button::onClick != nullptr) {
							Button::onClick();
						}
					}
					Color::fill(255, 0, 0);
				}
				else {
					Color::fill(0, 255, 0);
				}

				Shapes::rect(Button::x, Button::y, Button::width, Button::height, Button::drawMode);

				// set text to black for now
				Color::textFill(0, 0, 0);


				float textX{};
				float textY{};

				if(Button::drawMode == Shapes::CORNER) {
					textX = Button::x + Button::width / 2;
					textY = Button::y - Button::height / 2;
				}
				else {
					textX = Button::x;
					textY = Button::y;
				}
				
				Text::text(msg, textX, textY, Text::ALIGN_CENTER);

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

	class Slider {
		private:
			float x;
			float y;
			
			float maxValue;		
			float height;
			Shapes::SHAPE_MODE drawMode;
			ElementStyle style{};

		public:
			float minValue;
			float velocity;
			float currValue;
			
			float clampValue(void) {
				if (currValue < minValue) {
					currValue = minValue;
				}
				if (currValue > maxValue) {
					currValue = maxValue;
				}
				return currValue;
			};
			

			void bgDraw(void) {
				clampValue();
				Shapes::rect(Slider::x, Slider::y, maxValue, height, Shapes::CORNER);
			};

			void fgDraw(void) {
				clampValue();
				Shapes::rect(Slider::x, Slider::y, currValue, height, Shapes::CORNER);
			};

			void fgUpdate(float velocity, float deltaTime) {
				clampValue();
				currValue -= velocity * deltaTime;
			};

			Slider(Shapes::Quad quad, Shapes::SHAPE_MODE mode = Shapes::CORNER)
				: x(quad.position.x), y(quad.position.y), minValue(100.0f), maxValue(quad.width), currValue(0.0f), velocity(50.0f), height(quad.height),
				  drawMode(mode), style{} {
			}

			Slider(void)
				: x(0), y(0), minValue(0.0f), maxValue(100), currValue(0.0f), velocity(0.0f), height(50), drawMode(Shapes::CORNER), style{} {
			}
	};
}


#endif // UI_ELEMENTS_HPP