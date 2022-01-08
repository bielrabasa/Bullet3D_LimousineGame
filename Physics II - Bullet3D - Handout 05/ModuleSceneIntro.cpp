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

	sensor = App->physics->AddBody(Cube(20, 10, 20), 0.0f);
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
	//Arrow vectical fluctuation
	fletxa_fluctuation += 0.03;
	if (fletxa_fluctuation >= 360)
		fletxa_fluctuation = 0;
	float fluct_dist = sin(fletxa_fluctuation) * 10;

	fletxa->SetPos(fletxa_pos.x, fletxa_pos.y + 5 + fluct_dist, fletxa_pos.z);
	fletxa_point->SetPos(fletxa_pos.x, fletxa_pos.y + fluct_dist, fletxa_pos.z);
	fletxa_point_top->SetPos(fletxa_pos.x, fletxa_pos.y + 1 + fluct_dist, fletxa_pos.z);
	
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
	int y = 30;	//arrow height

	fletxa_fluctuation = 0.0f;
	fletxa_pos.Set(x, y, z);

	fletxa->SetPos(x, y + 5, z);
	fletxa_point->SetPos(x, y, z);
	fletxa_point_top->SetPos(x, y + 1, z);
	sensor->SetPos(x, 5, z);
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2 == sensor) {
		srand(time(NULL));
		
		//change between mission and objective
		mission = !mission;

		//Change sensor randomly between possible positions
		if (mission) {
			//searching for people
			vec2 nextMission = missions[rand() % 20];
			MoveSensor(nextMission.x, nextMission.y);
		}
		else {
			//bringing people to place
			vec2 nextObjective = objectives[rand() % 6];
			MoveSensor(nextObjective.x, nextObjective.y);
		}
	}
}

void ModuleSceneIntro::NormalBuildingCreation() {
	App->physics->CreateBuilding(30, 30, -250, -138);
	App->physics->CreateBuilding(30, 30, -207, -153);
	App->physics->CreateBuilding(30, 30, -214, -117);
	App->physics->CreateBuilding(30, 30, -181, -92);
	App->physics->CreateBuilding(30, 30, -144, -164);
	App->physics->CreateBuilding(30, 30, -151, -128);
	App->physics->CreateBuilding(30, 30, -144, -92);
	App->physics->CreateBuilding(30, 30, -108, -143);
	App->physics->CreateBuilding(30, 30, -108, -105);
	App->physics->CreateBuilding(30, 30, -71, -163);
	App->physics->CreateBuilding(30, 30, -71, -105);
	App->physics->CreateBuilding(30, 30, -34, -150);
	App->physics->CreateBuilding(30, 30, -34, -105);
	App->physics->CreateBuilding(30, 30, 4, -150);
	App->physics->CreateBuilding(30, 30, 4, -105);
	App->physics->CreateBuilding(30, 30, 58, -150);
	App->physics->CreateBuilding(30, 30, 90, -150);
	App->physics->CreateBuilding(30, 30, 176, -156);
	App->physics->CreateBuilding(30, 30, 220, -156);
	App->physics->CreateBuilding(30, 30, 220, -122);
	App->physics->CreateBuilding(58, 28, 60, -105);
	App->physics->CreateBuilding(30, 30, 60, -76);
	App->physics->CreateBuilding(30, 30, 97, -76);
	App->physics->CreateBuilding(30, 30, 60, -37);
	App->physics->CreateBuilding(30, 30, 97, -37);
	App->physics->CreateBuilding(30, 30, 142, -79);
	App->physics->CreateBuilding(30, 30, 142, -47);
	App->physics->CreateBuilding(30, 30, 220, 7);
	App->physics->CreateBuilding(30, 30, 176, -37);
	App->physics->CreateBuilding(30, 30, 220, -37);
	App->physics->CreateBuilding(30, 30, 60, 3);
	App->physics->CreateBuilding(30, 30, 97, 7);
	App->physics->CreateBuilding(30, 30, 142, -15);
	App->physics->CreateBuilding(30, 30, 142, 17);
	App->physics->CreateBuilding(30, 30, 176, 7);
	App->physics->CreateBuilding(30, 30, 176, -74);
	App->physics->CreateBuilding(49, 38, 57, 43);
	App->physics->CreateBuilding(50, 65, 143, 52);
	App->physics->CreateBuilding(56, 86, 74, 85);
	App->physics->CreateBuilding(59, 38, 76, 190);
	App->physics->CreateBuilding(45, 15, 205, 44);
	App->physics->CreateBuilding(25, 44, 225, 59);
	App->physics->CreateBuilding(24, 55, 213, 111);
	App->physics->CreateBuilding(70, 24, 143, 142);
	App->physics->CreateBuilding(59, 38, 3, 206);
	App->physics->CreateBuilding(86, 56, -26, 145);
	App->physics->CreateBuilding(30, 30, 3, 104);
	App->physics->CreateBuilding(30, 30, 3, 60);
	App->physics->CreateBuilding(30, 30, -34, 60);
	App->physics->CreateBuilding(30, 30, -34, 104);
	App->physics->CreateBuilding(59, 38, -82, 206);
	App->physics->CreateBuilding(30, 30, -134, 203);
	App->physics->CreateBuilding(30, 30, -166, 214);
	App->physics->CreateBuilding(30, 30, -198, 203);
	App->physics->CreateBuilding(30, 30, -230, 214);
	App->physics->CreateBuilding(30, 30, -167, 167);
	App->physics->CreateBuilding(30, 30, -135, 167);
	App->physics->CreateBuilding(30, 30, -103, 167);
	App->physics->CreateBuilding(30, 30, -71, 167);
	App->physics->CreateBuilding(30, 30, -186, 132);
	App->physics->CreateBuilding(30, 30, -154, 132);
	App->physics->CreateBuilding(30, 30, -122, 132);
	App->physics->CreateBuilding(30, 30, -90, 132);
	App->physics->CreateBuilding(30, 30, -167, 97);
	App->physics->CreateBuilding(30, 30, -135, 97);
	App->physics->CreateBuilding(30, 30, -103, 97);
	App->physics->CreateBuilding(30, 30, -71, 97);
	App->physics->CreateBuilding(30, 30, -186, 62);
	App->physics->CreateBuilding(30, 30, -154, 62);
	App->physics->CreateBuilding(30, 30, -122, 62);
	App->physics->CreateBuilding(30, 30, -90, 62);
	App->physics->CreateBuilding(30, 30, -226, 167);
	App->physics->CreateBuilding(30, 30, -237, 135);
	App->physics->CreateBuilding(30, 30, -226, 103);
	App->physics->CreateBuilding(30, 30, -237, 71);
	App->physics->CreateBuilding(30, 30, -90, 5);
	App->physics->CreateBuilding(30, 30, -90, -35);
	App->physics->CreateBuilding(30, 30, -124, -35);
	App->physics->CreateBuilding(30, 30, -124, 5);
	App->physics->CreateBuilding(30, 30, -158, 5);
	App->physics->CreateBuilding(30, 30, -158, -35);
	App->physics->CreateBuilding(40, 40, -201, -57);
	App->physics->CreateBuilding(40, 40, -201, 17);
	App->physics->CreateBuilding(10, 60, 53, -250);
	App->physics->CreateBuilding(197, 10, 53, -176);
}