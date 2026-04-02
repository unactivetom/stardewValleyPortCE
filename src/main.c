#include <graphx.h>
#include <keypadc.h>

void begin(void);
void end(void);
bool should_loop(void);
void draw(void);
void logic(void);




int main(void) {
	begin();
	
	while(should_loop()) {
		logic();
		gfx_BlitScreen(); // Draw to the back buffer.
		draw();
		gfx_SwapDraw(); // Swap the back buffer to the front.
	}
	end();

	return 0;
	
}

void begin(void) {
	gfx_Begin();
	kb_SetMode(MODE_3_CONTINUOUS);
	kb_Scan();

	//gfx_SetPalette(global_palette, sizeof_global_palette, 0);
	gfx_SetColor(1);
	gfx_SetTextFGColor(2);
	gfx_SetTextBGColor(1);
	gfx_SetTransparentColor(0);

	gfx_SetDrawBuffer();

}

void end(void) {
	gfx_End();
}

bool should_loop(void) {

	return kb_IsDown(kb_KeyClear) == 0; // Loop until the Clear key is pressed.
}

void draw(void) {

}

void logic(void) {
	kb_Scan();
}