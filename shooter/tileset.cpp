#include "tileset.h"
#include "shooter_constants.h"

// dimensions
const int MAP_HEIGHT = WINDOW_HEIGHT / TILE_SIZE_PX + 1 ; // number of rows visible on screen

const int MAP_WIDTH_PX = LEVEL_WIDTH_PX;
const int MAP_HEIGHT_PX = MAP_HEIGHT * TILE_SIZE_PX;

// positions graphics inside bmp
const int GROUND_TILE_0_X = 554;
const int GROUND_TILE_0_Y = 20;
const int GROUND_ZERO_GUTTER = 4;
const int GROUND_TILE_SIZE = TILE_SIZE_PX + GROUND_ZERO_GUTTER;
TileSet TileSet::instance;

TileSet::TileSet() : tex_(), src_() {
	src_.x = src_.y = 0;
	src_.w = TILE_SIZE_PX;
	src_.h = TILE_SIZE_PX;
}
void TileSet::load_image(const std::string& path, const game::Renderer& r) {
	tex_.reset(new game::SDLTexture(r.create_texture_from_image(path)));
	tex_->blend_mode(SDL_BLENDMODE_BLEND);
}
bool TileSet::has_image() const {return tex_.get() != nullptr;}
void TileSet::blit_ground(const int pos_x, const int pos_y, const game::Renderer& r, SDL_Rect & dst_r) {
	src_.x = GROUND_TILE_0_X + pos_x * GROUND_TILE_SIZE;
	src_.y = GROUND_TILE_0_Y + pos_y * GROUND_TILE_SIZE;
	r.copy_from(*tex_,src_,dst_r);
}

void TileSet::blit_shooter(const game::Renderer& r, SDL_Rect & dst_r) {
	static SDL_Rect src = {835*2,255*2,SHOOTER_WIDTH_PX,SHOOTER_HEIGHT_PX};
	SDL_Rect des = {dst_r.x,dst_r.y,SHOOTER_WIDTH_PX,SHOOTER_HEIGHT_PX};
	r.copy_from(*tex_,src,des);
}

void TileSet::blit_dome(const game::Renderer& r, const game::Vector2 & dst) {
	static SDL_Rect src = {20,286,STATIC_DOME_PX,STATIC_DOME_PX};
	SDL_Rect des = {dst.x_i(),dst.y_i(),STATIC_DOME_PX,STATIC_DOME_PX};
	r.copy_from(*tex_,src,des);
}

