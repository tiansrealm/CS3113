#include "GameApp.h"


using namespace std;

GLuint LoadTexture(const char *image_path);
GLuint LoadTextureAlpha(const char *image_path);
//=====================================================================================================================

GameApp::GameApp() :
done(false), lastFrameTicks(0.0f), displayWindow(nullptr), shader(nullptr){

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
	//projectionMatrix.setOrthoProjection(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, -1.0f, 1.0f);
	projectionMatrix.setOrthoProjection(-SCREEN_WIDTH / 4, SCREEN_WIDTH / 4, -SCREEN_HEIGHT / 4, SCREEN_HEIGHT / 4, -1.0f, 1.0f);
	shader->setModelMatrix(modelMatrix);
	shader->setProjectionMatrix(projectionMatrix);
	shader->setViewMatrix(viewMatrix);
	glUseProgram(shader->programID);

	textures["player"] = LoadTextureAlpha("smiley.png");
	textures["flame"] = LoadTextureAlpha("FlameTest.png");
	textures["grid"] = LoadTextureAlpha("grid.png");


	currentState = new MapState("map1", this);
	gameStates["map1"] = currentState;

	player = new Entity(shader, new SheetSprite(textures["player"], 0.0f, 0.0f, 1.0f, 1.0f, 16.0f, 16.0f), 0, 50);
	currentState->entities.push_back(player);

	grid = new Entity(shader, new SheetSprite(textures["grid"], 0.0f, 0.0f, 1.0f, 1.0f, 640.0f, 480.0f), -320, 240.0f);
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("music.mp3");
	//music = Mix_LoadMUS("music2.mp3");
	//Mix_PlayMusic(music, -1);
	sound = Mix_LoadWAV("Jump.wav");

	//test particles
	//emitter = new ParticleEmitter(10, textures["flame"], shader);
	
}
GameApp::~GameApp() {
	// SDL and OpenGL cleanup (joysticks, textures, etc).
	Mix_FreeMusic(music);
	Mix_FreeChunk(sound);
	SDL_Quit();
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
	currentState->render();
	//grid
	//viewMatrix.identity();
	//shader->setViewMatrix(viewMatrix);
	shader->setModelMatrix(grid->matrix);
	grid->draw();
	
	SDL_GL_SwapWindow(displayWindow);
}

void GameApp::Update(float elapsed) {
	currentState->update(elapsed);
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
