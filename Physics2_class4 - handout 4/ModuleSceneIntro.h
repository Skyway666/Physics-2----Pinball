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
	PhysBody* Lpinball; //type -1, body
	PhysBody* Rpinball; //type -1, body
	PhysBody* Bpinball; //type -1, body
	PhysBody* wall;     //type -1, body
	//Flippers
	PhysBody* Lflipper; //type -1, body
	PhysBody* Rflipper; //type -1, body
	//Bouncers
	PhysBody* bouncer1; //type 1, body
	PhysBody* bouncer2; //type 1, body
	PhysBody* bouncer3; //type 1, body
	//Barrells
	PhysBody* barrels_1; //type 2, body
	PhysBody* barrels_2; //type 2, body
	//Ball
	PhysBody* ball; //type 0, body
    //CowBoys
	PhysBody* cowboys[11]; //type 3, body
	//Pinball sensors
	PhysBody* ball_throw; //type 0, sensor
	PhysBody* wall_sensor; //type 1, sensor
	//Bools
	bool allow_throw;
	bool wall_collision;
	bool sensed;
	bool erase = false;
	// Textures
	SDL_Texture* background;
	SDL_Texture* sprites;

	// Sprites:

	// Cowboys
	SDL_Rect P { 0, 92, 41, 61 };
	SDL_Rect E { 41, 92, 41, 61 };
	SDL_Rect P2 { 83, 92, 41, 61 };
	SDL_Rect S { 123, 92, 41, 61 };
	SDL_Rect I { 164, 92, 41, 61 };
	SDL_Rect S2 { 0, 28, 42, 61 };
	SDL_Rect A { 43, 28, 42, 61 };
	SDL_Rect L { 86, 28, 40, 61 };
	SDL_Rect O { 127, 28, 38, 57 };
	SDL_Rect O2 { 166, 28, 38, 57 };
	SDL_Rect N { 205, 28, 38, 57 };
	SDL_Rect saloon { 1, 208, 91, 64 };

	int idk = 400;
	int idk2 = 360;
	bool idkk = false;

	SDL_Texture* ball_sprite;
	//Sounds
	uint bonus_fx;
	uint boing;
	uint paw;


	p2Point<int> ray;
	bool ray_on;
};
