APPBIN = dungenon
WEBBIN = dungenon.html

APPCXX = g++
WEBCXX = em++

MOD = modsrc
MODBIN = modbin

MODULES= $(wildcard $(MOD)/*.cpp)

EXPORTED = -s EXPORTED_FUNCTIONS="['_main','_resizeCanvas']"

MODFILESAPP = $(MODULES:$(MOD)/%.cpp=$(MODBIN)/%.so)
MODFILESWEB = $(MODULES:$(MOD)/%.cpp=$(MODBIN)/%.wasm)

APPINCLUDE = -I /usr/include -I /usr/local/include -I ./include
WEBINCLUDE = -I /usr/share/emscripten/cache/sysroot/include -I /home/oneleggedguy/Code/raylib/src -I ./include

LIBS = -lraylib
WEBLIBDIR = -L /home/oneleggedguy/Code/raylib/src/web
APPLIBDIR = -L /usr/local/lib

WEBOUTDIR = ../server

APPOPTIONS = -std=c++17
WEBOPTIONS = -s FORCE_FILESYSTEM=1 -s TOTAL_MEMORY=67108864 -s USE_GLFW=3 -s ASYNCIFY -s EXPORTED_RUNTIME_METHODS=ccall -s MAIN_MODULE=2 -DPLATFORM_WEB -Os -Wall -s FETCH --shell-file ./html_template/shell_ray.html -std=c++17 -s $(EXPORTED)

SRCDIR = ./src

WEBSRCFILES = $(SRCDIR)/main.cpp $(SRCDIR)/network.cpp $(SRCDIR)/model.cpp $(SRCDIR)/gui.cpp
APPSRCFILES = $(SRCDIR)/main.cpp $(SRCDIR)/model.cpp $(SRCDIR)/gui.cpp

appbuild: $(APPBIN)

webbuild: $(WEBBIN)

appmodules: $(MODFILESAPP)

webmodules: $(MODFILESWEB)

appall: appbuild appmodules

weball: webbuild webmodules

.PHONY: appbuild webbuild appmodules webmodules appall weball

$(APPBIN) : $(APPSRCFILES)
	$(APPCXX) $(APPSRCFILES) -o $(APPBIN) $(APPOPTIONS) $(APPINCLUDE) $(APPLIBDIR) $(LIBS)

$(WEBBIN) : $(WEBSRCFILES)
	$(WEBCXX) $(WEBSRCFILES) -o $(WEBOUTDIR)/$(WEBBIN) $(WEBOPTIONS) $(WEBINCLUDE) $(WEBLIBDIR) $(LIBS) 

$(MODFILESAPP): $(MODBIN)/%.so: $(MOD)/%.cpp
	$(APPCXX) -shared -fPIC $< -o $@ $(APPINCLUDE)

$(MODFILESWEB): $(MODBIN)/%.wasm: $(MOD)/%.cpp
	$(WEBCXX) $< --no-entry -o $@ -s SIDE_MODULE=2 $(WEBINCLUDE)