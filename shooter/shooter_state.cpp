#include <log.h>
#include "shooter_state.h"
#include "tileset.h"

namespace {
	class ShooterCollisions : public game::CollisionListener {
	};

	const game::BoundedBox2 play_box(0,0,LEVEL_WIDTH_PX, LEVEL_HEIGHT_PX);
	
}


void ShooterState::load_map(const std::string &file_name) {
	map_.reset(new WarZone(file_name));
}

void ShooterState::load_dromes(const std::string &file_name) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_name.c_str());
	auto root = doc.RootElement();
	auto xml = root->FirstChildElement();
	auto statics = xml->FirstChildElement("staticDromes");
	ASSERT(shooter_);
	if (statics != nullptr) {
		auto elem = statics->FirstChildElement();
		while (elem != nullptr) {
			add_part(new Drome(object_t::StaticDome,elem,*shooter_));
			elem = elem->NextSiblingElement();
		}
	}
}

void ShooterState::initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) {
	GameObject::initialise(rctx,dc);
	load_map(rctx.path_to("level1.bmp"));
	load_dromes(rctx.path_to("level1.xml"));
	if (!TileSet::instance.has_image())
		TileSet::instance.load_image(rctx.path_to("Master484.png"),dc.render());
	map_left_ = (dc.width()/2) - MAP_WIDTH_PX/2;
	ENSURE(map_left_ + MAP_WIDTH_PX <= dc.width(), "map is too wide for screen");
	update_tile_indexes();
}

ShooterState::ShooterState() :
			col_mgr_(new ShooterCollisions(),play_box,4),
			shooter_(new Shooter()),
			map_(),
	left_key_down_(false), right_key_down_(false),
	up_key_down_(false), down_key_down_(false),
	draw_dst_(), map_left_(0), row_at_top_(LEVEL_HEIGHT-MAP_HEIGHT),  tile_x_(), tile_y_(){
		draw_dst_.x = 0;
		draw_dst_.y = 0;
}

void ShooterState::update(const game::UpdateContext & uc) {

	switch (uc.input().key_down()) {
		case SDLK_LEFT:left_key_down_ = true;break;
		case SDLK_RIGHT:right_key_down_ = true;break;
		case SDLK_UP:up_key_down_ = true;break;
		case SDLK_DOWN: down_key_down_ = true;break;
		default:break;
	}
	switch (uc.input().key_up()) {
		case SDLK_LEFT:left_key_down_ = false;break;
		case SDLK_RIGHT:right_key_down_ = false;break;
		case SDLK_UP:up_key_down_ = false;break;
		case SDLK_DOWN: down_key_down_ = false;break;
		default:break;
	}
	if (left_key_down_ && !right_key_down_)
		shooter_->set_dir(HorizontalDir::Left);
	else if (right_key_down_ && !left_key_down_)
		shooter_->set_dir(HorizontalDir::Right);
	else
		shooter_->set_dir(HorizontalDir::None);
	if (up_key_down_ && !down_key_down_)
		shooter_->set_dir(VerticalDir::Up);
	else if (down_key_down_ && !up_key_down_)
		shooter_->set_dir(VerticalDir::Down);
	else
		shooter_->set_dir(VerticalDir::None);
	shooter_->move(uc.seconds_per_update());
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

void ShooterState::calculate_tile(const int x, const int y, int &t_x, int &t_y) {
  const auto& gmap = map();
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




void ShooterState::update_tile_indexes() {
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


void ShooterState::draw(const game::DrawContext &dc) {
	const int bottom_px = shooter().bottom_of_view() + SHOOTER_GUTTER;
	const int bottom_row = bottom_px / TILE_SIZE_PX;
	const int top_row = bottom_row - MAP_HEIGHT;
	const int offset = bottom_row * TILE_SIZE_PX - bottom_px;
	const int top_px = bottom_px - MAP_HEIGHT_PX;

	draw_dst_.x = map_left_;
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

	draw_dst_.x = map_left_ + shooter().box().left();
	draw_dst_.y = shooter().view_top() - top_px;

	TileSet::instance.blit_shooter(dc.render(),draw_dst_);

}


