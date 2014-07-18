# Installing SFML (2014-07-18)
- Registered at forum: http://en.sfml-dev.org/forums/
-- The forum seems quite active; a good sign
- Downloaded for mingw 32 bit from http://www.sfml-dev.org/download.php
- Extracted to F:\dev\smfl\SFML-2.1
- Help files now at: file:///F:/dev/smfl/SFML-2.1/doc/html/index.htm
- Include dir: F:\dev\smfl\SFML-2.1\include
- Lib dir: F:\dev\smfl\SFML-2.1\lib
- The basic infor for mingw setup is available here:  http://www.sfml-dev.org/tutorials/2.1/start-cb.php
- This works if and only if the dlls are in rundir 
- I see there are static libs, which I am ignoring for now
- Overall a very cool and painless setup process.  I am impressed!

# Handling time: http://www.sfml-dev.org/tutorials/2.1/system-time.php
- sf::Time keeps a relative value
- it can be converted from and to standard time units
- and it has a number of expected operators
- There is also an sf::Clock that can be used to mark points in time

# Threads: http://www.sfml-dev.org/tutorials/2.1/system-thread.php
- if you want use c++11 threads, they are mst likely better.

# Data streams: http://www.sfml-dev.org/tutorials/2.1/system-stream.php
- there are a rich set of resources that can be loaded from file; or even from memory (packed with the executable)
- you can also implement your own stream class if you want to read from something else

# Open and manage a Window: http://www.sfml-dev.org/tutorials/2.1/window-window.php
- sf::Window and sf::Style work together to decide on how your window will look
- the window receives events for the game loop; and must handle a special event called 'close'
- you can bypass graphics and draw stuff directly on the window using OpenGL
- you can also embed a sfml in a winapi window using the os window handle
- you framerate can be set to synch with the monitor's frequency; or you can also set it explicitly (not both)
- you can create multiple windows
- events must be polled in the windows thread

# Events explained: http://www.sfml-dev.org/tutorials/2.1/window-events.php
- an event is a union
- typical windows events, open, close and so on
- there are keypressed and keyrelease and also text entered events
- then there are the mouse and joystick events

# Keyboard, mouse and joystick: http://www.sfml-dev.org/tutorials/2.1/window-inputs.php
- These are not events, but are realtime inputs - that is the state of the input device

# using OpenGL: http://www.sfml-dev.org/tutorials/2.1/window-opengl.php
- sfml gives you access to the opengl headers; you have to inlude the lib yourself
- you can also have multiple opengl windows, and even no window; or render in a different thread
- it is possible, but tricky to mix 'raw' opengl and normal 2d rendering

# Drawing 2D: http://www.sfml-dev.org/tutorials/2.1/graphics-draw.php
- Use sf::RenderWindow instead of sf::Window if you want to draw
- On every cycle you typically clear before drawing; but clear is fast; no need to try and work around it. Then things are 'drawn' in the render window.  And finally 'display' is called to show the draw window
- You can also draw to textures (sf::RenderTarget)
- It is also possible to do the rendering in another thread

#Sprites and textures: http://www.sfml-dev.org/tutorials/2.1/graphics-sprite.php
- a sprite is a textured rectangle
- you can load a texture from a file, memory, tream or from another loaded image 
- you can also load parts of an image
- a texture can be automatically smoothed
- you can repeat a texture in a sprite
- you can change the colour and clip the texture on the sprite as well
- a sprite has a proition, a rotation and a scale
- remember, the lifetime of the texture and sprite are tied together
- use a single texture (or a few textures as possible) - changing textures is not good for performance
- you can also use textures with open gl

#Text and fonts: http://www.sfml-dev.org/tutorials/2.1/graphics-text.php
- ttf works easily enough
- think of the text object like a sprite (has a font instead of a texture)
- it is possible to put a font on a texture; and then get the sf::glyph from the texture

# Shapes: http://www.sfml-dev.org/tutorials/2.1/graphics-shape.php
 - A whole set of shapes can be drawn
 - There is also an antialias setting to get rid of jegged edges
 
# Your own entities: http://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array.php
 - instead of drawing one sprite at a time, use a vertex array
 - this allows you to use opengl like primitives on the array
 - vertex array can be used for texturing
 - vertex arrays can be transformed
 - with the vertex array you can create an entity; yoy can add the 'drawable' and 'transformable' class on that entity
 - a tile map example illustrates this point nicely
 - another example is a particle system

# Position, rotation, scale: transforming entities: http://www.sfml-dev.org/tutorials/2.1/graphics-transform.php
 - smfl entities have a bounding box (sizes aligned with x and y axis)
 
# Using shaders: http://www.sfml-dev.org/tutorials/2.1/graphics-shader.php
 - GLSL shaders can be used applied to an entity when it is drawn
 
# Controlling the 2D camera: http://www.sfml-dev.org/tutorials/2.1/graphics-view.php
 - A sf::view allows you to scroll over your world
 - the view can be set, moved, rotated an zoomed
 - typically the view is shown in the window, but you can use viewports for split-screen and other effects
 - a view can also be used to create a 'pane' around the play area; or maybe a hud
 - if you use views, remeber to adjust based on the window size
 
# Playing sound: http://www.sfml-dev.org/tutorials/2.1/audio-sounds.php
 - use sf::sound for small effects and sf::music for backgound music
 - sounds can be spatial: http://www.sfml-dev.org/tutorials/2.1/audio-spatialization.php
 
