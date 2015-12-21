#include "GameState.h"
using namespace std;

GameState::GameState(const string& name, GameApp *app) : name(name), app(app){
	shader = app->shader;
}


void GameState::render(){
	// clear and render 
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
	for (const auto & i : ghostEntities)
	{
		shader->setModelMatrix(i.second->matrix);
		i.second->draw();
	}
	Matrix model;
	for (size_t i = 0; i < emitters.size(); ++i){
		model.identity();
		model.Translate(emitters[i]->pos.x, emitters[i]->pos.y, 0);
		shader->setModelMatrix(model);
		emitters[i]->render();
	}
	for (TextData td : textDatas){
		app->displayText(td.text, td.x, td.y, td.font_w, td.font_h, td.spacing);
	}
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
	for (size_t i = 0; i < emitters.size(); ++i){
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
	float x = app->player->pos.x;
	float y = app->player->pos.y;
	if(x < app->orthoWidth/2){
		x = app->orthoWidth/2;
	}
	if(y < app->orthoHeight/2){
		y = app->orthoHeight/2;
	}
	viewMatrix.Translate(-x, -y, 0);
	shader->setViewMatrix(viewMatrix);
}
//=======================================================================
void MapState::loadMap(){
	ifstream infile("tileMap.txt");
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
		else if (line == "[Object Layer 1]") {
			readEntityData(infile);
		}
	}
	//


	/*
	int SPRITE_COUNT_X = 19;
	int SPRITE_COUNT_Y = 6;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			int data = (int)mapData[y][x];
			if (data != 0){
				float u = (float)((data - 1) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)((data - 1) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				SheetSprite* tileSprite = new SheetSprite(textures["tileMap"], u, v, 16 / 256.0f, 16 / 128.0f, TILE_SIZE, TILE_SIZE);
				float newX = x * TILE_SIZE - SCREEN_WIDTH / 2;
				float newY = -y * TILE_SIZE + SCREEN_HEIGHT / 2;
				staticEntities.push_back(new Entity(shader, tileSprite, newX, newY));
			}
		}
	}*/
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			int data = (int)mapData[y][x];
			if (data != 0){
				string tileNum = "tile" + to_string(data - 1);
				SheetSprite* tileSprite = app->sprites["tile" + to_string(data - 1)];
				float newX = x * TILE_SIZE;
				float newY = -y * TILE_SIZE + SCREEN_HEIGHT;
				Entity*  test = new Entity(shader, tileSprite, newX, newY);
				staticEntities.push_back(new Entity(shader, tileSprite, newX, newY));

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
			float placeX = atoi(xPosition.c_str()) * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) * -TILE_SIZE + SCREEN_HEIGHT;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}
void MapState::placeEntity(string& type, float x, float y){

	SheetSprite* tileSprite = app->sprites[type];
	Entity*  entity = new Entity(shader, tileSprite, x, y + 1, false);

	entities.push_back(entity);
}
