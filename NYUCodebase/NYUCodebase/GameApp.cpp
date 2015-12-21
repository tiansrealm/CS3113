#include "GameApp.h"


using namespace std;

GLuint LoadTexture(const char *image_path);
GLuint LoadTextureAlpha(const char *image_path);
//=====================================================================================================================

GameApp::GameApp() :
done(false), lastFrameTicks(0.0f), displayWindow(nullptr), shader(nullptr), 
orthoWidth(SCREEN_WIDTH), orthoHeight(SCREEN_HEIGHT){
	setup();
}
void GameApp::setup() {
	// SDL and OpenGL initialization
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Tian's HW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	shader = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	projectionMatrix.setOrthoProjection(-ORTHO_WIDTH / 2, ORTHO_WIDTH / 2, -ORTHO_HEIGHT / 2, ORTHO_HEIGHT / 2, -1.0f, 1.0f);
	//projectionMatrix.setOrthoProjection(-SCREEN_WIDTH / 4, SCREEN_WIDTH / 4, -SCREEN_HEIGHT / 4, SCREEN_HEIGHT / 4, -1.0f, 1.0f);
	shader->setModelMatrix(modelMatrix);
	shader->setProjectionMatrix(projectionMatrix);
	shader->setViewMatrix(viewMatrix);
	glUseProgram(shader->programID);

	aniTime = 0.0f;

	textures["player"] = LoadTextureAlpha("sprites/smiley.png");
	textures["flame"] = LoadTextureAlpha("sprites/FlameTest.png");
	textures["grid"] = LoadTextureAlpha("sprites/grid.png");
	textures["font"] = LoadTextureAlpha("sprites/font.png");
	textures["button"] = LoadTextureAlpha("sprites/button.png");
	textures["star7"] = LoadTextureAlpha("sprites/star7.png");
	textures["tileMap"]= LoadTextureAlpha("sprites/tileMap.png");
	textures["hpBar"]= LoadTextureAlpha("sprites/hpBar.png");
	textures["portal"]= LoadTextureAlpha("sprites/portal.png");
	textures["fireball"]= LoadTextureAlpha("sprites/fireball.png");

	textures["wizardStandR"]= LoadTextureAlpha("sprites/wizardStandRight.png");
	textures["wizardJump R"]= LoadTextureAlpha("sprites/wizardJumpRight.png");
	textures["wizardAtt  R"]= LoadTextureAlpha("sprites/wizardAttRight.png");
	textures["wizardHurt R"]= LoadTextureAlpha("sprites/wizardHurtRight.png");
	textures["wizardStandL"]= LoadTextureAlpha("sprites/wizardStandLeft.png");
	textures["wizardJump L"]= LoadTextureAlpha("sprites/wizardJumpLeft.png");
	textures["wizardAtt  L"]= LoadTextureAlpha("sprites/wizardAttLeft.png");
	textures["wizardHurt L"]= LoadTextureAlpha("sprites/wizardHurtLeft.png");
	loadSprites();
	starEmitter = new ParticleEmitter(25, textures["star7"], shader);
	fireballEmitter = new ParticleEmitter(30, textures["fireball"], shader);
	fireballEmitter->startSize = 10.0f;
	fireballEmitter->endSize = 40.0f;
	fireballEmitter->maxLifetime = 3.0f;
	fireballEmitter->is_on = false;
	fireballEmitter->gravity.y = 0.0f;
	fireballEmitter->vel.x = 100.0;
	fireballEmitter->vel.y = 0.0f;
	fireballEmitter->resetParticles();

	cursor = new Entity(shader, sprites["star7"], 0, 0, false);
	player = new Entity(shader, sprites["wizardStandR"], 50, 100, false);
	player->hp = 100;


	
	loadStates();
	

	

	grid = new Entity(shader, new SheetSprite(textures["grid"], 0.0f, 0.0f, 1.0f, 1.0f, 640.0f, 480.0f), -320, 240.0f);
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	musics["jungle music"] = Mix_LoadMUS("sounds/jungle music.mp3");
	//music = Mix_LoadMUS("music2.mp3");
	Mix_PlayMusic(musics["jungle music"], -1);
	sounds["whoosh"] = Mix_LoadWAV("sounds/whoosh.wav");

	
}
GameApp::~GameApp() {
	// SDL and OpenGL cleanup (joysticks, textures, etc).
	for (auto& iter: musics){
		Mix_FreeMusic(iter.second);
	}
	for (auto& iter: sounds){
		Mix_FreeChunk(iter.second);
	}
	SDL_Quit();
}

