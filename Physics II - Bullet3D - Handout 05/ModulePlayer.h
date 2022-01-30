#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 5000.0f
#define TURN_DEGREES 30.0f * DEGTORAD
#define BRAKE_POWER 500.0f		//normal = 200

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

public:

	PhysVehicle3D* vehicle;
	vec3 position;
	float turn;
	float acceleration;
	float brake;
	int turbo = 100;
	bool t = false;
	int score = 0;
	bool win = false;
	bool lose = false;
};