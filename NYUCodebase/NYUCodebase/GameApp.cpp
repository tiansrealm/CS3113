#include "GameApp.h"
#include <fstream>

#include <iostream>
#include <sstream>

using namespace std;


GLuint LoadTexture(const char *image_path){
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}
GLuint LoadTextureAlpha(const char *image_path){
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

//=====================================================================================================================

GameApp::GameApp() :
done(false), lastFrameTicks(0.0f), displayWindow(nullptr), program(nullptr),
screenWidth(640), screenHeight(320){

	setup();
}
void GameApp::setup() {
	// SDL and OpenGL initialization
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Tian's HW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, screenWidth, screenHeight);
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	projectionMatrix.setOrthoProjection(-screenWidth/2, screenWidth/2, -screenHeight/2, screenHeight/2, -1.0f, 1.0f);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);
	glUseProgram(program->programID);
	GLuint playerTexture = LoadTextureAlpha("smiley.png");
	player = new Entity(this, SheetSprite(playerTexture, 0.0f, 0.0f, 1.0f, 1.0f, 16.0f), 0, 0);
	entities.push_back(player);
	
	loadMap();
}
GameApp::~GameApp() {
	// SDL and OpenGL cleanup (joysticks, textures, etc).
	SDL_Quit();
}
void GameApp::Render() {
	// clear, render and swap the window
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (size_t i = 0; i < entities.size(); i++) {
		program->setModelMatrix(entities[i]->matrix);
		entities[i]->draw();
	}
	viewMatrix.identity();
	viewMatrix.Translate(-player->x, -player->y, 0);
	program->setViewMatrix(viewMatrix);
	SDL_GL_SwapWindow(displayWindow);
}
void GameApp::ProcessEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		// check for input events
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_LEFT]){
			player->accel_x = -50;
		}
		if (keys[SDL_SCANCODE_RIGHT]){
			player->accel_x = 50;
		}
		if (keys[SDL_SCANCODE_UP]){
			player->velocity_y = 60;
		}
		if (keys[SDL_SCANCODE_DOWN]){
			//player->accel_y = -10;
		}
	}
}
void GameApp::Update(float elapsed) {
	// move things based on time passed
	// check for collisions and respond to them
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->update(elapsed);

	}
}

bool GameApp::updateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	ProcessEvents();
	Update(elapsed);
	Render();
	return done;
}

//===========================================load map
void GameApp::loadMap(){

	ifstream infile("testMap.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer]") {
			readEntityData(infile);
		}
	}
	//
	GLuint spriteSheetTexture = LoadTextureAlpha("arne_sprites.png");
	int SPRITE_COUNT_X = 16;
	int SPRITE_COUNT_Y = 8;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			int data = (int)mapData[y][x];
			if (data != 0){
				float u = (float)((data-1) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)((data-1) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				SheetSprite tileSprite = SheetSprite(spriteSheetTexture, u, v, 16 / 256.0f, 16 / 128.0f, 16.0f);
				//float test = x * TILE_SIZE;// - screenWidth / 2;
				float newX = x * 16 - screenWidth / 2;
				Entity* tile = new Entity(this, tileSprite, newX, -y * 16 + screenHeight/2);
				tile->stationary = true;
				entities.push_back(tile);
			}
		}
	}
}

bool GameApp::readHeader(std::ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		mapData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			mapData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool GameApp::readLayerData(std::ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						//tiles in this format are indexed from 1 not 0
						mapData[y][x] = val;
					}
					else {
						mapData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

bool GameApp::readEntityData(std::ifstream &stream) {
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
				float placeX = atoi(xPosition.c_str()) * 16 - screenWidth / 2;
			float placeY = atoi(yPosition.c_str()) * -16 + screenHeight /2 + 10;//10 extra to spawn on top platform
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void GameApp::placeEntity(string& type, float x, float y){
	GLuint spriteSheetTexture = LoadTextureAlpha("arne_sprites.png");
	SheetSprite enemySprite = SheetSprite(spriteSheetTexture, 0.0f, 5.0/8.0f, 16/256.0f, 16/128.0f, 16.0f);
	Entity* enemy = new Entity(this, enemySprite, x , y );
	entities.push_back(enemy);
}