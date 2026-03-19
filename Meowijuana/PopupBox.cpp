#include "../pch.h"
#include "UI_Elements.hpp"

namespace UI_Elements {
    // -------------------------------------------------------------------------
    // PopupBox Implementation
    // -------------------------------------------------------------------------
    PopupBox::PopupBox(float x, float y, float width, float height, char const* title, char const* message, bool initialState, Shapes::SHAPE_MODE mode)
        : UI_Element(x, y, width, height, mode), isVisible(initialState), title(title), message(message), width(width), height(height), onConfirm(nullptr), onDismiss(nullptr) {
        this->style = getDefaultStyle();
        this->textStyle = getDefaultTextStyle();
    }
    PopupBox::PopupBox(void)
        : PopupBox(0.0f, 0.0f, 200.0f, 120.0f, "Title", "Default popup message") {
    }

    void PopupBox::show() {
        isVisible = true;
    }
    void PopupBox::hide() {
        isVisible = false;
    }
    bool PopupBox::getVisible() const {
        return isVisible;
    }
    void PopupBox::setVisible(bool visible) {
        isVisible = visible;
    }
    void PopupBox::setTitle(char const* t) {
        title = t;
    }
    void PopupBox::setMessage(char const* msg) {
        message = msg;
    }
    void PopupBox::setOnConfirm(void (*func)(void)) {
        onConfirm = func;
    }
    void PopupBox::setOnDismiss(void (*func)(void)) {
        onDismiss = func;
    }

    void PopupBox::draw(void) {
        if (!isVisible) return;

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);

        // Compute center and button regions
        float centerX = (drawMode == Shapes::CORNER) ? (this->x + this->width * 0.5f) : this->x;
        float centerY = (drawMode == Shapes::CORNER) ? (this->y - this->height * 0.5f) : this->y;

        // Button dimensions
        float btnWidth = this->width * 0.35f;
        float btnHeight = this->height * 0.18f;
        float btnY = centerY - this->height * 0.5f + btnHeight;

        float confirmX = centerX - btnWidth * 0.6f;
        float dismissX = centerX + btnWidth * 0.6f;

        // Hit-test helpers (CENTER mode)
        auto inRect = [](float mx, float my, float rx, float ry, float rw, float rh) {
            return mx >= rx - rw * 0.5f && mx <= rx + rw * 0.5f &&
                my >= ry - rh * 0.5f && my <= ry + rh * 0.5f;
            };

        AEVec2 mouse{};
        AEInputGetCursorPosition(&mouse.x, &mouse.y); // adjust to your project's mouse API

        bool confirmHovered = inRect(mouse.x, mouse.y, confirmX, btnY, btnWidth, btnHeight);
        bool dismissHovered = inRect(mouse.x, mouse.y, dismissX, btnY, btnWidth, btnHeight);

        // Handle confirm click
        if (confirmHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (onConfirm != nullptr) onConfirm();
            hide();
        }
        // Handle dismiss click
        if (dismissHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (onDismiss != nullptr) onDismiss();
            hide();
        }

        // ---- Draw popup background ----
        Color::stroke(style.strokeColor);
        Color::strokeWeight(style.strokeWeight);
        Color::fill(style.primaryColor);
        Shapes::rect(this->x, this->y, this->width, this->height, drawMode);

        // ---- Draw title bar ----
        float titleBarHeight = this->height * 0.22f;
        float titleBarY = centerY + this->height * 0.5f - titleBarHeight * 0.5f;
        Color::fill(style.secondaryColor);
        Shapes::rect(centerX, titleBarY, this->width, titleBarHeight, Shapes::CENTER);

        Color::textFill(textStyle.primaryColor);
        Text::textSize(this->textStyle.fontSize);
        Text::text(title, centerX, titleBarY, Text::CENTER, Text::CENTER_V);

        // ---- Draw message ----
        float messageY = centerY + this->height * 0.1f;
        Color::textFill(textStyle.primaryColor);
        Text::textSize(this->textStyle.fontSize);
        Text::text(message, centerX, messageY, Text::CENTER, Text::CENTER_V);

        // ---- Draw Confirm button ----
        Color::stroke(style.strokeColor);
        Color::strokeWeight(style.strokeWeight);
        Color::fill(confirmHovered ? style.secondaryColor : style.primaryColor);
        Shapes::rect(confirmX, btnY, btnWidth, btnHeight, Shapes::CENTER);
        Color::textFill(textStyle.primaryColor);
        Text::textSize(this->textStyle.fontSize);
        Text::text("OK", confirmX, btnY, Text::CENTER, Text::CENTER_V);

        // ---- Draw Dismiss button ----
        Color::fill(dismissHovered ? style.secondaryColor : style.primaryColor);
        Shapes::rect(dismissX, btnY, btnWidth, btnHeight, Shapes::CENTER);
        Color::textFill(textStyle.primaryColor);
        Text::textSize(this->textStyle.fontSize);
        Text::text("Cancel", dismissX, btnY, Text::CENTER, Text::CENTER_V);

        // ---- Debug ----
        Shapes::debugRect(this->x, this->y, this->width, this->height, this->drawMode);
    }
}