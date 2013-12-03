TARGET_DIR = RunDir
# choose a debug option -- the first one is for debugging ... the orther for fastest
DEBUG = -O0 -g3
SPEED = -O3 -DNDEBUG
FLAGS = $(SPEED) 
SDL = C:\development\cpp\libraries\SDL2-2.0.1\i686-w64-mingw32
SDL_FLAGS = -Dmain=SDL_main -mwindows
COMPILE_ARGS = -Dmain=SDL_main -mwindows -I"$(SDL)\include" -ITutLib -IGameLandLib -Wall -c -fmessage-length=0 -std=c++11 $(FLAGS)
LINK_ARGS = -L"$(SDL)\lib" -L$(TARGET_DIR) -lmingw32 -lwsock32 -lglu32 -lopengl32 -lSDL2 -lSDL2main -lSDL2_ttf -lSDL2.dll -lSDL2_image
DIRS = GameLandLib GameLandTests TutLib TerrainDemo Sneaky shooter
SOURCES := $(foreach e, $(DIRS), $(wildcard $(e)/*.cpp))
DEPS := $(patsubst %.cpp, %.depends, $(SOURCES))
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))
GAME_LAND_LIB := $(TARGET_DIR)/libGameLandLib.a
TUT_LIB := $(TARGET_DIR)/libTutLib.a
GAME_LAND_TEST := $(TARGET_DIR)/GameLandTests.exe
TERRAIN_DEMO := $(TARGET_DIR)/TerrainDemo.exe
SNEAKY := $(TARGET_DIR)/Sneaky.exe
SHOOTER := $(TARGET_DIR)/Shooter.exe
.PHONY : clean all run_sneaky run_terrain run_test dox run_shooter

run_shooter: $(SHOOTER)
	cd $(TARGET_DIR); ./Shooter.exe -nfullscreen; cat log.txt

run_sneaky: $(SNEAKY)
	cd $(TARGET_DIR); ./Sneaky.exe

run_terrain: $(TERRAIN_DEMO)
	cd $(TARGET_DIR); ./TerrainDemo.exe

$(SHOOTER): $(OBJS) $(GAME_LAND_LIB)
	g++ $(wildcard shooter/*.o) -o $@ -lGameLandLib $(LINK_ARGS)  


run_tests:  $(GAME_LAND_TEST)
	cd $(TARGET_DIR); ./GameLandTests.exe
	
dox: 
	rm -f -r ../codespear.github.io/gameex/html
	cd docs; doxygen doxy.txt


$(SNEAKY): $(OBJS) $(GAME_LAND_LIB)
	g++ $(wildcard Sneaky/*.o) -o $@ -lGameLandLib $(LINK_ARGS)  

$(TERRAIN_DEMO): $(OBJS) $(GAME_LAND_LIB)
	g++ $(wildcard TerrainDemo/*.o) -o $@ -lGameLandLib $(LINK_ARGS)  


$(GAME_LAND_TEST): $(OBJS) $(TUT_LIB) 
	g++ $(wildcard GameLandTests/*.o) -o $@ -lGameLandLib -lTutLib $(LINK_ARGS)  

$(GAME_LAND_LIB): $(OBJS)
	ar -r $@ $(wildcard GameLandLib/*.o)

$(TUT_LIB): $(GAME_LAND_LIB) 	
	ar -r $@ $(wildcard TutLib/*.o)

clean:
	rm -f $(foreach e, $(DIRS), $(wildcard $(e)/*.depends)) $(foreach e, $(DIRS), $(wildcard $(e)/*.o))

%.o: %.cpp %.depends 
	g++ $(COMPILE_ARGS) -c $< -o $@

%.depends: %.cpp 
	g++ -MT $(@:.depends=.o) -MM $(COMPILE_ARGS) $< -o $@

# The - is in front of the include, so that we do not get an error if the depends
# files to not exist
-include $(DEPS)