void GameApp::ProcessEvents() {
	// check for input events
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_Event event;
	string state = currentState->name;
	int pixelX, pixelY;
	SDL_GetMouseState(&pixelX, &pixelY);
	int mouseX = (pixelX * orthoWidth / SCREEN_WIDTH) - orthoWidth / 2;
	int orthoY =  pixelY * orthoHeight / SCREEN_HEIGHT;
	int mouseY = (orthoHeight / 2 - orthoY);

	cursor->setPos(mouseX, mouseY);

	GameState* nextState = currentState;
	starEmitter->pos.x = mouseX/2;
	starEmitter->pos.y = mouseY/2 - 20;
	fireballEmitter->vel.x = 70.0;
	fireballEmitter->pos.x = player->pos.x/2 + player->width/2;
	if(player->vel.x < 0){
		fireballEmitter->pos.x = player->pos.x/2 - player->width/2;
		fireballEmitter->vel.x = -70.0;
		//fireballEmitter->textureID = textures["fireball_left"];
	}
	fireballEmitter->pos.y = player->pos.y/2 - player->height/4;
	
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (event.type == SDL_KEYDOWN){
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				if(state.substr(0,3) == "map"){
					if(player->collideBot) { 
						player->vel.y = 160; 
						Mix_PlayChannel(-1, sounds["whoosh"], 0);
					}
				}
			}


			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if(state.substr(0,3) == "map" && aniTime == 0){
					fireballEmitter->is_on = true;
					fireballEmitter->resetParticles();
				}
			}
		}
		if(event.type == SDL_MOUSEBUTTONDOWN) {       
			if(state == "start" ){
				if (currentState->ghostEntities["quitButton"]->collidesWith(cursor->pos.x, cursor->pos.y)){
					done = true;
				}
				else if(currentState->ghostEntities["startButton"]->collidesWith(cursor->pos.x, cursor->pos.y)){
					nextState = gameStates["map1"];
				}
			}
			else if(state == "map1" || state == "map2" || state == "map3"){
				if (currentState->ghostEntities["pauseButton"]->collidesWith(cursor->pos.x, cursor->pos.y)){
					nextState = gameStates["pause"];
				}
			} 
			else if(state == "pause"){
				if (currentState->ghostEntities["resumeButton"]->collidesWith(cursor->pos.x, cursor->pos.y)){
					nextState = gameStates["map1"];
				}
				if (currentState->ghostEntities["quitButton"]->collidesWith(cursor->pos.x, cursor->pos.y)){
					done = true;
				}
			} 
		}
		if(state == "map1"){
			if (player->collidesWith(*currentState->namedEntities["portal"])){
					nextState = gameStates["map2"];
					player->setPos(50, 120);
					player->vel.x = 0;
					player->vel.y = 0;
			}
		}
		if(state == "map2"){
			if (player->collidesWith(*currentState->namedEntities["portal2"])){
					nextState = gameStates["map3"];
					player->setPos(50, 120);
					player->vel.x = 0;
					player->vel.y = 0;
			}
		}
		if(state == "map3"){
			if (player->collidesWith(*currentState->namedEntities["portal"])){
					nextState = gameStates["win"];
			}
		}
		
	}

	if (keys[SDL_SCANCODE_LEFT]) {
		if(state.substr(0,3) == "map"){
			player->accel.x = -120;
		}
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		if(state.substr(0,3) == "map"){
			player->accel.x = 120;
		}
	}
	if(currentState != nextState){
		currentState = nextState;
		if(currentState->name == "map1"){
		//	Mix_PlayMusic(musics["m1"], -1);
		}
	}
}

void GameApp::Render() {
	currentState->render();

	//shader->setModelMatrix(grid->matrix);
	//grid->draw();

	viewMatrix.identity();
	shader->setViewMatrix(viewMatrix);
	if (currentState->name.substr(0,3) == "map"){
		displayText(to_string(player->hp), 180-orthoWidth/2,  400 - orthoHeight/2, 16, 16, 10);
	}
	shader->setModelMatrix(cursor->matrix);
	cursor->draw();
	SDL_GL_SwapWindow(displayWindow);
}

