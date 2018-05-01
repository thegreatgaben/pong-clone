CXX = g++ -std=c++11
CXXFLAGS = -Wall \
		   -Wfatal-errors

FILES = main.cpp \
		game.cpp \
		ball.cpp \
		paddle.cpp \
		texture.cpp

LDFLAGS = -lSDL2 \
		  -lSDL2_ttf \
		  -lSDL2_mixer

APPNAME = pong.sdl

all: $(APPNAME)

$(APPNAME): $(FILES)
	$(CXX) $(CXXFLAGS) -o $(APPNAME) $(FILES) $(LDFLAGS)
