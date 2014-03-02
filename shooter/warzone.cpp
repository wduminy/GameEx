#include "warzone.h"
#include "tileset.h"
#include "shooter_constants.h"
#include <sdl_objects.h>
namespace {
		GroundCover cover_from(const unsigned char r, const unsigned char g, const unsigned char b) {
		if (r > 140)
			return GroundCover::Sand;
		if (b > 160)
			return GroundCover::Water;
		if (g > 250)
			return GroundCover::Scrub;
		if (g > 230)
			return GroundCover::Grass;
		if (g > 100)
			return GroundCover::Bush;
		return GroundCover::Road;
	}
}



WarZone::WarZone(const std::string &file_name, const Shooter& sh) :
		data_(), draw_dst_(), tile_x_(), tile_y_(), shooter_(sh) {
	draw_dst_.x = 0;
	draw_dst_.y = 0;
	game::Surface surface(file_name);
	SDL_Surface& s = surface.sdl();
	if (s.format->BytesPerPixel != 3)
		throw systemex::runtime_error("BMP is not 24 bit.");
	if (s.w != MAP_WIDTH)
		throw systemex::runtime_error_ex("level width (%d) and BMP width (%d) does not match.", MAP_WIDTH, s.w);		
	if (LEVEL_SIZE != static_cast<size_t> (s.w * s.h))
		throw systemex::runtime_error_ex("level size (%d) and BMP size (%d) does not match.", LEVEL_SIZE, s.w * s.h);
	auto source = (unsigned char *) s.pixels;
	for(int c = 0U; c < s.w; c++)
	        for(int r = 0U; r < s.h; r++) {
	        	auto pos = r * s.pitch + (c * 3);
	        	data_[r * s.w + c] = cover_from(source[pos+2],source[pos+1],source[pos]);
	        }
}


// these macros help to identify the tiles in the tile image (bmp)
#define STILE(sx,sy,all) if (m==GroundCover::all && u==GroundCover::all && d==GroundCover::all && l==GroundCover::all && r==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_DLR(sx,sy,all) if (m==GroundCover::all && d==GroundCover::all && l==GroundCover::all && r==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_UDL(sx,sy,all) if (m==GroundCover::all && u==GroundCover::all && d==GroundCover::all && l==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_UDR(sx,sy,all) if (m==GroundCover::all && u==GroundCover::all && d==GroundCover::all && r==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_ULR(sx,sy,all) if (m==GroundCover::all && u==GroundCover::all && l==GroundCover::all && r==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_LR(sx,sy,all) if (m==GroundCover::all && l==GroundCover::all && r==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_UD(sx,sy,all) if (m==GroundCover::all && u==GroundCover::all && d==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_UL(sx,sy,all) if (m==GroundCover::all && u==GroundCover::all && l==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_UR(sx,sy,all) if (m==GroundCover::all && u==GroundCover::all && r==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_DL(sx,sy,all) if (m==GroundCover::all && d==GroundCover::all && l==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_DR(sx,sy,all) if (m==GroundCover::all && d==GroundCover::all && r==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_U(sx,sy,all) if (m==GroundCover::all && u==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_D(sx,sy,all) if (m==GroundCover::all && d==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_L(sx,sy,all) if (m==GroundCover::all && l==GroundCover::all) {t_x=sx;t_y=sy;return;};
#define STILE_R(sx,sy,all) if (m==GroundCover::all && r==GroundCover::all) {t_x=sx;t_y=sy;return;};

#define STILE_M(sx,sy,all) if (m==GroundCover::all) {t_x=sx;t_y=sy;return;};

#define TILE_U(sx,sy,mid,other) if (m==GroundCover::mid && u==GroundCover::other) {t_x=sx;t_y=sy;return;};
#define TILE_D(sx,sy,mid,other) if (m==GroundCover::mid && d==GroundCover::other) {t_x=sx;t_y=sy;return;};
#define TILE_L(sx,sy,mid,other) if (m==GroundCover::mid && l==GroundCover::other) {t_x=sx;t_y=sy;return;};
#define TILE_R(sx,sy,mid,other) if (m==GroundCover::mid && r==GroundCover::other) {t_x=sx;t_y=sy;return;};

#define TILE_LR(sx,sy,all,other) if (m==GroundCover::all && l==GroundCover::other && r==GroundCover::other) {t_x=sx;t_y=sy;return;};
#define TILE_UD(sx,sy,all,other) if (m==GroundCover::all && u==GroundCover::other && d==GroundCover::other) {t_x=sx;t_y=sy;return;};
#define TILE_UL(sx,sy,all,other) if (m==GroundCover::all && u==GroundCover::other && l==GroundCover::other) {t_x=sx;t_y=sy;return;};
#define TILE_UR(sx,sy,all,other) if (m==GroundCover::all && u==GroundCover::other && r==GroundCover::other) {t_x=sx;t_y=sy;return;};
#define TILE_DL(sx,sy,all,other) if (m==GroundCover::all && d==GroundCover::other && l==GroundCover::other) {t_x=sx;t_y=sy;return;};
#define TILE_DR(sx,sy,all,other) if (m==GroundCover::all && d==GroundCover::other && r==GroundCover::other) {t_x=sx;t_y=sy;return;};

