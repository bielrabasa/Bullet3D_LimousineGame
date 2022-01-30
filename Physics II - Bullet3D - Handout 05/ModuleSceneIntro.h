#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void MoveSensor(float x, float z);

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;
	
	void NormalBuildingCreation();
	void WinLose(bool win, float wx, float wz, float x, float z, float r);

public:
	Cube* floor;
	Cube* park;

	struct Person {
		Sphere* head = nullptr;
		Cylinder* body = nullptr;
		Cylinder* armL = nullptr;
		Cylinder* armR = nullptr;
		Cylinder* legL = nullptr;
		Cylinder* legR = nullptr;
		Person();
		void Move(float x, float y, float z);
		void Render();
	}person;

	PhysBody3D* sensor;
	Cylinder* fletxa;
	Cylinder* fletxa_point_top;
	Sphere* fletxa_point;
	float fletxa_fluctuation = 0.0f;
	vec3 fletxa_pos = vec3(0.0f, 0.0f, 0.0f);
	
	vec2 missions[20];
	vec2 objectives[6];
	bool mission = true;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;
	p2List<Cube*> buildings;

};
