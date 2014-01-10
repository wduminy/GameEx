# Shooter
## Story
You are the pilot of a *Shooter*. Your goal is to cross a large section of terrestrial terrain, occupied by enemy forces. The terrain you must cross is divided into nine zones.  At the end of each *WarZone* your shooter will be repaired - that is if you can reach the end.

Life is not simple: the enemy forces has ground based vehicles names Dromautibles. These are commonly called Dromes.  You can destroy a Drome using your *MarkBomber*  With this weapon you aim at a spot on the ground and launch a *MarkBomb* to that spot.  The MarkBomb is controlled from your Shooter and it will hit its target.  But your MarkBomber can only handle three active MarkBombs concurrently.  If four of these are in the air, your MarkBomber will launch another.      

Be careful, the enemy *Drome* launches ballistic weapons called SlowBombs. Lucky thing is that each Drome has a limited number of these bombs.  The *Slobomb* aims to get in your way; and they explode on impact.  That is when you impact them!  A *StaticDrome* is mounted on one spot; but there is also the *RoamingDrome* that moves around; but at least it keeps on the road.  A RoamingDrome is harder to hit because it is moving but these Dromes have less ammunition than a FixedDromes. 

Alas that is not all. The enemy also attacks from the air.  These so-called "Cannies" are quite nimble and they are armed (just like you) with a *LaserBlaster*.  A LaserBlaster can shoot five or so LaserBombs in short succession.  But then it needs a bit of time to cool down.  A *LaserBomb* moves fast and destroys enemy planes on contact.  However, the range of this weapon is limited.   

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

## Controls


## Credits
- Artwork: (Master484) [http://m484games.ucoz.com/]
- Libraries
-- SDL (http://www.libsdl.org)
-- SDL Image (http://www.libsdl.org/projects/SDL_image/)

## Plan
- Implement map scrolling
- Derive detail from ground map