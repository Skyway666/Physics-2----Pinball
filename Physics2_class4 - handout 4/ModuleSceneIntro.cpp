#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	allow_throw = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Load Assets
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	boing = App->audio->LoadFx("pinball/boing.wav");
	paw = App->audio->LoadFx("pinball/paw.wav");
	lose_sound = App->audio->LoadFx("pinball/lose.wav");
	kill_cowboys = App->audio->LoadFx("pinball/kill_cowboys.wav");
	multiply = App->audio->LoadFx("pinball/multiply.wav");

	background = App->textures->Load("pinball/background.png");
	sprites = App->textures->Load("pinball/sprites.png");
	ball_sprite = App->textures->Load("pinball/wheel.png");
	flipper_sprite = App->textures->Load("pinball/flipper.png");

	font_score = App->fonts->Load("pinball/Font.png", "> ?@ABCDEFGHIJKLMNOPQRSTUVWXYZ!¡?_^#$%&'()x+.-,;tpsczpc/0123456789:", 1);

	// Set up pinball board
	int Pinball_box[50] = {
		1190, 740,
		1190, 700,
		1300,650,
		1200, 482,
		1170, 310,
		1130, 200,
		1030, 160,
		975, 180,
		870, 137,
		350, 425,
		318, 392,
		90, 505,
		165, 660,
		250, 746,
		425, 856,
		425, 1200,
		750, 1200,
		750, 856,
		897, 755,
		982, 293,
		980, 220,
		1036, 203,
		1115, 286,
		1155, 463,
		1144, 740
	};

	int Wall1[8] = {
		416, 0,
		416, 100,
		430, 100,
		430, 0
	};

	int Wall2[8] = {
		1190, 740,
		1200, 482,
		1190, 740,
		1200, 482,
	};

	for (int i = 0; i != 8; i++)
	{
		Wall1[i] = Wall1[i] * 2.5;
	}

	pinball = App->physics->CreateChain(0, 0, Pinball_box, 50, b2_staticBody, -1);

	Lflipper = App->physics->CreateRectangle(170 * 2.5, (352 * 2.5), 180, 35,b2_dynamicBody,-1);

	Rflipper = App->physics->CreateRectangle(300 * 2.5, (352 * 2.5), 180, 35, b2_dynamicBody ,-1);

	Sflipper = App->physics->CreateRectangle(1000, 529, 180, 35, b2_dynamicBody, -1);

	wall1 = App->physics->CreateChain(0, 0, Wall1, 8, b2_staticBody,-1, true);

	wall2 = App->physics->CreateChain(0, 0, Wall2, 8, b2_staticBody, -1, true);

	obstacle1 = App->physics->CreateRectangle(838, 219, 10, 35, b2_staticBody, -1);

	obstacle2 = App->physics->CreateRectangle(910, 237, 10, 35, b2_staticBody, -1);

	//Set up bouncers
	bouncer1 = App->physics->CreateCircle(760, 279, 25, b2_staticBody,1);
	bouncer2 = App->physics->CreateCircle(767, 362, 25, b2_staticBody, 1);
	bouncer3 = App->physics->CreateCircle(867, 320, 25, b2_staticBody, 1);

	//Set up barrels
	int Barrels_1[14] = {
		276,647,
		326,641, 
		381,634,
		391,681,
		425,705,
		420,764,
		275,696

	};

	barrels_1 = App->physics->CreateChain(0, 0, Barrels_1, 14, b2_staticBody, 2, false, 2);

	int Barrels_2[10] = {
		756, 698,
		874, 634,
		901, 662,
		898,699,
		758,753
	};

	barrels_2 = App->physics->CreateChain(0, 0, Barrels_2, 10, b2_staticBody, 2, false, 2);

	//Set up cowboys
	for (int i = 0; i <= 4; i++) // First row
	{
		cowboys[i] = App->physics->CreateRectangle(502 + i*35, 440-i*20, 30, 80, b2_staticBody, 3);
		cowboys[i]->alive = true;
	}

	for (int i = 5; i < 11; i++) //Second row
	{
		cowboys[i] = App->physics->CreateRectangle(440 + (i-5) * 35, 396 - (i-5) * 20, 30, 80, b2_staticBody, 3);
		cowboys[i]->alive = true;
	}
	
	//Set up joints
	b2RevoluteJointDef first_joint;
	b2RevoluteJointDef second_joint;
	b2RevoluteJointDef third_joint;
	
	first_joint.bodyA = Lflipper->body; // Flipper
	first_joint.bodyB = pinball->body; // Box
	first_joint.collideConnected = false;
	first_joint.localAnchorA.Set(PIXEL_TO_METERS(-50), PIXEL_TO_METERS(0));
	first_joint.localAnchorB.Set(PIXEL_TO_METERS(170 * 2.5), PIXEL_TO_METERS((352* 2.5)));
	first_joint.enableLimit = true;
	first_joint.lowerAngle = -30 * DEGTORAD;
	first_joint.upperAngle = 30 * DEGTORAD;

	second_joint.bodyA = Rflipper->body; // Flipper
	second_joint.bodyB = pinball->body; // Box
	second_joint.collideConnected = false;
	second_joint.localAnchorA.Set(PIXEL_TO_METERS(50), PIXEL_TO_METERS(0));
	second_joint.localAnchorB.Set(PIXEL_TO_METERS(300 * 2.5), PIXEL_TO_METERS((352 * 2.5)));
	second_joint.enableLimit = true;
	second_joint.lowerAngle = -30 * DEGTORAD;
	second_joint.upperAngle = 30 * DEGTORAD;

	third_joint.bodyA = Sflipper->body; // Flipper
	third_joint.bodyB = pinball->body; // Box
	third_joint.collideConnected = false;
	third_joint.localAnchorA.Set(PIXEL_TO_METERS(50), PIXEL_TO_METERS(0));
	third_joint.localAnchorB.Set(PIXEL_TO_METERS(1000), PIXEL_TO_METERS(529));
	third_joint.enableLimit = true;
	third_joint.lowerAngle = -30 * DEGTORAD;
	third_joint.upperAngle = 30 * DEGTORAD;


	App->physics->world->CreateJoint(&first_joint);
	App->physics->world->CreateJoint(&second_joint);
	App->physics->world->CreateJoint(&third_joint);
	//Set up sensors 561,922
	ball_throw = App->physics->CreateRectangleSensor(1162, 738, 90, 35, 0);
	ball_throw->listener = this;
	ball_throw->alive = true;

	wall_sensor = App->physics->CreateRectangleSensor(400 * 2.5, (30 * 2.5), 10, 300, 1);
	wall_sensor->listener = this;
	wall_sensor->alive = true;

	bonus_sensorL = App->physics->CreateRectangleSensor(815, 204, 30, 25, 2);
	bonus_sensorL->listener = this;
	bonus_sensorL->alive = true;

	bonus_sensorC = App->physics->CreateRectangleSensor(881, 224, 30, 25, 2);
	bonus_sensorC->listener = this;
	bonus_sensorC->alive = true;

	bonus_sensorR = App->physics->CreateRectangleSensor(949, 250, 30, 25, 2);
	bonus_sensorR->listener = this;
	bonus_sensorR->alive = true;

	end_game = App->physics->CreateRectangleSensor(561, 1000, 1000, 25, 3);
	end_game->listener = this;
	end_game->alive = true;

	//Set up first ball
	ball = App->physics->CreateCircle(1164, 633, 18, b2_dynamicBody,0, true);
	ball->listener = this;

	//Set up score variables
	score_mult = 1;
	total_score = 0;
	actual_score = 0;
	lives = 5;

	//Set up bools
    wall_collision = true;
	cowboys_killed = false;

	Reset_Big_Game();
	sprintf_s(score_text, 20, "SCORE!");
	sprintf_s(balls_text, 5, "B");
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background);
	App->textures->Unload(sprites);
	App->textures->Unload(ball_sprite);
	App->textures->Unload(flipper_sprite);
	App->fonts->UnLoad(font_score);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Ball should allways have 0 inclination
	ball->body->SetTransform(b2Vec2(ball->body->GetPosition().x, ball->body->GetPosition().y), 0);

	//Wall collision change management
	if (wall_collision)
	{
		wall1->body->SetActive(true);
		wall2->body->SetActive(false);
	}
	else
	{
		wall1->body->SetActive(false);
		wall2->body->SetActive(true);
	}
    //Cowboys management
	for (int i = 0; i < 11; i++) 
	{
		if(!cowboys[i]->alive)
		cowboys[i]->body->SetActive(false);
	}
	//Score bust management
	if (!bonus_sensorL->alive && !bonus_sensorC->alive && !bonus_sensorR->alive)
	{
		bonus_sensorL->alive = true;
		bonus_sensorC->alive = true;
		bonus_sensorR->alive = true;
		if (score_mult == 1)
			score_mult++;
		else if (score_mult != 10)
			score_mult += 2;
		App->audio->PlayFx(multiply);
		score_timer.Start(2);
	}
	//Music management
	if (music_stop.IsOver() && Mix_PausedMusic())
	{
		Mix_ResumeMusic();
	}


	//Ball restart position
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{ 
		Reset_Big_Game();
		wall_collision = true;
	}



	//Pallets controller
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		Lflipper->body->ApplyAngularImpulse(-190, true);
	else
		Lflipper->body->ApplyAngularImpulse(1, true);

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
	{ 
		Rflipper->body->ApplyAngularImpulse(190, true);
	    Sflipper->body->ApplyAngularImpulse(190, true);
	}
	else
	{ 
		Rflipper->body->ApplyAngularImpulse(-1, true);
		Sflipper->body->ApplyAngularImpulse(-1, true);
	}

	//Ball thrower
	if (allow_throw && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ball->body->ApplyForceToCenter(b2Vec2(0, -10000), true);
	}
	allow_throw = false;
	// Prepare for raycast ------------------------------------------------------


	//Blit everything
	App->renderer->Blit(background, 0, 0, 1.66);

	if (cowboys[10]->alive)
		App->renderer->Blit(sprites, 589, 271, 1.66, &N);
	if (cowboys[9]->alive)
		App->renderer->Blit(sprites, 557, 288, 1.66, &O2);
	if (cowboys[8]->alive)
		App->renderer->Blit(sprites, 525, 305, 1.66, &O);
	if (cowboys[7]->alive)
		App->renderer->Blit(sprites, 489, 323, 1.66, &L);
	if (cowboys[6]->alive)
		App->renderer->Blit(sprites, 453, 341, 1.66, &A);
	if (cowboys[5]->alive)
		App->renderer->Blit(sprites, 416, 359, 1.66, &S2);
	if (cowboys[4]->alive)
		App->renderer->Blit(sprites, 627, 316, 1.66, &I);
	if (cowboys[3]->alive)
		App->renderer->Blit(sprites, 592, 334, 1.66, &S);
	if (cowboys[2]->alive)
		App->renderer->Blit(sprites, 559, 352, 1.66, &P2);
	if (cowboys[1]->alive)
		App->renderer->Blit(sprites, 522, 372, 1.66, &E);
	if (cowboys[0]->alive)
		App->renderer->Blit(sprites, 486, 392, 1.66, &P);

	if (!cowboys[0]->alive && !cowboys[1]->alive && !cowboys[2]->alive && !cowboys[3]->alive &&
		!cowboys[4]->alive && !cowboys[5]->alive && !cowboys[6]->alive && !cowboys[7]->alive &&
		!cowboys[8]->alive && !cowboys[9]->alive && !cowboys[10]->alive) 
	{ 
		App->renderer->Blit(sprites, 416, 191, 1.66, &saloon);
		if (!cowboys_killed)
		{
			App->audio->PlayFx(kill_cowboys);
			cowboys_killed = true;
			actual_score += 100;
		}
	    
	}

	App->fonts->BlitText(335 - strlen(score_text) * App->fonts->char_width * 3, 864, 0, score_text);
	App->fonts->BlitText(365, 864, 0, balls_text);

	int x, y;
	
	ball->GetPosition(x, y, true);
	App->renderer->Blit(ball_sprite,x,y, 0.7);
	Rflipper->GetPosition(x, y, true);
	App->renderer->Blit(flipper_sprite, x + 5, y - 40, 0.55, (SDL_Rect*)0, 1, Rflipper->GetRotation() + 30);
	Sflipper->GetPosition(x, y, true);
	App->renderer->Blit(flipper_sprite, x + 5, y - 40, 0.55, (SDL_Rect*)0, 1, Sflipper->GetRotation() + 30);
	Lflipper->GetPosition(x, y, true);
	App->renderer->Blit(flipper_sprite, x - 5, y - 40, 0.55, (SDL_Rect*)0, 1, Lflipper->GetRotation() - 30, true);

	return UPDATE_CONTINUE;
}
void ModuleSceneIntro::Reset_Small_Game()
{
	ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(1250), PIXEL_TO_METERS(580)), 0);
	ball->body->SetLinearVelocity(b2Vec2(0, 0));
	total_score = total_score + actual_score * score_mult;
	actual_score = 0;
	score_mult = 1;
	lives--;
}

