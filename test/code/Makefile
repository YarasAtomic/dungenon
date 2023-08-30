APPBIN = dungenon
WEBBIN = dungenon.html

APPCXX = g++
WEBCXX = em++

MOD = modules
MODBIN = modbin

MODULES= $(wildcard $(MOD)/*.cpp)

MODFILESAPP = $(MODULES:$(MOD)/%.cpp=$(MODBIN)/%.so)
MODFILESWEB = $(MODULES:$(MOD)/%.cpp=$(MODBIN)/%.wasm)

APPINCLUDE = -I /usr/include -I /usr/local/include -I ./include
WEBINCLUDE = -I /usr/share/emscripten/cache/sysroot/include -I /home/oneleggedguy/Code/raylib/src -I ./include

LIBS = -lraylib
WEBLIBDIR = -L /home/oneleggedguy/Code/raylib/src/web
APPLIBDIR = -L /usr/local/lib

WEBOUTDIR = ../server

APPOPTIONS = -std=c++17
WEBOPTIONS = -s FORCE_FILESYSTEM=1 -s TOTAL_MEMORY=67108864 -s USE_GLFW=3 -s ASYNCIFY -s EXPORTED_RUNTIME_METHODS=ccall -s MAIN_MODULE=2 -DPLATFORM_WEB -Os -Wall -s FETCH --shell-file html_template/shell_ray.html -std=c++17
# WEBOPTIONS = -s FORCE_FILESYSTEM=1 -s TOTAL_MEMORY=67108864 -s USE_GLFW=3 -s ASYNCIFY -s EXPORTED_RUNTIME_METHODS=ccall -s -DPLATFORM_WEB -Os -Wall -s FETCH --shell-file html_template/shell_ray.html -std=c++17 -fsanitize=address
# RAYOPTIONS = --preload-file Sounds --preload-file Graphics -DPLATFORM_WEB

appbuild: $(APPBIN)

webbuild: $(WEBBIN)

appmodules: $(MODFILESAPP)

webmodules: $(MODFILESWEB)

appall: appbuild appmodules

weball: webbuild webmodules

.PHONY: appbuild webbuild appmodules webmodules appall weball

$(APPBIN) : main.cpp
# $(APPCXX) main.cpp -o $(APPBIN) $(APPOPTIONS) $(APPINCLUDE)
	$(APPCXX) main.cpp -o $(APPBIN) $(APPOPTIONS) $(APPINCLUDE) $(APPLIBDIR) $(LIBS)

$(WEBBIN) : main.cpp network.cpp
	$(WEBCXX) main.cpp network.cpp -o $(WEBOUTDIR)/$(WEBBIN) $(WEBOPTIONS) $(WEBINCLUDE) $(WEBLIBDIR) $(LIBS) 

$(MODFILESAPP): $(MODBIN)/%.so: $(MOD)/%.cpp
	$(APPCXX) -shared -fPIC $< -o $@ $(APPINCLUDE)

$(MODFILESWEB): $(MODBIN)/%.wasm: $(MOD)/%.cpp
	$(WEBCXX) $< --no-entry -o $@ -s SIDE_MODULE=2