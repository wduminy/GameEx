#include "shooter_map.h"
#include <sdl_objects.h>
#include <log.h>

namespace {
	// dimensions
	const int TILE_SIZE = 28; // number of pixels in source and destination
	const int MAP_WIDTH = 10; // number of tiles
	const int MAP_HEIGHT = 10; // number of tiles
	const int MAP_WIDTH_PX = MAP_WIDTH * TILE_SIZE;
	const int MAP_HEIGHT_PX = MAP_HEIGHT * TILE_SIZE;

	// positions graphics inside bmp
	const int GROUND_TILE_0_X = 277;
	const int GROUND_TILE_0_Y = 10;
	const int GROUND_ZERO_GUTTER = 2;
	const int GROUND_TILE_SIZE = TILE_SIZE + GROUND_ZERO_GUTTER;
	class TileSet {
	public:
		TileSet() : _image(), _src() {
			_src.x = _src.y = 0;
			_src.w = TILE_SIZE;
			_src.h = TILE_SIZE;
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
	public:	 
		game::Surface::u_ptr _image;	
	private:
		SDL_Rect _src;
	};
	TileSet tiles;
}

ShooterMap::ShooterMap()
	: _draw_dst(),_map_top(0), _map_left(0) {
		_draw_dst.x = 0;
		_draw_dst.y = 0;
}

void ShooterMap::initialise (const game::ResourceContext &rctx, const game::DrawContext &dc) {
	GameObject::initialise(rctx,dc);
	if (!tiles.has_image())
		tiles.load_image(rctx.path_to("Master484.bmp"),dc.screen());
	_map_top = (dc.height()/2) - MAP_HEIGHT_PX/2;
	_map_left = (dc.width()/2) - MAP_WIDTH_PX/2;
	ENSURE(_map_top + MAP_HEIGHT_PX <= dc.height(),"map is too high for screen");
	ENSURE(_map_left + MAP_WIDTH_PX <= dc.width(), "map is too wide for screen");
	_draw_dst.x = _map_left;
	_draw_dst.y = _map_top;
	tiles.blit_ground(0,0,dc.screen(),_draw_dst);
	_draw_dst.x = _map_left + TILE_SIZE;
	_draw_dst.y = _map_top + TILE_SIZE;
	tiles.blit_ground(0,0,dc.screen(),_draw_dst);
	//SDL_BlitSurface(tiles._image->sdl_p(),0,dc.screen(),&_draw_dst);
}


void ShooterMap::draw(const game::DrawContext &dc) {
}

void ShooterMap::update(const game::UpdateContext &uc) {
}

