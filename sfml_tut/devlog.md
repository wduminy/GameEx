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
 
