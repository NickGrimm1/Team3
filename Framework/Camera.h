/**
Version History:
0.0.5 05/02/2015
0.0.6 11/02/2015
	Added target entity & made variables protected to create inherited free, offset & chase cameras.
*/

/**
<summary>A somewhat basic 6DOF camera.

Author: Nick Grimm
Version: 0.0.6 11/02/2015.</summary>
*/
#pragma once
#include "Quaternion.h"
#include "T3Matrix4.h"
#include "T3Vector3.h"
#include "../Team3Project1/DrawableEntity3D.h"

class DrawableEntity3D;

class Camera	
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='targetEntity'>The target Entity. Default is NULL.</param>
	<param name='pitch'>The initial pitch. Default is 0.</param>
	<param name='yaw'>The initial yaw. Default is 0.</param>
	<param name='roll'>The initial roll. Default is 0.</param>
	<param name='position'>The initial position. Default is (0, 0, 0).</param>
	<param name='forward'>The initial forward vector. Default is (0, 0, -1).</param>
	<param name='up'>The initial up vector. Default is (0, 1, 0).</param>
	*/
	Camera(float pitch = 0, float yaw = 0, float roll = 0, T3Vector3 position = T3Vector3(), T3Vector3 target = T3Vector3(0, 0, -1), bool invertY = false)
		: position(position),
		originalTarget(target.Normal()),
		originalUp(T3Vector3::UnitOrthogonal(Quaternion::EulerAnglesToQuaternion(pitch, yaw, roll).ToMatrix() * T3Vector3::UnitX(), (target).Normal())),
		invertY(invertY),
		originalRight(Quaternion::EulerAnglesToQuaternion(pitch, yaw, roll).ToMatrix() * T3Vector3::UnitX()),
		rotation(Quaternion::EulerAnglesToQuaternion(pitch, yaw, roll).ToMatrix())
	{
		std::cout << "Camera: target: " << originalTarget << std::endl;
	}
	~Camera(void){}

	/**
	<summary>Gets the camera's position in world space.</summary>
	<param name='value'>The amount of pitch to add.</param>
	*/
	T3Vector3 GetPosition() const { return position;}
	/**
	<summary>Sets the camera's position in world space.</summary>
	<param name='value'>The position</param>
	*/
	void SetPosition(const T3Vector3& value) { position = value;}
	/**
	<summary>Adds movement to this camera's position, automatically calculates rotation.</summary>
	<param name='value'>The amount of movement to add in the original axes provided. (eg if (0,0,-1) was originally 'forward', moving (0,0,-1) will move the camera 1 unit along its current forward vector.</param>
	*/
	void AddMovement(const T3Vector3& value) { position += rotation * value; }
	/**
	<summary>Adds movement to this camera's position, using the raw co-ordinates given.</summary>
	<param name='value'>The amount of movement to add.</param>
	*/
	void AddRawMovement(const T3Vector3& value) { position += value; }
	/**
	<summary>Adds pitch to this camera's rotation.</summary>
	<param name='value'>The amount of pitch to add.</param>
	*/
	void AddPitch(const float value) 
	{
		rotatedRight = rotation * originalRight;
		rotatedRight.Normalise();
		if (!invertY)
			rotation = T3Matrix4::Rotation(value, rotatedRight) * rotation;
		else
			rotation = T3Matrix4::Rotation(-value, rotatedRight) * rotation;
	}
	/**
	<summary>Adds yaw to this camera's rotation.</summary>
	<param name='value'>The amount of yaw to add.</param>
	*/
	void AddYaw(const float value) 
	{ 
		rotatedUp = rotation * originalUp;
		rotatedUp.Normalise();
		rotation =  T3Matrix4::Rotation(value, rotatedUp) * rotation;
	}
	/**
	<summary>Adds roll to this camera's rotation.</summary>
	<param name='value'>The amount of roll to add.</param>
	*/
	void AddRoll(const float value) 
	{
		rotatedTarget = rotation * originalTarget;
		rotatedTarget.Normalise();
		rotation =  T3Matrix4::Rotation(value, rotatedTarget) * rotation;
	}

	/**
	<summary>Updates the camera</summary>
	*/
	virtual void UpdateCamera();
	/**
	<summary>Builds a view matrix for the current camera variables, suitable for sending straight
	to a vertex shader (i.e it's already an 'inverse camera matrix')</summary>
	*/
	T3Matrix4 BuildViewMatrix();
protected:
	T3Vector3 position;
	T3Matrix4 rotation;
	T3Vector3 originalUp;
	T3Vector3 originalRight;
	T3Vector3 originalTarget;
	T3Vector3 rotatedTarget;
	T3Vector3 rotatedUp;
	T3Vector3 rotatedRight;
	bool invertY;
};