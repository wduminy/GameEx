TARGET_DIR = RunDir
COMPILE_ARGS = -I"C:\development\cpp\libraries\SDL-1.2.15\include" -ITutLib -IGameLandLib -O3 -Wall -c -fmessage-length=0 -std=c++0x
LINK_ARGS = -L"C:\development\cpp\libraries\SDL-1.2.15\lib" -L$(TARGET_DIR) -lmingw32 -lwsock32 -lglu32 -lopengl32 -lSDLmain -lSDL_ttf -lSDL.dll 
DIRS = GameLandLib GameLandTests TutLib TerrainDemo Sneaky
SOURCES := $(foreach e, $(DIRS), $(wildcard $(e)/*.cpp))
DEPS := $(patsubst %.cpp, %.depends, $(SOURCES))
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))
GAME_LAND_LIB := $(TARGET_DIR)/libGameLandLib.a
TUT_LIB := $(TARGET_DIR)/libTutLib.a
GAME_LAND_TEST := $(TARGET_DIR)/GameLandTests.exe
TERRAIN_DEMO := $(TARGET_DIR)/TerrainDemo.exe
SNEAKY := $(TARGET_DIR)/Sneaky.exe
.PHONY : clean all run_sneaky run_terrain run_test dox 

run_terrain: $(TERRAIN_DEMO)
	cd $(TARGET_DIR); ./TerrainDemo.exe

dox: 
	rm -f -r ../codespear.github.io/gameex/html
	cd docs; doxygen doxy.txt

run_sneaky: $(SNEAKY)
	cd $(TARGET_DIR); ./Sneaky.exe

$(SNEAKY): $(OBJS) $(GAME_LAND_LIB)
	g++ $(wildcard Sneaky/*.o) -o $@ -lGameLandLib $(LINK_ARGS)  

$(TERRAIN_DEMO): $(OBJS) $(GAME_LAND_LIB)
	g++ $(wildcard TerrainDemo/*.o) -o $@ -lGameLandLib $(LINK_ARGS)  

run_tests:  $(GAME_LAND_TEST)
	cd $(TARGET_DIR); ./GameLandTests.exe

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