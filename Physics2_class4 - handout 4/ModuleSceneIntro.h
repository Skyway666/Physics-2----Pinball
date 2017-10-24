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
	//Pinball sensors
	PhysBody* ball_throw; //type 0, sensor
	PhysBody* wall_sensor; //type 1, sensor

	//Bools
	bool allow_throw;
	bool wall_collision;

	bool sensed;

	// Textures
	SDL_Texture* background;
	SDL_Texture* ball_sprite;
	uint bonus_fx;
	uint boing;
	p2Point<int> ray;
	bool ray_on;
};
