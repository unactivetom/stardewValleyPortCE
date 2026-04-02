#include <graphx.h>
#include <keypadc.h>
#include "gfx/gfx.h"

extern unsigned char basic_grasspaths_map_map[]; // include tilemap data


#define START_X ((GFX_LCD_WIDTH - 16) / 2) // 16 NEEDS CHANGING
#define START_Y ((GFX_LCD_HEIGHT - 32) / 2)
#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 32

void begin(void);
void end(void);
bool should_loop(void);
void draw(void);
void logic(void);
void update_player(void);
void draw_sprite(gfx_sprite_t *sprite);


struct entity {
	int x;
	int y;
	int old_x;
	int old_y;
	int speed;
	gfx_sprite_t *sprite;
};

struct entity player;

// Create a buffer for what's behind a sprite
gfx_sprite_t* behind_sprite;
gfx_tilemap_t tilemap;


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

	// init variables
	player.x = START_X;
	player.y = START_Y;
	player.old_x = START_X;
	player.old_y = START_Y;
	player.speed = 3;
	player.sprite = alex_forward_still;

	// init tilemap NEEDS BETTER VARIABLE DECLARATION
	tilemap.map = basic_grasspaths_map_map;
	tilemap.tiles = basic_grass_tileset_tiles;
	tilemap.type_width = gfx_tile_16_pixel;
	tilemap.type_height = gfx_tile_16_pixel;
	tilemap.tile_height = 16;
	tilemap.tile_width = 16;
	tilemap.draw_height = 15;
	tilemap.draw_width = 20;
	tilemap.height = 15;
	tilemap.width = 20;
	tilemap.y_loc = 0;
	tilemap.x_loc = 0;

	// init keypad
	kb_SetMode(MODE_3_CONTINUOUS);
	kb_Scan();

	// init graphics
	gfx_Begin();
	gfx_SetPalette(global_palette, sizeof_global_palette, 0);
	gfx_SetColor(0);
	gfx_SetTextFGColor(1);
	gfx_SetTextBGColor(0);
	gfx_SetTransparentColor(0);

	gfx_SetDrawBuffer();

	// Create and initialize a buffer for behind the sprite.
	behind_sprite = gfx_MallocSprite(SPRITE_WIDTH, SPRITE_HEIGHT);
	
	

}

void end(void) {
	gfx_End();
}

bool should_loop(void) {

	return kb_IsDown(kb_KeyClear) == 0;
}

void draw(void) {
	
	gfx_Tilemap(&tilemap, 0, 0);
	
	draw_sprite(player.sprite);
}

void logic(void) {
	kb_Scan();

	update_player();

}

void update_player(void) {

	int xChange = 0, yChange = 0;
	
	// Update player position
	int arrow_keys = kb_Data[7];
	if (arrow_keys & kb_Up) yChange -= player.speed;
	if (arrow_keys & kb_Down) yChange += player.speed;
	if (arrow_keys & kb_Left) xChange -= player.speed;
	if (arrow_keys & kb_Right) xChange += player.speed;
	
	player.x += xChange;
	player.y += yChange;
	

	// Keep player in bounds
	if (player.x < 0) player.x = 0;
	if (player.x > GFX_LCD_WIDTH - alex_forward_still_width) player.x = GFX_LCD_WIDTH - alex_forward_still_width;
	if (player.y < 0) player.y = 0;
	if (player.y > GFX_LCD_HEIGHT - alex_forward_still_height) player.y = GFX_LCD_HEIGHT - alex_forward_still_height;

	
}

void draw_sprite(gfx_sprite_t *sprite) {
	
	/* Render the original background */
	gfx_Sprite(behind_sprite, player.old_x, player.old_y);

	/* Get the background behind the sprite */
	gfx_GetSprite(behind_sprite, player.x, player.y);

	// Draw the sprite
	gfx_TransparentSprite(sprite, player.x, player.y);

	// Update old position for drawing the background
	player.old_x = player.x;
	player.old_y = player.y;
}