void GameApp::Update(float elapsed) {
	if(elapsed > .1f){elapsed = .1f;}
	currentState->update(elapsed);
	//player animation update
	string sprite = "wizard";
	string orientation, action;
	if(player->vel.x >= 0){ 	orientation = "R";
	}else{ 						orientation = "L"; }
	if(player->collideBot){
		action = "Stand";
	}else{
		action = "Jump ";
	}
	if(aniTime > 0){
		action = "Att  ";
	}
	player->sprite = sprites[sprite + action + orientation];

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

void GameApp::displayText(const string s, float x, float y, float w, float h, float spacing){
	shader->setViewMatrix(Matrix());
	Matrix model;
	model.Translate(x, y, 0);
	SheetSprite temp;
	viewMatrix.identity();
	shader->setViewMatrix(viewMatrix);
	for (size_t i = 0; i < s.length(); i++){
		temp = *sprites[s.substr(i, 1)];
		temp.width = w;
		temp.height = h;
		shader->setModelMatrix(model);
		temp.draw(shader);
		model.Translate(w + spacing, 0, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameApp::loadSprites(){
	//fonts
	vector<string> symbols = {
		" ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/",
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?",
		"@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
		"P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_",
		"`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
		"p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~"
	};

	for(size_t i = 0; i < symbols.size(); ++i ){
		int row = i / 16;
		int col = i % 16;
		sprites[symbols[i]] = new SheetSprite(textures["font"], row, col, 32, 32, 
												512, 192, 32, 32);
	}

	//tilemap
	int tileWidth = 19, tileHeight = 6;
	int tileNum = tileWidth * tileHeight;
	for(int i = 0; i < tileNum; ++i ){
		int row = i / tileWidth;
		int col = i % tileWidth;
		sprites["tile" + to_string(i)] = new SheetSprite(textures["tileMap"], row, col, 32, 32,
												608, 192, 32, 32);
	}
	//
	sprites["button"] = new SheetSprite(textures["button"], 0.0f, 0.0f, 1.0f, 1.0f, 60.0f, 23.0f);
	sprites["star7"] = new SheetSprite(textures["star7"], 0.0f, 0.0f, 1.0f, 1.0f, 32.0f, 32.0f);
	sprites["portal"] = new SheetSprite(textures["portal"], 0.0f, 0.0f, 1.0f, 1.0f, 64.0f, 100.0f);
	sprites["fireball"] = new SheetSprite(textures["fireball_sheet"], 0.0f, 0.0f, 1.0f, 1.0f, 94.0f, 60.0f);


	sprites["wizardStandR"] = new SheetSprite(textures["wizardStandR"], 0.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
	sprites["wizardStandL"] = new SheetSprite(textures["wizardStandL"], 0.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
	sprites["wizardJump R"] = new SheetSprite(textures["wizardJump R"], 0.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
	sprites["wizardJump L"] = new SheetSprite(textures["wizardJump L"], 0.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
	sprites["wizardAtt  R"] = new SheetSprite(textures["wizardAtt  R"], 0.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
	sprites["wizardAtt  L"] = new SheetSprite(textures["wizardAtt  L"], 0.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
	sprites["wizardHurt R"] = new SheetSprite(textures["wizardHurt R"], 0.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
	sprites["wizardHurt L"] = new SheetSprite(textures["wizardHurt L"], 0.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
}



void GameApp::loadStates(){
	
	gameStates["start"] =  new GameState("start", this);
	gameStates["pause"] =  new GameState("pause", this);
	gameStates["win"] =  new GameState("pwin", this);
	gameStates["map1"] = new MapState("map1", this);
	gameStates["map2"] = new MapState("map2", this);
	gameStates["map3"] = new MapState("map3", this);
	//Start state
	gameStates["start"]->textDatas.push_back(TextData("Start Game", -160, 112, 32, 32));
	gameStates["start"]->textDatas.push_back(TextData("Quit", -64, -32, 32, 32));

	Entity* startButton = new Entity( shader, sprites["button"], -186, 128);
	startButton->setWidthHeight(370, 64);
	Entity* quitButton = new Entity( shader, sprites["button"], -90, -16);
	quitButton->setWidthHeight(190, 64);

	//Add entities
	gameStates["start"]->ghostEntities["startButton"] = startButton;
	gameStates["start"]->ghostEntities["quitButton"] = quitButton;
	
	gameStates["win"]->textDatas.push_back(TextData("YOU WIN!!!", -160, 112, 40, 40));
	//map
	gameStates["map1"]->textDatas.push_back(
		TextData("Pause",
			475-orthoWidth/2,  434 - orthoHeight/2, 16, 16));


	gameStates["map1"]->textDatas.push_back(
		TextData("HP: ", 
			40-orthoWidth/2,  440 - orthoHeight/2, 36, 36));

	gameStates["map1"]->textDatas.push_back(
		TextData("Arrows Key to move. Space to Fire.", 
			40-orthoWidth/2,  350 - orthoHeight/2, 16, 16));
	gameStates["map1"]->textDatas.push_back(
		TextData("Reach the portal to advance.", 
			40-orthoWidth/2,  330 - orthoHeight/2, 16, 16));
	gameStates["map2"]->textDatas.push_back(
		TextData("Pause",
			475-orthoWidth/2,  434 - orthoHeight/2, 16, 16));


	gameStates["map2"]->textDatas.push_back(
		TextData("HP: ", 
			40-orthoWidth/2,  440 - orthoHeight/2, 36, 36));	
	gameStates["map2"]->textDatas.push_back(
		TextData("Arrows Key to move. Space to Fire.", 
			40-orthoWidth/2,  350 - orthoHeight/2, 16, 16));
	gameStates["map2"]->textDatas.push_back(
		TextData("Reach the portal to advance.", 
			40-orthoWidth/2,  330 - orthoHeight/2, 16, 16));

	gameStates["map3"]->textDatas.push_back(
		TextData("Pause",
			475-orthoWidth/2,  434 - orthoHeight/2, 16, 16));


	gameStates["map3"]->textDatas.push_back(
		TextData("HP: ", 
			40-orthoWidth/2,  440 - orthoHeight/2, 36, 36));
	gameStates["map3"]->textDatas.push_back(
		TextData("Arrows Key to move. Space to Fire.", 
			40-orthoWidth/2,  350 - orthoHeight/2, 16, 16));
	gameStates["map3"]->textDatas.push_back(
		TextData("Reach the portal to win.", 
			40-orthoWidth/2,  330 - orthoHeight/2, 16, 16));

	Entity* hpBar = new Entity( shader, 
		new SheetSprite(textures["hpBar"], 0.0f, 0.0f, 1.0f, 1.0f, 360.0f, 36.0f)
		, 120-orthoWidth/2,  440 - orthoHeight/2);

	Entity* pauseButton = new Entity( shader, sprites["button"], 470-orthoWidth/2,  440 - orthoHeight/2);
	pauseButton->setWidthHeight(100, 32);

	Entity* portal1 = new Entity( shader, sprites["portal"], 1760,  160, false );
	Entity* portal2 = new Entity( shader, sprites["portal"], 540,  480, false );
	gameStates["map1"]->ghostEntities["hpBar"] = hpBar;
	gameStates["map1"]->entities.push_back(player);
	gameStates["map1"]->ghostEntities["pauseButton"] = pauseButton;

	gameStates["map1"]->entities.push_back(portal1);
	gameStates["map1"]->namedEntities["portal"] = portal1;

	gameStates["map2"]->ghostEntities["hpBar"] = hpBar;
	gameStates["map2"]->entities.push_back(player);
	gameStates["map2"]->ghostEntities["pauseButton"] = pauseButton;

	gameStates["map2"]->entities.push_back(portal2);
	gameStates["map2"]->namedEntities["portal2"] = portal2;

	gameStates["map3"]->ghostEntities["hpBar"] = hpBar;
	gameStates["map3"]->entities.push_back(player);
	gameStates["map3"]->ghostEntities["pauseButton"] = pauseButton;

	gameStates["map3"]->entities.push_back(portal1);
	gameStates["map3"]->namedEntities["portal"] = portal1;
	//pause

	Entity* resumeButton = new Entity( shader, sprites["button"], 
		-112,  198 );
	resumeButton->setWidthHeight(252, 64);

	gameStates["pause"]->textDatas.push_back(
		TextData("Resume", -96,  182 , 32, 32));
	gameStates["pause"]->textDatas.push_back(TextData("Quit", -64, -32, 32, 32));
	
	gameStates["pause"]->ghostEntities["resumeButton"] = resumeButton;
	gameStates["pause"]->ghostEntities["quitButton"] = quitButton;


	currentState = gameStates["start"];
	//test particles
	
	for(auto& iter : gameStates){
		iter.second->emitters.push_back(starEmitter);
	}
	//currentState->emitters.push_back(new ParticleEmitter(10, textures["flame"], shader));
}