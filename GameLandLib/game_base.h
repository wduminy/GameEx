/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include <list>
#include <deque>
#include <forward_list>
#include "gl_objects.h"

namespace game {
typedef uint32_t time;
using std::string;


typedef std::unique_ptr<SDL_Surface> SDL_Surface_u_ptr;

/**
 * The ResourceContext keeps the resources.
 * The folder where the resources are is specified in the constructor.
 */
class ResourceContext {
public:

	/**
	 * @param subDirectory must have a '/' at the end
	 */
	ResourceContext(const string& subDirectory = "");

	string load_text(const char * filename) const;
	const string& dir() const {return _root_directory;};
	/** Returns the path to a media file */
	string path_to(const char * file_name) const {return _root_directory + file_name; } 
	ShaderProgram * load_program(Glex * gl, const char * filePrefix) const;

	/**
	 * @param gl
	 * @param filename
	 * @param textureIndex zero based index where 0 implies
	 * @return
	 */
	Texture * load_texture_bmp(Glex * gl, const char * filename, const int textureIndex) const;
private:
	const string _root_directory;
public:
	typedef std::unique_ptr<ResourceContext> u_ptr;
};

/** Provides detail of what the state of input devices */
class InputEvent {
public:
	const SDL_Event& event() const { return _event;}
	/** The key that is pressed down */
	SDL_Keycode key_down() const;
	/** The key that has been released */
	SDL_Keycode key_up() const;
	/** Is the control key pressed? */
	bool is_ctl_down() const; 
	/** True when the window is closed. Used in the game loop. See SDL_QUIT for info.*/
	bool is_quit() const;
	/** Update the event state. Used in the game loop */
	void poll();	
private:
	SDL_Event _event;
};

/**
 * The context in which the main game loop calls update.
 * For a client class the input() query may the the most interesting */
class UpdateContext {
public:
	UpdateContext(unsigned int targetUpdatesPerSecond, unsigned int targetFramesPerSecond);
	bool is_update_step() {return update;}
	bool is_draw_step() {return draw;}
	unsigned int update_interval() const {return _update_interval; }
	double updates_per_second() const { return 1000.0 / _update_interval; }
	double seconds_per_update() const { return 1.0 / updates_per_second();}
	void tick();
	void log_statistics() const;
	/** Provides access to the latest user input */
	const InputEvent& input() const {return _event;}
private:
	InputEvent _event;
	const unsigned int _update_interval;
	const unsigned int _draw_interval;
	const Uint32 _first_tick;
	time next_update;
	time next_draw;
	bool update;
	bool draw;
	Uint32 _tick_time;
	unsigned int _draws, _updates;
public:
	typedef std::unique_ptr<UpdateContext> u_ptr;
};

class DrawContext {
	PREVENT_COPY(DrawContext)
public:
	DrawContext(const bool fullscreen, const int width, const int height, const bool opengl);
	~DrawContext();
	Glex& gl() const;
	operator Glex*() const {return _glex.get();}
	int width() const {return _width;}
	int height() const {return _height;}
	bool has_opengl() const {return _glex.get();}
	void swap();
	void screen_to_bmp(const std::string& filename) const;
	Renderer& render() const {return *_renderer;}
private:
	SDL_Window * _window;
	Glex::u_ptr _glex;
	int _width;
	int _height;
	std::unique_ptr<Renderer> _renderer;
public:
	typedef std::unique_ptr<DrawContext> u_ptr;

};

class Drawable {
	virtual void draw(const DrawContext& gc) = 0;
};

/**
 Use this as base class for all objects that are updated or drawn in your game. It is not 
 the base class of all objects you need for your  game. Rather the GameObject instances forms 
 a runtime structure that defines the basic composition of your game. Instances cannot be 
 copied.  You keep exactly one instance for each important game concept in memory. 

   The most important members to take note of are update() and draw().  These functions are 
 called during the update and draw phases of the game loop.  If the object should not be 
 called during update it can be deactivated. Also, if it is not to be drawn, it can be hidden.  
 These states are independent; e.g. an object that is not active can still be visible. 

   The calls to update() is done in the order in which the GameObject instances are added 
 to their container but the calls to draw() is different.  The draw order of all game objects
 are decided when the game starts and is based on the value given during construction.  It is 
 therefore possible to have a draw order for your objects that is entirely different from the
 update order.

 \sa \ref using_gameobjects
*/
class GameObject : Drawable {
PREVENT_COPY(GameObject)
public:
	GameObject(int drawOrder = 0) :	_draw_order(drawOrder), _is_hidden(false), _is_active(true) {};
	/** initialise this and initialise children */
	virtual void initialise(const ResourceContext & rctx, const DrawContext& dctx) {};
	/** draw only this, not children */
	virtual void draw(const DrawContext& gc) = 0;
	/** update this and update children */
	virtual void update(const UpdateContext &) {}
	virtual void collect(std::deque<GameObject *> &c) {c.push_back(this);}
	int draw_order() const {return _draw_order;}
	void hide() {set_hidden(true);}
	void show() {set_hidden(false);}
	bool is_hidden() const {return _is_hidden;}
	bool is_visible() const {return !_is_hidden;}
	virtual void activate() {set_active(true);}
	/** Deactivate excludes an object from the update loop.  Note that it may still be visible */
	void deactivate() {set_active(false);}
	bool is_active() const {return _is_active;}
	virtual ~GameObject() {}
	virtual void set_hidden(const bool value) {_is_hidden = value;}
	virtual void set_active(const bool value) {_is_active = value;}
private:
	const int _draw_order;
	bool _is_hidden; // draw is not called when hidden
	bool _is_active; // update is not called when not active
public:
	typedef std::unique_ptr<GameObject> u_ptr;
};

/**
 Use this as base class for GameObject instances that contain other GameObjects.  Children
 are added via add_part(), either during construction or during initialise().  Typically
 initialised is used when the GameObject requires a draw context or a resource context 
 during construction.  
   The children are added to the container and will be destroyed when the container is 
 destroyed.  However, this destruction occurs only when the game loop terminates.  This means
 that it is safe to reference these pointers while the game is in progress. 
   The hierarchy formed by this containment is used to manage the visibility and the active 
 status. This means that hiding an object will hide its children and deactivating an object 
 deactivates it children.
   There is a subtle but important difference between the visibility and the active status 
 hierarchy when a child disagrees with its parent.  The active status of the parent overrides 
 the active status of the child, but the visibility status of the parent does not override the 
 visibility of the child. So if you hide an object and thereafter show one of its children, the 
 child will be drawn.
 */
class GameObjectWithParts: public GameObject {
public:
	GameObjectWithParts(const int drawOrder = 0) :
			GameObject(drawOrder), _parts() {}
	/** Call initialise() for every child.  In the order in which the children were added */
	virtual void initialise(const ResourceContext &rctx, const DrawContext& dctx) override {
		for (auto p = _parts.begin(); p != _parts.end(); p++) (*p)->initialise(rctx,dctx);
	};
	/** Call update() for each active child */
	void update(const UpdateContext &context) override;

