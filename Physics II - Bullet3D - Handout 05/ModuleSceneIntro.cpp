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
	sensor->SetPos(0, 1, -50);

	fletxa = new Cylinder(1, 10);
	fletxa->SetRotation(90, vec3(0, 0, 1));
	fletxa->color = Color(0.1, 5.0, 0.1);

	fletxa_point_top = new Cylinder(2, 2);
	fletxa_point_top->SetRotation(90, vec3(0, 0, 1));
	fletxa_point_top->color = Color(0.1, 5.0, 0.1);

	fletxa_point = new Sphere(2);
	fletxa_point->color = Color(0.1, 5.0, 0.1);

	MoveArrow(0, -50);

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

	for (p2List_item<Cube*>* current_building = buildings.getFirst(); current_building != nullptr; current_building = current_building->next) {
		current_building->data->Render();
	}
	
	fletxa->Render();
	fletxa_point->Render();
	fletxa_point_top->Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::MoveArrow(float x, float z) {
	int y = 10;
	fletxa->SetPos(x, y + 5, z);
	fletxa_point->SetPos(x, y, z);
	fletxa_point_top->SetPos(x, y + 1, z);
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2 == sensor) {
		
	}
}

