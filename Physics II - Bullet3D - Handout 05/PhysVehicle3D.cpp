#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render(bool p, int turbo, bool t)
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	//Main body
	CreateRenderPart(Color(1, 0.1, 1), 0, 0, -6, 0, 0, 0);
	CreateRenderPart(Color(1, 0.1, 1), 0, -1, 0, 0, -0.5, 0);

	//Turbo
	CreateRenderPart(Color(1, 0.7, 0), -1, -1.99, -15 + 7 * (turbo / 100.0f), 0, 1, -3.5 + (3.5 * (turbo / 100.0f)));
	
	if (t) {
	//	CreateRenderPart();
	}

	//Windows
	for (int i = 0; i < 6; i++) {
		CreateRenderPart(Color(Black), 0.1, -1.2, -14.2, 0, 0.4, -3.5 + 1.40 * i);
	}

	//person
	if (p){
		btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
		btVector3 offset(info.chassis_offset.x, info.chassis_offset.y + 1.2, info.chassis_offset.z - 2);
		offset = offset.rotate(q.getAxis(), q.getAngle());

		Sphere chassis(0.8);
		vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);

		chassis.transform.M[12] += offset.getX();
		chassis.transform.M[13] += offset.getY();
		chassis.transform.M[14] += offset.getZ();

		chassis.color.Set(1, 0.7, 0.5);

		chassis.Render();
	}
}

//color, size (respect vehicle's colisionbox), offsets
void PhysVehicle3D::CreateRenderPart(Color color, 
									float chassisX, float chassisY, float chassisZ,
									float offsetX, float offsetY, float offsetZ) {
	
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x + offsetX, info.chassis_offset.y + offsetY, info.chassis_offset.z + offsetZ);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	Cube chassis(info.chassis_size.x + chassisX, info.chassis_size.y + chassisY, info.chassis_size.z + chassisZ);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.color = color;

	chassis.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}