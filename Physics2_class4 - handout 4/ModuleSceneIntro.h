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
	PhysBody* pinball; //type -1, body
	//Activable walls
	PhysBody* wall1;     //type -1, body
	PhysBody* wall2;     //type -1, body
	PhysBody* railing;     //type -1, body
	PhysBody* barrow;     //type -1, body
	PhysBody* barrow1;     //type -1, body
	//Obstacles
	PhysBody* obstacle1; //type -1, body
	PhysBody* obstacle2; //type -1, body
	//Flippers
	PhysBody* Lflipper; //type -1, body
	PhysBody* Rflipper; //type -1, body
	PhysBody* Sflipper; //type -1,body
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
	PhysBody* ball_throw1; //type 1, sensor
	PhysBody* ball_throw2; //type 1, sensor
	PhysBody* ball_throw3; //type 1, sensor
	PhysBody* wall_sensor; //type 2, sensor
	PhysBody* bonus_sensorL;//type 3,sensor
	PhysBody* bonus_sensorC;//type 3,sensor
	PhysBody* bonus_sensorR;//type 3,sensor
	PhysBody* end_game; //type 4, sensor
	PhysBody* ball_throw4; //type 5, sensor
    //Score controlling variables
	int score_mult;
	int total_score;
	int actual_score;
	int lives;
	timer score_timer;
	timer score_timer_barrow;
	int font_score = -1;
	char score_text[20];
	char balls_text[5];
	// Bools
	bool allow_throw;
	bool allow_throw2;
	bool allow_throw3;
	bool wall_collision;
	bool cowboys_killed;
	bool erase = false;
	bool horsekick = false;
	// Integers
	int horse_counter = 0;
	// Textures
	SDL_Texture* background;
	SDL_Texture* sprites;
	SDL_Texture* front;
	int idk = 500;
	int idk2 = 500;
	bool idkk = true;

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

	SDL_Texture* ball_sprite;
	SDL_Texture* flipper_sprite;
	//Sounds
	uint bonus_fx;
	uint boing;
	uint paw;
	uint lose_sound;
	uint kill_cowboys;
	uint level_1;
	uint level_2;
	uint level_3;
	uint level_4;
	uint maximum;
	uint horse_sound;
	uint you_lose;
	uint lose_melody;
	uint amazing;

	timer music_stop;

	p2Point<int> ray;
	bool ray_on;
};
