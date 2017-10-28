#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;
class timer
{
public:
	void Start(int amount_of_time) //Amount of time in seconds
	{
		timer_duration = SDL_GetTicks() + amount_of_time * 1000;
		timer_active = true;
	}
	bool IsOver()
	{
		if(timer_active)
		{ 
			if (timer_duration > SDL_GetTicks())
			{ 
				return false;
			}
			else
			{ 
				timer_active = false;
				return true;
			}
		}
		return true;
	} 
private:
	int timer_duration;
	bool timer_active;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Reset_Small_Game();
	void Reset_Big_Game();

public:


	p2List<PhysBody*> circles;


	// Pinball board
	PhysBody* Lpinball; //type -1, body
	PhysBody* Rpinball; //type -1, body
	PhysBody* Bpinball; //type -1, body
	PhysBody* wall1;     //type -1, body
	PhysBody* wall2;
	PhysBody* obstacle1; //type -1, body
	PhysBody* obstacle2; //type -1, body
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
	PhysBody* bonus_sensorL;//type 2,sensor
	PhysBody* bonus_sensorC;//type 2,sensor
	PhysBody* bonus_sensorR;//type 2,sensor
	PhysBody* end_game; //type 3, sensor
    //Score controlling variables
	int score_mult;
	int total_score;
	int actual_score;
	int lives;
	timer score_timer;
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
	SDL_Rect sombrero { 222, 218, 45, 44 };
	SDL_Rect horse { 92, 208, 61, 118};
	SDL_Rect flag1 { 1, 153, 79, 53 };
	SDL_Rect flag2 { 83, 153, 59, 52 };
	SDL_Rect flag3 { 144, 154, 50, 53 };
	SDL_Rect x10 { 0, 0, 37, 27 };
	SDL_Rect hold { 37, 0, 37, 27 };
	SDL_Rect x6 { 74, 0, 37, 27 };
	SDL_Rect x4 { 111, 0, 37, 27 };
	SDL_Rect x8 { 148, 0, 37, 27 };
	SDL_Rect x2 { 185, 0, 37, 27 };

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
