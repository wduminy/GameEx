#pragma once
#include <sdl_objects.h>
#include <game_math.h>
using game::Scalar;

class TileSet {
private:
	std::unique_ptr<game::SDLTexture> tex_;
	SDL_Rect src_;
	TileSet();
public:
	static TileSet instance;
	void load_image(const std::string& path, const game::Renderer& r);
	bool has_image() const;
	void blit_ground(const int pos_x, const int pos_y, const game::Renderer& r, SDL_Rect & dst_r);
	void blit_shooter(const game::Renderer& r,  const Scalar& x, const Scalar& y);
	void blit_dome(const game::Renderer& r, const game::Vector2& dst, const int img_index);
};


extern const int MAP_HEIGHT;
extern const int MAP_HEIGHT_PX;
extern const int MAP_WIDTH_PX;

