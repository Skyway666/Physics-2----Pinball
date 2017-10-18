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

	// Creation of the pinball board
	int Pinball_box[20] = {
		170, 352, // Palanca 1
		51, 273,
		0, 193,
		371, 0,
		466, 29,
		416, 96,
		384, 286,
		300, 352, //Palanca 2
		300, 390,
		170, 390

	};

	for (int i = 0; i != 20; i++)
	{
		Pinball_box[i] = Pinball_box[i] * 2.5;
	}

	chains.add(App->physics->CreateChain(0, 0, Pinball_box, 20, b2_staticBody));

	boxes.add(App->physics->CreateRectangle(170 * 2.5, 352 * 2.5, 150, 35));

	boxes.add(App->physics->CreateRectangle(300 * 2.5, 352 * 2.5, 150, 35));

	b2RevoluteJointDef first_joint;
	b2RevoluteJointDef second_joint;

	chains.at(0, bodyA);
	boxes.at(0, bodyB);
	boxes.at(1, bodyC);
	
	first_joint.bodyA = bodyB->body; // Pala
	first_joint.bodyB = bodyA->body; // Tablero
	first_joint.collideConnected = false;
	first_joint.localAnchorA.Set(PIXEL_TO_METERS(-50), PIXEL_TO_METERS(0));
	first_joint.localAnchorB.Set(PIXEL_TO_METERS(170 * 2.5), PIXEL_TO_METERS(352* 2.5));
	first_joint.enableLimit = true;
	first_joint.lowerAngle = -30 * DEGTORAD;
	first_joint.upperAngle = 30 * DEGTORAD;

	second_joint.bodyA = bodyC->body; // Pala
	second_joint.bodyB = bodyA->body; // Tablero
	second_joint.collideConnected = false;
	second_joint.localAnchorA.Set(PIXEL_TO_METERS(50), PIXEL_TO_METERS(0));
	second_joint.localAnchorB.Set(PIXEL_TO_METERS(300 * 2.5), PIXEL_TO_METERS(352 * 2.5));
	second_joint.enableLimit = true;
	second_joint.lowerAngle = -30 * DEGTORAD;
	second_joint.upperAngle = 30 * DEGTORAD;

	App->physics->world->CreateJoint(&first_joint);
	App->physics->world->CreateJoint(&second_joint);

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
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 150, 25));
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		bodyB->body->ApplyForce(b2Vec2(0, -50), b2Vec2(50, 0), true);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		bodyC->body->ApplyForce(b2Vec2(0, 50), b2Vec2(50, 0), true);

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);


	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
}
