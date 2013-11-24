# Shooter
## Story
For inspiration, see http://www.gamespot.com/xevious/
http://home.hiwaay.net/~lkseitz/cvg/vvgl/XEVIOUS.html
http://strategywiki.org/wiki/Xevious/Walkthrough
http://strategywiki.org/wiki/Xevious
http://www.arcade-museum.com/game_detail.php?game_id=10505
http://shmuptheory.blogspot.com/2010/02/anatomy-of-shmup.html

Shooter is a *vertical shoot'em up*.
A level should take no more than 2 minutes to complete.
Lets say we fly 10km an hour -- that is 300 meters in 2 minutes
A pixel is about 15 cm -- that is then 2000 pixels in 2 minutes.
Not impossible.  With 28 pixel per tile we need 70 tiles ... hmmm? 

## Shortcomings
*Resolution*
Currently to 800 x 600.  Supporting multiple graphics modes
with the fixed tileset seems a bit tricky.  Switching to full screen can  be done
but your OS must be setup to "cut off" the sides of the display, otherwise round 
things look oval.  Do do this run "shooter.exe -fullscreen".

## Controls


## Credits
- Artwork: (Master484) [http://m484games.ucoz.com/]
- Libraries
-- SDL (http://www.libsdl.org)
-- SDL Image (http://www.libsdl.org/projects/SDL_image/)

## Plan
- Implement map scrolling
- Derive detail from ground map