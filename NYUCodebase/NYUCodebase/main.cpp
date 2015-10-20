#include <iostream>
using namespace std;

#include "GameApp.h"






int main(int argc, char *argv[])
{
	GameApp app;
	while (!app.updateAndRender()){}
	
	return 0;
}