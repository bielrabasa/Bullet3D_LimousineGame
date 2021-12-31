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
void PhysVehicle3D::Render()
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

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	CreateRenderPart(offset, Color(Red), 0, 0, -6, 0, 0, 0);
	CreateRenderPart(offset, Color(Red), 0, -1, 0, 0, -0.5, 0);

	/*for (int i = 0; i < 6; i++) {
		CreateRenderPart(offset, Color(Black), 0.1, -1.2, -14.2, 0, 0.4, -3.5 + 1.40 * i);
	}*/
	
}

//vehicle offset, color, size (respect vehicle's colisionbox), offsets
void PhysVehicle3D::CreateRenderPart(btVector3 offset, Color color, 
									float chassisX, float chassisY, float chassisZ,
									float offsetX, float offsetY, float offsetZ) {
	
	Cube chassis(info.chassis_size.x + chassisX, info.chassis_size.y + chassisY, info.chassis_size.z + chassisZ);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);

	chassis.transform.M[12] += offset.getX() + offsetX;
	chassis.transform.M[13] += offset.getY() + offsetY;
	chassis.transform.M[14] += offset.getZ() + offsetZ;

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