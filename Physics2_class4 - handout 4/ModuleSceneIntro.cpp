#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
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

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	// Creation of the pinball board body
	int Pinball_box_1[10] = {
		170, 390,
		170, 342, // Flipper 1
		51, 273,
		0, 193,
		371, 0,
	

	};	
	
	int Pinball_box_2[8] = {
		416, 30,
		384, 286,
		300, 342, // Flipper 2
		300, 390,
	};

	int Pinball_ball_throw[14] = {
		416, 30,
		455, 150,
		416, 286,
		435, 286,
		474, 150,
		435,0,
		371,0
	};

	int Wall[8] = {
		416, 0,
		416, 100,
		430, 100,
		430, 0,
	};


	for (int i = 0; i != 10; i++)
	{
		Pinball_box_1[i] = Pinball_box_1[i] * 2.5;
	}
	
	for (int i = 0; i != 8; i++)
	{
		Pinball_box_2[i] = Pinball_box_2[i] * 2.5;
	}


	for (int i = 0; i != 14; i++)
	{
		Pinball_ball_throw[i] = Pinball_ball_throw[i] * 2.5;
	}

	for (int i = 0; i != 8; i++)
	{
		Wall[i] = Wall[i] * 2.5;
	}

	Lpinball = App->physics->CreateChain(0, 0, Pinball_box_1, 10, b2_staticBody,true);

	Rpinball = App->physics->CreateChain(0, 0, Pinball_box_2, 8, b2_staticBody, true);

	Bpinball = App->physics->CreateChain(0, 0, Pinball_ball_throw, 14, b2_staticBody, true);

	Lflipper = App->physics->CreateRectangle(170 * 2.5, (352 * 2.5), 180, 35);

	Rflipper = App->physics->CreateRectangle(300 * 2.5, (352 * 2.5), 180, 35);


	wall = App->physics->CreateChain(0, 0, Wall, 8, b2_staticBody, true);

	//Set filter data
	b2Filter filter = wall->body->GetFixtureList()->GetFilterData();
	filter.groupIndex = -1;
	wall->body->GetFixtureList()->SetFilterData(filter);


	//Set up joints
	b2RevoluteJointDef first_joint;
	b2RevoluteJointDef second_joint;
	

	first_joint.bodyA = Lflipper->body; // Pala
	first_joint.bodyB = Lpinball->body; // Tablero
	first_joint.collideConnected = false;
	first_joint.localAnchorA.Set(PIXEL_TO_METERS(-50), PIXEL_TO_METERS(0));
	first_joint.localAnchorB.Set(PIXEL_TO_METERS(170 * 2.5), PIXEL_TO_METERS((352* 2.5)));
	first_joint.enableLimit = true;
	first_joint.lowerAngle = -30 * DEGTORAD;
	first_joint.upperAngle = 30 * DEGTORAD;

	second_joint.bodyA = Rflipper->body; // Pala
	second_joint.bodyB = Rpinball->body; // Tablero
	second_joint.collideConnected = false;
	second_joint.localAnchorA.Set(PIXEL_TO_METERS(50), PIXEL_TO_METERS(0));
	second_joint.localAnchorB.Set(PIXEL_TO_METERS(300 * 2.5), PIXEL_TO_METERS((352 * 2.5)));
	second_joint.enableLimit = true;
	second_joint.lowerAngle = -30 * DEGTORAD;
	second_joint.upperAngle = 30 * DEGTORAD;

	App->physics->world->CreateJoint(&first_joint);
	App->physics->world->CreateJoint(&second_joint);

	//Set up sensors 416, 286,
	ball_throw = App->physics->CreateRectangleSensor(428 * 2.5, (286 * 2.5), 90, 35);
	ball_throw->listener = this;


	wall_sensor = App->physics->CreateRectangleSensor(400 * 2.5, (30 * 2.5), 10, 300, 1);
	wall_sensor->listener = this;

	//Set up first ball

	ball = App->physics->CreateCircle(1078, 653, 18, true);
	ball->listener = this;
	//Set filter data
	filter = ball->body->GetFixtureList()->GetFilterData();
	filter.groupIndex = -1;
	ball->body->GetFixtureList()->SetFilterData(filter);



	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

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
		b2Filter filter = wall->body->GetFixtureList()->GetFilterData();
		filter.groupIndex = 0;
		wall->body->GetFixtureList()->SetFilterData(filter);
	}
	else
	{
		b2Filter filter = wall->body->GetFixtureList()->GetFilterData();
		filter.groupIndex = -1;
		wall->body->GetFixtureList()->SetFilterData(filter);
	}


	//Ball restart position
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{ 
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(1078), PIXEL_TO_METERS(653)), 0);
		ball->body->SetLinearVelocity(b2Vec2(0, 0));
	}



	//Pallets controller
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		Lflipper->body->ApplyAngularImpulse(-80, true);
	else
		Lflipper->body->ApplyAngularImpulse(1, true);

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		Rflipper->body->ApplyAngularImpulse(80, true);
	else
		Rflipper->body->ApplyAngularImpulse(-1, true);


	//Ball thrower
	if (allow_throw && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ball->body->ApplyForceToCenter(b2Vec2(0, 10000), true);
	}
	allow_throw = false;
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	
	return UPDATE_CONTINUE;
}

//If there is just one sensor, body a is the sensor
void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	//App->audio->PlayFx(bonus_fx);

	if (bodyA->body->GetFixtureList()->IsSensor())
	{
		if (bodyA->type == 0) 
		{
			allow_throw = true;
			wall_collision = false;
		}
		else if (bodyA->type == 1)
		{
			wall_collision = true;
		}
	}
}