void WarZone::calculate_tile(const int x, const int y, int &t_x, int &t_y) {
  const auto& gmap = *this;
  auto m = gmap(x,y);
  const auto u = gmap(x,y-1);
  const auto d = gmap(x,y+1);
  const auto l = gmap(x-1,y);
  const auto r = gmap(x+1,y);
  STILE(6,3,Road)
  STILE_DLR(6,4,Road)
  STILE_UDL(6,5,Road)
  STILE_UDR(6,6,Road)
  STILE_ULR(6,7,Road)
  STILE_LR(0,3,Road)
  STILE_UD(1,3,Road)
  STILE_UL(2,3,Road)
  STILE_DL(3,3,Road)
  STILE_UR(4,3,Road)
  STILE_DR(5,3,Road)
  STILE_U(1,3,Road)
  STILE_D(1,3,Road)
  STILE_L(0,3,Road)
  STILE_R(0,3,Road)
  STILE_M(6,3,Road)

  STILE(4,9,Scrub)
  TILE_UL(0,10,Scrub,Grass)
  TILE_UR(1,10,Scrub,Grass)
  TILE_DR(1,11,Scrub,Grass)
  TILE_DL(0,11,Scrub,Grass)
  TILE_D(0,9,Scrub,Grass)  STILE(4,9,Scrub)
  TILE_UL(0,10,Scrub,Grass)
  TILE_UR(1,10,Scrub,Grass)
  TILE_DR(1,11,Scrub,Grass)
  TILE_DL(0,11,Scrub,Grass)
  TILE_D(0,9,Scrub,Grass)
  TILE_U(1,9,Scrub,Grass)
  TILE_R(2,9,Scrub,Grass)
  TILE_L(3,9,Scrub,Grass)
  STILE_M(4,9,Scrub);
  TILE_U(1,9,Scrub,Grass)
  TILE_R(2,9,Scrub,Grass)
  TILE_L(3,9,Scrub,Grass)
  STILE_M(4,9,Scrub);

  STILE(4,9-3,Water)
  TILE_UL(0,10-3,Water,Grass)
  TILE_UR(1,10-3,Water,Grass)
  TILE_DR(1,11-3,Water,Grass)
  TILE_DL(0,11-3,Water,Grass)
  TILE_D(0,9-3,Water,Grass)
  TILE_U(1,9-3,Water,Grass)
  TILE_R(2,9-3,Water,Grass)
  TILE_L(3,9-3,Water,Grass)
  STILE_M(4,9-3,Water);

  STILE(4+5,9,Bush)
  TILE_UL(0+5,10,Bush,Grass)
  TILE_UR(1+5,10,Bush,Grass)
  TILE_DR(1+5,11,Bush,Grass)
  TILE_DL(0+5,11,Bush,Grass)
  TILE_D(0+5,9,Bush,Grass)
  TILE_U(1+5,9,Bush,Grass)
  TILE_R(2+5,9,Bush,Grass)
  TILE_L(3+5,9,Bush,Grass)
  STILE_M(4+5,9,Bush);

  STILE(0,4,Grass)
  STILE_LR(4,4,Grass)
  STILE_UD(1,4,Grass)
  STILE_M(3,4,Grass);
	t_x = 2; t_y = 0;
}


void WarZone::draw(const DrawContext& dc) {
	const int bottom_px = shooter_.bottom_of_view() + SHOOTER_GUTTER;
	const int bottom_row = bottom_px / TILE_SIZE_PX;
	const int top_row = bottom_row - MAP_HEIGHT;
	const int offset = bottom_row * TILE_SIZE_PX - bottom_px;
	draw_dst_.x = shooter_.map_left();
	draw_dst_.y = offset;
	draw_dst_.w = TILE_SIZE_PX;
	draw_dst_.h = TILE_SIZE_PX;
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j <= MAP_HEIGHT; j++) {
			size_t ix = MAP_WIDTH * (top_row+j) + i;
			draw_dst_.y = j * TILE_SIZE_PX + offset;
			if (ix < LEVEL_SIZE)
				TileSet::instance.blit_ground(tile_x_[ix],tile_y_[ix],dc.render(),draw_dst_);
			else
				TileSet::instance.blit_ground(2,0,dc.render(),draw_dst_);
		}
		draw_dst_.x += TILE_SIZE_PX;
	}
}


void WarZone::update_tile_indexes() {
	int cx = 0;
	int cy = 0;
	for (int i = 0; i < MAP_WIDTH; i++)
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			calculate_tile(i,j,cx,cy);
			size_t ix = MAP_WIDTH * j + i;
			tile_x_[ix] = cx;
			tile_y_[ix] = cy;
		}
}
