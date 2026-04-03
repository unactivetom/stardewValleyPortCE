#include <graphx.h>
#include <keypadc.h>
#include "gfx/gfx.h"

extern unsigned char tilemap_map[]; // include tilemap data


#define STD_SPRITE_WIDTH 16
#define STD_SPRITE_HEIGHT 32

#define START_X ((GFX_LCD_WIDTH - STD_SPRITE_WIDTH) / 2)
#define START_Y ((GFX_LCD_HEIGHT - STD_SPRITE_HEIGHT) / 2)

#define TILEMAP_HEIGHT 15
#define TILEMAP_WIDTH 20

typedef struct entity {
	int x;
	int y;
	int old_x;
	int old_y;
	int speed;
	gfx_sprite_t* sprite;
	bool is_moving;
	int direction;
	int currentSprite;
	bool sprint;
} Entity;

void begin(void);
void end(void);
bool should_loop(void);
void draw(void);
void logic(void);
void update_player(void);
void draw_sprite(Entity* entity);
void animate_sprite3(Entity* entity, gfx_sprite_t* sprite_array[4][3], int speed);
void animate_sprite2(Entity* entity, gfx_sprite_t* sprite_array[4][2], int speed);

Entity player;

// Create a buffer for what's behind a sprite
gfx_sprite_t* behind_sprite;
gfx_tilemap_t basic_tilemap;
int counter;

gfx_sprite_t* alex[4][4] =
{
	{alex_forward1, alex_forward2, alex_forward3, alex_forward4},
	{alex_right1, alex_right2, alex_right3, alex_right4},
	{alex_back1, alex_back2, alex_back3, alex_back4},
	{alex_left1, alex_left2, alex_left3, alex_left4}
};

gfx_sprite_t* farmer_walk[4][3] =
{
	{walk_front1, walk_front2, walk_front3},
	{walk_right1, walk_right2, walk_right3},
	{walk_back1, walk_back2, walk_back3},
	{walk_left1, walk_left2, walk_left3}
};

gfx_sprite_t* farmer_sprint[4][2] =
{
	{run_front1, run_front2},
	{run_right1, run_right2},
	{run_back1, run_back2},
	{run_left1, run_left2}
};


int main(void) {
	begin();

	while (should_loop()) {
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
	player.sprite = alex_forward1;
	player.is_moving = false;
	player.direction = 0;
	player.currentSprite = 0;
	player.sprint = false;


	// init tilemap
	basic_tilemap.map = tilemap_map;
	basic_tilemap.tiles = basic_grass_tileset_tiles;
	basic_tilemap.type_width = gfx_tile_16_pixel;
	basic_tilemap.type_height = gfx_tile_16_pixel;
	basic_tilemap.tile_height = 16;
	basic_tilemap.tile_width = 16;
	basic_tilemap.draw_height = 15;
	basic_tilemap.draw_width = 20;
	basic_tilemap.height = TILEMAP_HEIGHT;
	basic_tilemap.width = TILEMAP_WIDTH;
	basic_tilemap.y_loc = 0;
	basic_tilemap.x_loc = 0;


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
	behind_sprite = gfx_MallocSprite(STD_SPRITE_WIDTH, STD_SPRITE_HEIGHT);

	counter = 0;

}

void end(void) {
	gfx_End();
}

bool should_loop(void) {

	return kb_IsDown(kb_KeyClear) == 0;
}

void draw(void) {

	gfx_Tilemap(&basic_tilemap, 0, 0);

	draw_sprite(&player);


	gfx_FillRectangle(0, 0, 320, 16);
	gfx_PrintStringXY("x:", 64, 4);
	gfx_PrintInt(counter, 1);


}

void logic(void) {
	kb_Scan();

	update_player();

	if (!player.sprint) animate_sprite3(&player, farmer_walk, 4);
	else animate_sprite2(&player, farmer_sprint, 2);

	counter++;

}

void update_player(void) {

	if (kb_Data[1] & kb_2nd) player.speed = 4;
	else player.speed = 2;

	int xChange = 0, yChange = 0;

	// Update player position
	int arrow_keys = kb_Data[7];
	if (arrow_keys & kb_Up) { yChange -= player.speed; player.direction = 2; }
	if (arrow_keys & kb_Down) { yChange += player.speed; player.direction = 0; }
	if (arrow_keys & kb_Left) { xChange -= player.speed; player.direction = 3; }
	if (arrow_keys & kb_Right) { xChange += player.speed; player.direction = 1; }

	if (xChange != 0 || yChange != 0) player.is_moving = true;
	else player.is_moving = false;

	player.x += xChange;
	player.y += yChange;


	// Keep player in bounds
	if (player.x < 0) player.x = 0;
	if (player.x > GFX_LCD_WIDTH - STD_SPRITE_WIDTH) player.x = GFX_LCD_WIDTH - STD_SPRITE_WIDTH;
	if (player.y < 0) player.y = 0;
	if (player.y > GFX_LCD_HEIGHT - STD_SPRITE_HEIGHT) player.y = GFX_LCD_HEIGHT - STD_SPRITE_HEIGHT;


}

void draw_sprite(Entity* entity) {

	/* Render the original background */
	gfx_Sprite(behind_sprite, entity->old_x, entity->old_y);

	/* Get the background behind the sprite */
	gfx_GetSprite(behind_sprite, entity->x, entity->y);

	// Draw the sprite
	gfx_TransparentSprite(entity->sprite, entity->x, entity->y);

	// Update old position for drawing the background
	entity->old_x = entity->x;
	entity->old_y = entity->y;
}

void animate_sprite3(Entity* entity, gfx_sprite_t* sprite_array[4][3], int speed) {

	if (entity->is_moving) {
		if (counter % speed == 0) {
			int target_frame = entity->currentSprite += 1;
			if (target_frame == 3) {
				target_frame = 0;
				entity->currentSprite = 0;
			}
			entity->sprite = sprite_array[entity->direction][target_frame];

		}

	}
	else entity->sprite = sprite_array[entity->direction][0];

}
void animate_sprite2(Entity* entity, gfx_sprite_t* sprite_array[4][2], int speed) {

	if (entity->is_moving) {
		if (counter % speed == 0) {
			int target_frame = entity->currentSprite += 1;
			if (target_frame == 2) {
				target_frame = 0;
				entity->currentSprite = 0;
			}
			entity->sprite = sprite_array[entity->direction][target_frame];

		}

	}
	else entity->sprite = sprite_array[entity->direction][0];

}


/*
Create new thing:

Entities:
1. Add the png files to gfx folder
2. Add the data to convimg.yaml
3. Run make gfx
4. Make a new struct heriting from Entity struct.
5. Implement code.

Tilemaps:
1. Check if you use the right tileset
2. Add .csv file to tilemap folder
3. Add to makefile
4. Create new gfx_tilemap_t and init variables in begin()


IMPORTANT NOTES:
-animations can only have a certain amount of frames, or function overloading (multiple versions of the same function)



*/