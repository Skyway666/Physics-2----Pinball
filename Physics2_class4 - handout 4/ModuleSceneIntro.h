#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:


	p2List<PhysBody*> circles;


	// Pinball board
	PhysBody* Lpinball; //type -1, chain
	PhysBody* Rpinball; //type -1, chain
	PhysBody* Bpinball; //type -1, chain
	PhysBody* wall;     //type -1, chain
	//Flippers
	PhysBody* Lflipper; //type -1, rectangle
	PhysBody* Rflipper; //type -1, rectangle
	//Bouncers
	PhysBody* bouncer1; //type 1, circle
	PhysBody* bouncer2;
	//Ball
	PhysBody* ball; //type 0, body
	//Pinball sensors
	PhysBody* ball_throw; //type 0, sensor
	PhysBody* wall_sensor; //type 1, sensor

	//Bools
	bool allow_throw;
	bool wall_collision;

	bool sensed;

	// Textures
	SDL_Texture* background;
	SDL_Texture* sprites;

	// Sprites:

	// Cowboys
	//SDL_Rect P{ 0, 28, 43, 63 };
	//SDL_Rect E{ 0, 28, 43, 63 };
	//SDL_Rect P2{ 0, 28, 43, 63 };
	//SDL_Rect S{ 0, 28, 43, 63 };
	//SDL_Rect I{ 0, 28, 43, 63 };
	SDL_Rect S2 { 0, 28, 42, 61 };
	SDL_Rect A { 43, 28, 42, 61 };
	SDL_Rect L{ 86, 28, 40, 61 };
	SDL_Rect O{ 127, 28, 38, 57 };
	SDL_Rect O2{ 166, 28, 38, 57 };
	SDL_Rect N{ 205, 28, 38, 57 };

	int idk = 400;
	int idk2 = 360;
	bool idkk = false;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