	void add_part(GameObject::u_ptr object) {
		auto before_end = _parts.before_begin();
		for (auto& _ : _parts)
  		if (_) ++ before_end;
		_parts.emplace_after(before_end, std::move(object));
	};

	/** Add a part to this composite.
	 * Note that this memory of this part is is managed by this
	 * containing object.
	 * @param object
	 */
	void add_part(GameObject * object) {
		auto before_end = _parts.before_begin();
		for (auto& _ : _parts)
  		if (_) ++ before_end;
		_parts.emplace_after(before_end, GameObject::u_ptr(object));
	}

	void collect(std::deque<GameObject*> &c);
	/** Default draw method does nothing.  Note that it does not draw the children.*/
	void draw(const DrawContext&) override {};
protected:
	virtual void set_hidden(const bool value) override;
	virtual void set_active(const bool value) override;
	std::forward_list<GameObject::u_ptr> _parts;
};


/**
 Use this as base when you have a chain of GameObjectWithParts such that only one object in the
 chain is active at a time.     
 */
class GameObjectChainLink: public GameObjectWithParts {
PREVENT_COPY(GameObjectChainLink)
public:
	/** Default constructor */
	GameObjectChainLink() : _next(0) {}
	/** Set the next object in the chain */
	void set_next(GameObjectChainLink * value) {_next = value;}
	/** Deactivate this object and activate the next object in the chain */
	void activate_next() {deactivate(); if (_next) {_next->activate();};}
private:
	GameObjectChainLink * _next;
};

class MainObject: public GameObjectWithParts {
public:
	MainObject(int drawOrder=-1, GLdouble nearest=0.1, GLdouble farest=100)
		: GameObjectWithParts(drawOrder), _nearest(nearest), _farest(farest), _print_screen(false) {};
    void initialise(const ResourceContext & rctx, const DrawContext& dctx) override;
	void update(const UpdateContext& ctx) override;
	void draw(const DrawContext&) override;
private:
	GLdouble _nearest, _farest;
	bool _print_screen;
public:
	typedef std::unique_ptr<MainObject> u_ptr;
};

class Game {
public:
	// the game owns these 'pointers'
	Game(MainObject::u_ptr primaryPart,
		UpdateContext::u_ptr update,
		DrawContext::u_ptr draw,
		ResourceContext::u_ptr resource);
	Game(MainObject * primaryPart,
			const std::string& resource_path,
			int ups = 50, int fps = 20,
			bool fullscreen = false, int width=800, int height=600,
			bool opengl = true);
	// the game runs until primary is dead
	int run();
private:
	MainObject::u_ptr _primary;
	UpdateContext::u_ptr _update;
	DrawContext::u_ptr _draw;
	ResourceContext::u_ptr _resource;
};

}

