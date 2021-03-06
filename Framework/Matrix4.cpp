#include "T3Matrix4.h"
#include <iostream>

T3Matrix4::T3Matrix4(void)	{
	ToIdentity();
}

T3Matrix4::T3Matrix4( float elements[16] )	{
	memcpy(this->values,elements,16*sizeof(float));
}

T3Matrix4::~T3Matrix4(void)	{
	ToIdentity();
}

void T3Matrix4::ToIdentity() {
	ToZero();
	values[0]  = 1.0f;
	values[5]  = 1.0f;
	values[10] = 1.0f;
	values[15] = 1.0f;
}

void T3Matrix4::ToZero()	{
	for(int i = 0; i < 16; i++)	{
		values[i] = 0.0f;
	}
}

T3Vector3 T3Matrix4::GetPositionVector() const{
	return T3Vector3(values[12],values[13],values[14]);
}

void	T3Matrix4::SetPositionVector(const T3Vector3 in) {
	values[12] = in.x;
	values[13] = in.y;
	values[14] = in.z;		
}

T3Vector3 T3Matrix4::GetScalingVector() const{
	return T3Vector3(values[0],values[5],values[10]);
}

void	T3Matrix4::SetScalingVector(const T3Vector3 &in) {
	values[0]  = in.x;
	values[5]  = in.y;
	values[10] = in.z;		
}

T3Matrix4 T3Matrix4::Perspective(float znear, float zfar, float aspect, float fov) {
	T3Matrix4 m;

	const float h = 1.0f / tan(fov*PI_OVER_360);
	float neg_depth = znear-zfar;

	m.values[0]		= h / aspect;
	m.values[5]		= h;
	m.values[10]	= (zfar + znear)/neg_depth;
	m.values[11]	= -1.0f;
	m.values[14]	= 2.0f*(znear*zfar)/neg_depth;
	m.values[15]	= 0.0f;

	return m;
}

//http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
T3Matrix4 T3Matrix4::Orthographic(float znear, float zfar,float right, float left, float top, float bottom)	{
	T3Matrix4 m;

	m.values[0]	= 2.0f / (right-left);
	m.values[5]	= 2.0f / (top-bottom);
	m.values[10]	= -2.0f / (zfar-znear);

	m.values[12]  = -(right+left)/(right-left);
	m.values[13]  = -(top+bottom)/(top-bottom);
	m.values[14]  = -(zfar+znear)/(zfar-znear);
	m.values[15]  = 1.0f;

	return m;
}

T3Matrix4 T3Matrix4::BuildViewMatrix(const T3Vector3 &from, const T3Vector3 &lookingAt, const T3Vector3 up /*= T3Vector3(0,1,0)*/ )	{
	
	T3Matrix4 r;
	r.SetPositionVector(T3Vector3(-from.x,-from.y,-from.z));

	T3Matrix4 m;

	T3Vector3 f = (lookingAt - from);
	f.Normalise();

	T3Vector3 s = T3Vector3::Cross(f,up);
	T3Vector3 u = T3Vector3::Cross(s,f);

	s.Normalise();
	u.Normalise();

	m.values[0] = s.x;
	m.values[4] = s.y;
	m.values[8] = s.z;

	m.values[1] = u.x;
	m.values[5] = u.y;
	m.values[9] = u.z;

	m.values[2]  = -f.x;
	m.values[6]  = -f.y;
	m.values[10] = -f.z;

	return m*r;
}

T3Matrix4 T3Matrix4::Rotation(float degrees, const T3Vector3 &inaxis)	 {
	T3Matrix4 m;

	T3Vector3 axis = inaxis;

	axis.Normalise();

	float c = cos((float)DegToRad(degrees));
	float s = sin((float)DegToRad(degrees));

	m.values[0]  = (axis.x * axis.x) * (1.0f - c) + c;
	m.values[1]  = (axis.y * axis.x) * (1.0f - c) + (axis.z * s);
	m.values[2]  = (axis.z * axis.x) * (1.0f - c) - (axis.y * s);

	m.values[4]  = (axis.x * axis.y) * (1.0f - c) - (axis.z * s);
	m.values[5]  = (axis.y * axis.y) * (1.0f - c) + c;
	m.values[6]  = (axis.z * axis.y) * (1.0f - c) + (axis.x * s);

	m.values[8]  = (axis.x * axis.z) * (1.0f - c) + (axis.y * s);
	m.values[9]  = (axis.y * axis.z) * (1.0f - c) - (axis.x * s);
	m.values[10] = (axis.z * axis.z) * (1.0f - c) + c;

	return m;
}

T3Matrix4 T3Matrix4::Rotation(T3Vector3 &x, T3Vector3 &y, T3Vector3 &z) {

	T3Matrix4 m;
	m.ToIdentity();

	x.Normalise();
	y.Normalise();
	z.Normalise();

	m.values[0] = x.x;
	m.values[1] = x.y;
	m.values[2] = x.z;

	m.values[4] = y.x;
	m.values[5] = y.y;
	m.values[6] = y.z;

	m.values[8]  = z.x;
	m.values[9]  = z.y;
	m.values[10] = z.z;

	return m;
}

T3Matrix4 T3Matrix4::Scale( const T3Vector3 &scale )	{
	T3Matrix4 m;

	m.values[0]  = scale.x;
	m.values[5]  = scale.y;
	m.values[10] = scale.z;	

	return m;
}

T3Matrix4 T3Matrix4::Translation( const T3Vector3 &translation )	{
	T3Matrix4 m;

	m.values[12] = translation.x;
	m.values[13] = translation.y;
	m.values[14] = translation.z;	

	return m;
}

T3Matrix4 T3Matrix4::GetTransposedRotation() {
	T3Matrix4 temp;
	temp.values[0] = values[0];
	temp.values[5] = values[5];
	temp.values[10] = values[10];
	temp.values[1] = values[4];
	temp.values[4] = values[1];
	temp.values[2] = values[8];
	temp.values[8] = values[2];
	temp.values[6] = values[9];
	temp.values[9] = values[6];
	return temp;
}