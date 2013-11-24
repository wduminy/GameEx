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

	GroundCover cover_from(const unsigned char r, const unsigned char g, const unsigned char b) {
		if (r > 100)
			return GroundCover::Sand;
		if (b > 100)
			return GroundCover::Water;
		if (g > 100)
			return GroundCover::Scrub;
		if (g > 80)
			return GroundCover::Grass;
		if (g > 30)
			return GroundCover::Bush;
		return GroundCover::Road;
	}
}

GroundMap::GroundMap(const std::string &file_name) : _data() {
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
	        	_data[r * s.w + c] = cover_from(source[pos+2],source[pos+1],source[pos]);
	        }
}

void GroundMap::calculate_tile(const int x, const int y, int &t_x, int &t_y) {
	const int ic = y * MAP_WIDTH + x;
	if (ic < 0) {t_x = 2; t_y = 0;}
	else if (_data[ic] == GroundCover::Road) {t_x = 6; t_y = 3;} 
	else if (_data[ic] == GroundCover::Grass) {t_x = 2; t_y = 4;} 
	else if (_data[ic] == GroundCover::Sand) {t_x = 0; t_y = 5;} 
	else if (_data[ic] == GroundCover::Water) {t_x = 4; t_y = 6; }
	else if (_data[ic] == GroundCover::Bush) {t_x = 9; t_y = 9; }
	else if (_data[ic] == GroundCover::Scrub) {t_x = 4; t_y = 9; }
	else {t_x = 2; t_y = 0;}	
}

ShooterMapView::ShooterMapView(const LevelState * state)
	: _draw_dst(), _map_left(0), _row_at_top(LEVEL_HEIGHT-MAP_HEIGHT), _state(state){
		_draw_dst.x = 0;
		_draw_dst.y = 0;
}

void ShooterMapView::initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) {
	GameObject::initialise(rctx,dc);
	if (!tiles.has_image())
		tiles.load_image(rctx.path_to("Master484.bmp"),dc.screen());
	_ground_map.reset(new GroundMap(rctx.path_to("Level1.bmp")));
	_map_left = (dc.width()/2) - MAP_WIDTH_PX/2;
	ENSURE(_map_left + MAP_WIDTH_PX <= dc.width(), "map is too wide for screen");
	draw_ground(dc);
}

void ShooterMapView::draw_ground(const game::DrawContext &dc) {
	const int bottom_px = _state->shooter().bottom() + SHOOTER_GUTTER;
	const int bottom_row = bottom_px / TILE_SIZE_PX;
	const int top_row = bottom_row - MAP_HEIGHT;
	const int offset = bottom_row * TILE_SIZE_PX - bottom_px;   
	const int top_px = bottom_px - MAP_HEIGHT_PX;

	_draw_dst.x = _map_left;
	_draw_dst.y = offset;
	int tile_x, tile_y;
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j <= MAP_HEIGHT; j++) {
			_ground_map->calculate_tile(i,top_row+j,tile_x,tile_y);
			tiles.blit_ground(tile_x,tile_y,dc.screen(),_draw_dst);
			_draw_dst.y = TILE_SIZE_PX * j + offset;
		}
		_draw_dst.x += TILE_SIZE_PX;
	}

	_draw_dst.x = _map_left + MAP_WIDTH_PX / 2 - SHOOTER_WIDTH_PX/2;
	_draw_dst.y = _state->shooter().bottom() - top_px - SHOOTER_HEIGHT_PX;

	tiles.blit_shooter(dc.screen(),_draw_dst);
}

void ShooterMapView::draw(const game::DrawContext &dc) {
	draw_ground(dc);
}

void ShooterMapView::update(const game::UpdateContext &uc) {
}

