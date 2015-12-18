#include "GameApp.h"
#include <fstream>

#include <iostream>
#include <sstream>

using namespace std;

GLuint LoadTexture(const char *image_path);
GLuint LoadTextureAlpha(const char *image_path);
//=====================================================================================================================

GameApp::GameApp() :
done(false), lastFrameTicks(0.0f), displayWindow(nullptr), shader(nullptr),
screenWidth(640), screenHeight(480){

	setup();
}
void GameApp::setup() {
	// SDL and OpenGL initialization
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Tian's HW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glViewport(0, 0, screenWidth, screenHeight);
	shader = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	//projectionMatrix.setOrthoProjection(-screenWidth / 2, screenWidth / 2, -screenHeight / 2, screenHeight / 2, -1.0f, 1.0f);
	projectionMatrix.setOrthoProjection(-screenWidth / 4, screenWidth / 4, -screenHeight / 4, screenHeight / 4, -1.0f, 1.0f);
	shader->setModelMatrix(modelMatrix);
	shader->setProjectionMatrix(projectionMatrix);
	shader->setViewMatrix(viewMatrix);
	glUseProgram(shader->programID);

	textures["player"] = LoadTextureAlpha("smiley.png");
	textures["flame"] = LoadTextureAlpha("FlameTest.png");
	textures["grid"] = LoadTextureAlpha("grid.png");
	player = new Entity(shader, new SheetSprite(textures["player"], 0.0f, 0.0f, 1.0f, 1.0f, 16.0f, 16.0f), 0, 50);
	entities.push_back(player);

	loadMap();
	grid = new Entity(shader, new SheetSprite(textures["grid"], 0.0f, 0.0f, 1.0f, 1.0f, 640.0f, 480.0f), -320, 240.0f);
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("music.mp3");
	//music = Mix_LoadMUS("music2.mp3");
	//Mix_PlayMusic(music, -1);
	sound = Mix_LoadWAV("Jump.wav");

	//test particles
	emitter = new ParticleEmitter(10, textures["flame"], shader);
	
}
GameApp::~GameApp() {
	// SDL and OpenGL cleanup (joysticks, textures, etc).
	Mix_FreeMusic(music);
	Mix_FreeChunk(sound);
	SDL_Quit();
	for (size_t i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	for (size_t i = 0; i < staticEntities.size(); i++) {
		delete staticEntities[i];
	}
}

void GameApp::ProcessEvents() {
	// check for input events
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN){
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				player->vel.y = 130;
				Mix_PlayChannel(-1, sound, 0);
			}
		}
	}

	if (keys[SDL_SCANCODE_LEFT]) {
		player->accel.x = -60;
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		player->accel.x = 60;
	}
}

void GameApp::Render() {
	// clear, render and swap the window
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	viewMatrix.identity();
	viewMatrix.Translate(-player->pos.x, 0, 0);
	shader->setViewMatrix(viewMatrix);
	for (size_t i = 0; i < entities.size(); i++) {
		shader->setModelMatrix(entities[i]->matrix);
		entities[i]->draw();
	}
	for (size_t i = 0; i < staticEntities.size(); i++) {
		shader->setModelMatrix(staticEntities[i]->matrix);
		staticEntities[i]->draw();
	}
	
	shader->setModelMatrix(modelMatrix);
	emitter->render();

	
	//grid
	//viewMatrix.identity();
	//shader->setViewMatrix(viewMatrix);
	shader->setModelMatrix(grid->matrix);
	grid->draw();
	
	SDL_GL_SwapWindow(displayWindow);
}

void GameApp::Update(float elapsed) {
	// move things based on time passed
	// check for collisions and respond to them

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->update(elapsed);
		for (size_t j = 0; j < staticEntities.size(); j++){
			entities[i]->collidesWith(*(staticEntities[j]), true);
		}

	}
	//for (size_t i = 0; i < staticEntities.size(); i++) {
	//	staticEntities[i]->update(elapsed);
	//}

	emitter->update(elapsed);

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
				SheetSprite* tileSprite = new SheetSprite(spriteSheetTexture, u, v, 16 / 256.0f, 16 / 128.0f, 16.0f, 16.0f);
				float test = x * TILE_SIZE - screenWidth / 2;
				float newX = x * 16 - screenWidth / 2;
				Entity* tile = new Entity(shader, tileSprite, newX, -y * 16 + screenHeight/2);
				tile->is_static = true;
				staticEntities.push_back(tile);
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
			float placeY = atoi(yPosition.c_str()) * -16 + screenHeight;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void GameApp::placeEntity(string& type, float x, float y){
	GLuint spriteSheetTexture = LoadTextureAlpha("arne_sprites.png");
	SheetSprite* enemySprite = new SheetSprite(spriteSheetTexture, 0.0f, 5.0/8.0f, 16/256.0f, 16/128.0f, 16.0f, 16.0f);
	Entity* enemy = new Entity(shader, enemySprite, x , y );
	entities.push_back(enemy);
}


//==================================================================================
