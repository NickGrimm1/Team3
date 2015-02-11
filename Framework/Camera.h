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
#include "Matrix4.h"
#include "Vector3.h"
#include "../Team3Project1/DrawableEntity3D.h"

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
	<param name='target'>The initial target position. Default is (0, 0, -1).</param>
	<param name='up'>The initial up vector. Default is (0, 1, 0).</param>
	*/
	Camera(DrawableEntity3D* targetEntity = NULL, float pitch = 0, float yaw = 0, float roll = 0, Vector3 position = Vector3(), Vector3 target = Vector3(0, 0, -1), Vector3 up = Vector3(0, 1, 0))
		: pitch(pitch), yaw(yaw), roll(roll), position(position), originalTarget(target), originalUp(up), invertY(invertY), targetEntity(targetEntity)
	{ }
	~Camera(void){}

	/**
	<summary>Gets the camera's position in world space.</summary>
	<param name='value'>The amount of pitch to add.</param>
	*/
	Vector3 GetPosition() const { return position;}
	/**
	<summary>Sets the camera's position in world space.</summary>
	<param name='value'>The position</param>
	*/
	void SetPosition(const Vector3& value) { position = value;}
	/**
	<summary>Gets the current Yaw.</summary>
	*/
	float GetYaw() const { return yaw;}
	/**
	<summary>Sets the current Yaw.</summary>
	<param name='value'>The yaw.</param>
	*/
	void SetYaw(const float value) {yaw = value;}
	/**
	<summary>Gets the current pitch.</summary>
	*/
	float GetPitch() const { return pitch;}
	/**
	<summary>Sets the current pitch.</summary>
	<param name='value'>The pitch.</param>
	*/
	void SetPitch(const float value) {pitch = value;}
	/**
	<summary>Gets the current roll.</summary>
	*/
	float GetRoll() const { return roll;}
	/**
	<summary>Sets the current roll.</summary>
	<param name='value'>The roll.</param>
	*/
	void SetRoll(const float value) {roll = value;}
	
	/**
	<summary>Adds movement to this camera's position, automatically calculates rotation.</summary>
	<param name='value'>The amount of movement to add in the original axes provided.</param>
	*/
	void AddMovement(const Vector3& value) { position += rotation * value; }
	/**
	<summary>Adds movement to this camera's position, using the raw co-ordinates given.</summary>
	<param name='value'>The amount of movement to add.</param>
	*/
	void AddRawMovement(const Vector3& value) { position += value; }
	/**
	<summary>Adds pitch to this camera's rotation.</summary>
	<param name='value'>The amount of pitch to add.</param>
	*/
	void AddPitch(const float value) { pitch += value; }
	/**
	<summary>Adds yaw to this camera's rotation.</summary>
	<param name='value'>The amount of yaw to add.</param>
	*/
	void AddYaw(const float value) { yaw += value; }
	/**
	<summary>Adds roll to this camera's rotation.</summary>
	<param name='value'>The amount of roll to add.</param>
	*/
	void AddRoll(const float value) { roll += value; }

	/**
	<summary>Updates the camera</summary>
	*/
	virtual void UpdateCamera();
	/**
	<summary>Builds a view matrix for the current camera variables, suitable for sending straight
	to a vertex shader (i.e it's already an 'inverse camera matrix')</summary>
	*/
	Matrix4 BuildViewMatrix();
protected:
	float yaw;
	float pitch;
	float roll;
	Vector3 position;
	Matrix4 rotation;
	Vector3 originalUp;
	Vector3 originalTarget;
	Vector3 rotatedTarget;
	Vector3 rotatedUp;
	bool invertY;
	DrawableEntity3D* targetEntity;
};