#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "time.h"

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

	floor = new Cube(500, 1, 500);
	floor->SetPos(0, -0.5, 0);
	floor->color = Color(0.1f, 0.1f, 0.1f);

	sensor = App->physics->AddBody(Cube(10, 10, 10), 0.0f);
	sensor->SetAsSensor(true);

	fletxa = new Cylinder(1, 10);
	fletxa->SetRotation(90, vec3(0, 0, 1));
	fletxa->color = Color(0.1, 5.0, 0.1);

	fletxa_point_top = new Cylinder(2, 2);
	fletxa_point_top->SetRotation(90, vec3(0, 0, 1));
	fletxa_point_top->color = Color(0.1, 5.0, 0.1);

	fletxa_point = new Sphere(2);
	fletxa_point->color = Color(0.1, 5.0, 0.1);

	MoveSensor(0, -50);

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


	//Renders
	floor->Render();
	for (p2List_item<Cube*>* current_building = buildings.getFirst(); current_building != nullptr; current_building = current_building->next) {
		current_building->data->Render();
	}
	fletxa->Render();
	fletxa_point->Render();
	fletxa_point_top->Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::MoveSensor(float x, float z) {
	int y = 15;	//arrow height
	fletxa->SetPos(x, y + 5, z);
	fletxa_point->SetPos(x, y, z);
	fletxa_point_top->SetPos(x, y + 1, z);
	sensor->SetPos(x, 2, z);
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2 == sensor) {
		srand(time(NULL));
		MoveSensor(-50 + rand() % 100, -50);
	}
}

