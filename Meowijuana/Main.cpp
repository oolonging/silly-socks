#include <crtdbg.h>
#include "AEEngine.h"
#include "Math.hpp"
#include "Graphics.hpp"
#include "UI_Elements.hpp"

class Player {
    public:
        float x;
        float y;
        float size;
        float speed;
        float hp = 100.0f;

        void draw() {
            Color::fill(241, 247, 237);
			Shapes::ellipse(Player::x, Player::y, Player::size, Player::size, Shapes::CENTER);
        }

        void handleMovement() {
            if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP)) {
                Player::y += Player::speed;
            }
            if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT)) {
                Player::x -= Player::speed;
            }
            if (AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN)) {
                Player::y -= Player::speed;
            }
            if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT)) {
                Player::x += Player::speed;
            }

            //constrict hp
            if (Player::hp < 0) Player::hp = 0;
            if (Player::hp > 100) Player::hp = 100;

			// wrap around screen
			int32_t ww = AEGfxGetWindowWidth();
			int32_t wh = AEGfxGetWindowHeight();

            if (Player::x > ww / 2) Player::x = (float) (-ww / 2);
            if (Player::x < -ww / 2) Player::x = (float) (ww / 2);
            if (Player::y > wh / 2) Player::y = (float) (-wh / 2);
            if (Player::y < -wh / 2) Player::y = (float) (wh / 2);
        }

        Player (float x, float y, float size, float speed) {
            Player::x = x;
            Player::y = y;
            Player::size = size;
            Player::speed = speed;
		}
};


Player player(0.0f, 0.0f, 75.0f, 5.0f);


// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    int gGameRunning = 1;

    AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);
    AESysSetWindowTitle("Silly Socks | Meowijuana");

    // initialize shapes helper
    Shapes::init();



    while (gGameRunning)
    {
        // Informing the system about the loop's start
        AESysFrameStart();

        // Basic way to trigger exiting the application
        // when ESCAPE is hit or when the window is closed
        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = 0;


        // reset background
        AEGfxSetBackgroundColor(84.0f / 255.0f, 73.0f / 255.0f, 75.0f / 255.0f);

        // Tell the engine to get ready to draw something with texture.
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);

        // Set the the color to multiply to white, so that the sprite can 
        // display the full range of colors (default is black).
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

        // Set the color to add to nothing, so that we don't alter the sprite's color
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        // Set blend mode to AE_GFX_BM_BLEND
        // This will allow transparency.
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);


        // get mouse in screen pixels (0,0 = top-left)
        int32_t mx = 0, my = 0;
        AEInputGetCursorPosition(&mx, &my);

        // convert to world coords (0,0 = window center, +y up)
        int32_t ww = AEGfxGetWindowWidth();
        int32_t wh = AEGfxGetWindowHeight();
        float worldX = mx - ww * 0.5f;
        float worldY = wh * 0.5f - my;

        //test rects
        Color::fill(0, 0, 255);
		Shapes::rect(worldX, worldY, 50.0f, 50.0f, Shapes::CENTER);
        Color::fill(255, 0, 0);
        Shapes::rect(worldX, worldY, 50.0f, 50.0f, Shapes::CORNER);



		// draw the player
		player.draw();
        player.handleMovement();


        // Informing the system about the loop's end
        AESysFrameEnd();
    }

    // shutdown shapes helper
    Shapes::exit();

    // free the system
    AESysExit();
}
