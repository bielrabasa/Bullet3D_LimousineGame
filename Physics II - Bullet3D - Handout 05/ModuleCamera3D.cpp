#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		followVehicle = !followVehicle;

	vec3 newPos(0,0,0);
	float speed = 50.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 100.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------
	if (!followVehicle) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
	}
	else{	//FOLLOW VEHICLE

		//Rotate camera with mouse arround the vehicle
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			float Sensitivity = 0.0015f;
			camRotation -= App->input->GetMouseXMotion() * Sensitivity;
			
			//Cam rotation limiter (almost 90? each side)
			if (camRotation > 0.55f)
				camRotation = 0.55f;

			if (camRotation < -0.55f)
				camRotation = -0.55f;
		}

		vec3 vehiclePos = vec3(App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().x(),
							   App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().y() + 10.0f,
							   App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().z());
		
		//vec3 cameraPos = vec3(vehiclePos.x + 0.0f, vehiclePos.y + 30.0f, vehiclePos.z - 50.0f);
		
		//Move the camera to the back of the vehicle and rotate with it
		Position.x = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 30 * App->player->vehicle->vehicle->getForwardVector().getX();
		Position.y = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 15 * App->player->vehicle->vehicle->getUpAxis();
		Position.z = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 30 * App->player->vehicle->vehicle->getForwardVector().getZ();

		//Regulate cam rotation
		Position -= X * camRotation * -50.0f;
		Reference -= X * camRotation * -50.0f;

		//Set the cam to look at the vehicle position
		LookAt(vehiclePos);

		if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_REPEAT) {
			Position.y += 20;
			LookAt(App->scene_intro->fletxa_pos);
		}

		if (App->player->win) {
			MoveAt(vec3(480, 30, 500));
			LookAt(vec3(500, 0, 500));
		}

		if (App->player->lose) {
			MoveAt(vec3(-480, 30, -500));
			LookAt(vec3(-500, 0, -500));
		}
	}
	

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ModuleCamera3D::MoveAt(const vec3& Movement)
{
	Position = Movement;
	Reference = Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}