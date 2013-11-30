#include "shooter_map.h"
#include <sdl_objects.h>
#include <log.h>

namespace {
	// dimensions
	const int MAP_HEIGHT = WINDOW_HEIGHT / TILE_SIZE_PX + 1 ; // number of rows visible on screen

	const int MAP_WIDTH_PX = LEVEL_WIDTH_PX;
	const int MAP_HEIGHT_PX = MAP_HEIGHT * TILE_SIZE_PX;

	// positions graphics inside bmp
	const int GROUND_TILE_0_X = 554;
	const int GROUND_TILE_0_Y = 20;
	const int GROUND_ZERO_GUTTER = 4;
	const int GROUND_TILE_SIZE = TILE_SIZE_PX + GROUND_ZERO_GUTTER;
	class TileSet {
	public:
		TileSet() : _image(), _src() {
			_src.x = _src.y = 0;
			_src.w = TILE_SIZE_PX;
			_src.h = TILE_SIZE_PX;
		}
		void load_image(const std::string& path, SDL_Surface * screen) { 
			_image.reset(new game::Surface(path,screen));
			_image->set_transparent(0,0,0);
	 		_image->write_properties(LOG << "Loaded tiles:");
		}
		bool has_image() const {return _image.get() != nullptr;}
		void blit_ground(const int pos_x, const int pos_y, SDL_Surface * dst_s, SDL_Rect & dst_r) {
			_src.x = GROUND_TILE_0_X + pos_x * GROUND_TILE_SIZE;
			_src.y = GROUND_TILE_0_Y + pos_y * GROUND_TILE_SIZE;
			_image->blit_to(_src,dst_s,dst_r);
		}
		void blit_shooter(SDL_Surface * dst_s, SDL_Rect & dst_r) {
			static SDL_Rect src = {835*2,255*2,SHOOTER_WIDTH_PX,SHOOTER_HEIGHT_PX};
			_image->blit_to(src,dst_s,dst_r);

		}
	public:	 
		game::Surface::u_ptr _image;	
	private:
		SDL_Rect _src;
	};
	TileSet tiles;

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

void ShooterMapView::calculate_tile(const int x, const int y, int &t_x, int &t_y) {
  const auto& gmap = _state->level().ground();
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


ShooterMapView::ShooterMapView(const LevelState * state)
	: _draw_dst(), _map_left(0), _row_at_top(LEVEL_HEIGHT-MAP_HEIGHT), _state(state), _tile_x(), _tile_y(){
		_draw_dst.x = 0;
		_draw_dst.y = 0;
}

void ShooterMapView::initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) {
	GameObject::initialise(rctx,dc);
	if (!tiles.has_image())
		tiles.load_image(rctx.path_to("Master484.bmp"),dc.screen());
	_map_left = (dc.width()/2) - MAP_WIDTH_PX/2;
	ENSURE(_map_left + MAP_WIDTH_PX <= dc.width(), "map is too wide for screen");
	update_tile_indexes();
}

void ShooterMapView::update_tile_indexes() {
	int cx = 0;
	int cy = 0;
	for (int i = 0; i < MAP_WIDTH; i++) 
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			calculate_tile(i,j,cx,cy);
			size_t ix = MAP_WIDTH * j + i;
			_tile_x[ix] = cx;
			_tile_y[ix] = cy;
		}
}


void ShooterMapView::draw(const game::DrawContext &dc) {
	const int bottom_px = _state->shooter().bottom() + SHOOTER_GUTTER;
	const int bottom_row = bottom_px / TILE_SIZE_PX;
	const int top_row = bottom_row - MAP_HEIGHT;
	const int offset = bottom_row * TILE_SIZE_PX - bottom_px;   
	const int top_px = bottom_px - MAP_HEIGHT_PX;

	_draw_dst.x = _map_left;
	_draw_dst.y = offset;

	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			size_t ix = MAP_WIDTH * (top_row+j) + i;
			_draw_dst.y = j * TILE_SIZE_PX + offset;
			if (ix < LEVEL_SIZE)
				tiles.blit_ground(_tile_x[ix],_tile_y[ix],dc.screen(),_draw_dst);
			else
				tiles.blit_ground(2,0,dc.screen(),_draw_dst);
		}
		_draw_dst.x += TILE_SIZE_PX;
	}

	_draw_dst.x = _map_left + _state->shooter().left();
	_draw_dst.y = _state->shooter().bottom() - top_px - SHOOTER_HEIGHT_PX;

	tiles.blit_shooter(dc.screen(),_draw_dst);
}

void ShooterMapView::update(const game::UpdateContext &uc) {}

