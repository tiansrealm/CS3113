#include "GameApp.h"

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

GameApp::GameApp() :
done(false), lastFrameTicks(0.0f), displayWindow(nullptr), program(nullptr) {

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
	glViewport(0, 0, 640, 360);
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);
	glUseProgram(program->programID);

	GLuint backGroundTexture = LoadTexture("backgrounds.png");
	SheetSprite* sprite = new SheetSprite(backGroundTexture, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	Entity* entity = new Entity(this, sprite);
	entities.push_back(entity);
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
		entities[i]->draw();
	}
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

		}
		else if (keys[SDL_SCANCODE_RIGHT]){

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