void ModuleSceneIntro::Reset_Big_Game()
{
	ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(1250), PIXEL_TO_METERS(580)), 0);
	ball->body->SetLinearVelocity(b2Vec2(0, 0));
	total_score = 0;

	if (lives == 0) //Reset cowboys
	{
		for (int i = 0; i < 11; i++)
		{
			cowboys[i]->body->SetActive(true);
			cowboys[i]->alive = true;
		}
		cowboys_killed = false;
	}
	lives = 5;
}

//If there is a sensor, body a is the sensor

//If there is no sensor, body b is the ball
void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//App->audio->PlayFx(bonus_fx);
	if (bodyA->body->GetFixtureList()->IsSensor()) //Sensors management
	{
		if (bodyA->type == 0)  //Ball throwing position
		{
			allow_throw = true;
			wall_collision = false;
		}
		if (bodyA->type == 1) //Temporal barrier
		{
			wall_collision = true;
		}
		if (bodyA->type == 2) //Score bust
		{
			if(score_timer.IsOver())
			bodyA->alive = false;
		}
		if (bodyA->type == 3) //Ball death
		{
			if (lives != 0)
			{
				Reset_Small_Game();
				App->audio->PlayFx(lose_sound);	
				Mix_PauseMusic();
				music_stop.Start(3);
				
			}
			else
			{
				Reset_Big_Game(); //Fx sound should be different from small_reset sound
			}
		}
	}
	else                                          //Body management
	{
		if (bodyA->type == 1) //bouncer
		{
			iPoint ball_position;
			iPoint bouncer_position;
			bodyA->GetPosition(bouncer_position.x, bouncer_position.y);
 			bodyB->GetPosition(ball_position.x, ball_position.y);


			b2Vec2 force((ball_position.x - bouncer_position.x), (ball_position.y - bouncer_position.y));
			float32 Length = sqrt(pow(force.x, 2) + pow(force.y, 2));
			force.x = (force.x / Length) * 200;
			force.y = (force.y / Length) * 200;

            App->audio->PlayFx(bonus_fx);
			bodyB->body->ApplyLinearImpulse(force,b2Vec2(0,0), true);
			actual_score += 10;
		}
		if (bodyA->type == 2)
		{
			App->audio->PlayFx(boing);
			actual_score+= 5;
		}
		if (bodyA->type == 3)
		{
			App->audio->PlayFx(paw);
			bodyA->alive = false;
			actual_score += 15;
		}
	}

}
