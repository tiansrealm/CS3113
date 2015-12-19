#include "GameState.h"
using namespace std;

GameState::GameState(const string& name, GameApp *app) : name(name), app(app){
	shader = app->shader;
}


void GameState::render(){
	// clear and render 
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	setView();

	for (size_t i = 0; i < entities.size(); ++i) {
		app->shader->setModelMatrix(entities[i]->matrix);
		entities[i]->draw();
	}
	for (size_t i = 0; i < staticEntities.size(); ++i) {
		shader->setModelMatrix(staticEntities[i]->matrix);
		staticEntities[i]->draw();
	}
	
	Matrix model;
	for(size_t i = 0; i < emitters.size(); ++i){
		model.identity();
		model.Translate(emitters[i]->pos.x, emitters[i]->pos.y, 0);
		shader->setModelMatrix(model);
		emitters[i]->render();
	}
	
	
	app->displayText("Hello Tian", -100, 100, 16, 16, 2);
}

void GameState::update(float elapsed){
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

	for(size_t i = 0; i < emitters.size(); ++i){
		emitters[i]->update(elapsed);
	}
}
void GameState::setView(){
	Matrix viewMatrix;
	shader->setViewMatrix(viewMatrix);
}

//=================================================================================================================
MapState::MapState(const string& name, GameApp *app) : GameState(name, app){
	loadMap();
}

void MapState::setView(){
	Matrix viewMatrix;
	viewMatrix.Translate(-app->player->pos.x, 0, 0);
	shader->setViewMatrix(viewMatrix);
}



//=======================================================================
void MapState::loadMap(){

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
				float test = x * TILE_SIZE - SCREEN_WIDTH / 2;
				float newX = x * 16 - SCREEN_WIDTH / 2;
				Entity* tile = new Entity(shader, tileSprite, newX, -y * 16 + SCREEN_HEIGHT/2);
				tile->is_static = true;
				staticEntities.push_back(tile);
			}
		}
	}
}


bool MapState::readHeader(std::ifstream &stream) {
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

bool MapState::readLayerData(std::ifstream &stream) {
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

bool MapState::readEntityData(std::ifstream &stream) {
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
			float placeX = atoi(xPosition.c_str()) * 16 - SCREEN_WIDTH / 2;
			float placeY = atoi(yPosition.c_str()) * -16 + SCREEN_HEIGHT;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void MapState::placeEntity(string& type, float x, float y){
	GLuint spriteSheetTexture = LoadTextureAlpha("arne_sprites.png");
	SheetSprite* enemySprite = new SheetSprite(spriteSheetTexture, 0.0f, 5.0/8.0f, 16/256.0f, 16/128.0f, 16.0f, 16.0f);
	Entity* enemy = new Entity(shader, enemySprite, x , y );
	entities.push_back(enemy);
}


