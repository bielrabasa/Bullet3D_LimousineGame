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

	//WIN
	WinLose(true, 10, 1, 0, -20, 10);	//W
	WinLose(true, 10, 1, 0, -10, -10);
	WinLose(true, 6, 1, -3, -17, -50);
	WinLose(true, 6, 1, -3, -13, 50);

	WinLose(true, 10, 1, 0, 0, 0);		//I
	
	WinLose(true, 10, 1, 0, 10, 0);		//N
	WinLose(true, 10, 1, 0, 18, 0);	
	WinLose(true, 12, 1, 0, 14, 40);

	//LOSE
	WinLose(false, 10, 1, 0, 19, 0);	//L
	WinLose(false, 5, 1, 5, 17, 90);
	
	WinLose(false, 10, 1, 0, 9, 0);	//O
	WinLose(false, 10, 1, 0, 4, 0);
	WinLose(false, 6, 1, 5, 6.5, 90);
	WinLose(false, 6, 1, -4.5, 6.5, 90);

	WinLose(false, 5, 1, -2.5, -4, 0);	//S
	WinLose(false, 5, 1, 2.5, -9, 0);
	WinLose(false, 6, 1, 5, -6.5, 90);
	WinLose(false, 6, 1, -4.5, -6.5, 90);
	WinLose(false, 6, 1, 0, -6.5, 90);

	WinLose(false, 10, 1, 0, -14, 0);	//E
	WinLose(false, 6, 1, 5, -16.5, 90);
	WinLose(false, 6, 1, -4.5, -16.5, 90);
	WinLose(false, 6, 1, 0, -16.5, 90);
	

	floor = new Cube(500, 1, 500);
	floor->SetPos(0, -0.5, 0);
	floor->color = Color(0.1f, 0.1f, 0.1f);

	park = new Cube(80, 0.1, 80);
	park->SetPos(0, 0.05, 0);
	park->color.Set(0.2, 1, 0.0);

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

	MoveSensor(0, 0);

	//Missions
	missions[0] = vec2(68, -237);
	missions[1] = vec2(-237, -141);
	missions[2] = vec2(-141, -131);
	missions[3] = vec2(-160, -46);
	missions[4] = vec2(-61, -73);
	missions[5] = vec2(1, -135);
	missions[6] = vec2(114, -44);
	missions[7] = vec2(235, -156);
	missions[8] = vec2(-92, 26);
	missions[9] = vec2(92, 26);
	missions[10] = vec2(183, 49);
	missions[11] = vec2(247, -1);
	missions[12] = vec2(163, 139);
	missions[13] = vec2(30, 101);
	missions[14] = vec2(8, 203);
	missions[15] = vec2(-85, 95);
	missions[16] = vec2(-58, 139);
	missions[17] = vec2(-110, 236);
	missions[18] = vec2(-185, 200);
	missions[19] = vec2(-241, 83);

	//Objectives
	objectives[0] = vec2(-228, -174);
	objectives[1] = vec2(12, -206);
	objectives[2] = vec2(197, -203);
	objectives[3] = vec2(-200, -2);
	objectives[4] = vec2(225, -65);
	objectives[5] = vec2(186, 185);

	person.Move(0, 5, 0);

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
	park->Render();
	for (p2List_item<Cube*>* current_building = buildings.getFirst(); current_building != nullptr; current_building = current_building->next) {
		current_building->data->Render();
	}
	fletxa->Render();
	fletxa_point->Render();
	fletxa_point_top->Render();
	person.Render();

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
			person.Move(nextMission.x, 5, nextMission.y);
			App->player->score++;
			if (App->player->score >= 3)
				App->player->win = true;
		}
		else {
			//bringing people to place
			vec2 nextObjective = objectives[rand() % 6];
			MoveSensor(nextObjective.x, nextObjective.y);
			person.Move(0, -10, 0);
		}
	}
}

ModuleSceneIntro::Person::Person() {
	head = new Sphere(1.0f);
	head->color.Set(1, 0.7, 0.5);

	body = new Cylinder(0.5f, 3.0f);
	body->color.Set(0, 1, 1);

	armL = new Cylinder(0.3f, 1.5f);
	armL->color.Set(1.0f, 0.7, 0.5);
	armR = new Cylinder(0.3f, 1.5f);
	armR->color.Set(1.0f, 0.7, 0.5);

	legL = new Cylinder(0.4f, 2.0f);
	legL->color.Set(0.0, 0.2, 1);
	legR = new Cylinder(0.4f, 2.0f);
	legR->color.Set(0.0, 0.2, 1);

	body->SetRotation(90, vec3(0, 0, 1));
	armL->SetRotation(45, vec3(0, 0, 1));
	armR->SetRotation(-45, vec3(0, 0, 1));
	legL->SetRotation(80, vec3(0, 0, 1));
	legR->SetRotation(-80, vec3(0, 0, 1));

}

void ModuleSceneIntro::Person::Move(float x, float y, float z) {
	head->SetPos(x, y, z);
	body->SetPos(x, y - 2, z);
	armL->SetPos(x - 0.8, y - 2, z);
	armR->SetPos(x + 0.8, y - 2, z);
	legL->SetPos(x - 0.5, y - 4, z);
	legR->SetPos(x + 0.5, y - 4, z);
}

void ModuleSceneIntro::Person::Render() {
	head->Render();
	body->Render();
	armL->Render();
	armR->Render();
	legL->Render();
	legR->Render();
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

void ModuleSceneIntro::WinLose(bool win, float wx, float wz, float x, float z, float r) {

	//Create main building
	Cube* c = new Cube(wx, 1, wz);
	c->SetRotation(r, vec3(0, 1, 0));

	float d = -500;
	if (win)
		d = 500;
		
	c->SetPos(x + d, 0.5, z + d);
	
	if(win)
		c->color = Color(Green);
	else
		c->color = Color(Red);
	
	buildings.add(c);
}