#include "../pch.h"
#include "UI_Elements.hpp"
#include "../InputManager.hpp"

namespace UI_Elements {
    // -------------------------------------------------------------------------
    // PopupBox Implementation
    // -------------------------------------------------------------------------
    PopupBox::PopupBox(float x, float y, float width, float height, char const* title, char const* message, char const* message2, bool initialState, Shapes::SHAPE_MODE mode)
        : UI_Element(x, y, width, height, mode), isVisible(initialState), title(title), message(message), message2(message2), width(width), height(height), onDismiss(nullptr) {
        this->style = getDefaultStyle();
        this->textStyle = getDefaultTextStyle();

        // Compute button positions relative to popup
        float centerX = (mode == Shapes::CORNER) ? (x + width * 0.5f) : x;
        float centerY = (mode == Shapes::CORNER) ? (y - height * 0.5f) : y;

        float btnWidth = width * 0.35f;
        float btnHeight = height * 0.18f;
        float btnY = centerY - height * 0.5f + btnHeight;

        dismissButton = new Button(centerX + btnWidth * 0.5f, btnY, btnWidth, btnHeight, "Close", Shapes::CENTER);
        confirmButton = new Button(centerX - btnWidth * 0.5f, btnY, btnWidth, btnHeight, "Confirm", Shapes::CENTER);
    }

    //default
    PopupBox::PopupBox(void)
        : PopupBox(0.0f, 0.0f, 200.0f, 120.0f, "Title", "Default popup message", " ") {
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
    void PopupBox::setOnDismiss(void (*func)(void)) {
        onDismiss = func;
        dismissButton->setOnClick(func);
    }

    void PopupBox::setOnconfirm(void (*func)(void)) {
        onConfirm = func;
        confirmButton->setOnClick(func);
    }

    void PopupBox::draw(void) {
        if (!isVisible) return;

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);

        float centerX = (drawMode == Shapes::CORNER) ? (this->x + this->width * 0.5f) : this->x;
        float centerY = (drawMode == Shapes::CORNER) ? (this->y - this->height * 0.5f) : this->y;

        // popup bg
        Color::stroke(style.strokeColor);
        Color::strokeWeight(style.strokeWeight);
        Color::fill(style.primaryColor);
        Shapes::rect(this->x, this->y, this->width, this->height, drawMode);

        textStyle.fontSize = 15.0f;

        // title bar
        float titleBarHeight = this->height * 0.22f;
        float titleBarY = centerY + this->height * 0.5f - titleBarHeight * 0.5f;
        Color::fill(style.secondaryColor);
        Shapes::rect(centerX, titleBarY, this->width, titleBarHeight, Shapes::CENTER);

        Color::textFill(textStyle.primaryColor);
        Text::textSize(this->textStyle.fontSize);
        Text::setFont(this->textStyle.fontName);
        Text::text(title, centerX, titleBarY, Text::CENTER_H, Text::CENTER_V);

        // message
        float messageY = centerY + this->height * 0.1f;
        Color::textFill(textStyle.primaryColor);
        Text::textSize(this->textStyle.fontSize);
        Text::text(message, centerX, messageY, Text::CENTER_H, Text::CENTER_V);
        Text::text(message2, centerX, messageY - 30.0f, Text::CENTER_H, Text::CENTER_V);

        // draw button
        if(onDismiss != nullptr) dismissButton->draw();
        if(onConfirm != nullptr) confirmButton->draw();

        // debug
        Shapes::debugRect(this->x, this->y, this->width, this->height, this->drawMode);
    }
}