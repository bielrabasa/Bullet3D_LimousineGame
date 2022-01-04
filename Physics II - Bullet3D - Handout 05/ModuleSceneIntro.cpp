#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 100.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	sensor = App->physics->AddBody(Cube(5, 5, 5), 0.0f);
	sensor->SetAsSensor(true);
	sensor->SetPos(0, 0, -20);
	sensor->collision_listeners.add(App->scene_intro);

	sensorvehicle = App->physics->AddBody(Sphere(5), 0.0f);
	sensorvehicle->SetAsSensor(true);	
	sensorvehicle->SetPos(0, 0, 0);
	sensorvehicle->collision_listeners.add(App->scene_intro);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	sensorvehicle->SetPos(App->player->position.x, App->player->position.y, App->player->position.z);

	for (p2List_item<Cube*>* current_building = buildings.getFirst(); current_building != nullptr; current_building = current_building->next) {
		current_building->data->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == sensor && body2 == sensorvehicle) {
		App->physics->CreateBuilding(100, 100, -30, -30);
	}
}

