#if WINDOWS_BUILD

#include "Frustum.h"

void Frustum::FromMatrix(const Matrix4 &mat) {
	T3Vector3 xaxis = T3Vector3(mat.values[0],mat.values[4],mat.values[8]);
	T3Vector3 yaxis = T3Vector3(mat.values[1],mat.values[5],mat.values[9]);
	T3Vector3 zaxis = T3Vector3(mat.values[2],mat.values[6],mat.values[10]);
	T3Vector3 waxis = T3Vector3(mat.values[3],mat.values[7],mat.values[11]);

	//RIGHT
	planes[0] = Plane(waxis - xaxis, (mat.values[15] - mat.values[12]), true);

	//LEFT
	planes[1] = Plane(waxis + xaxis, (mat.values[15] + mat.values[12]), true);

	//BOTTOM
	planes[2] = Plane(waxis + yaxis, (mat.values[15] + mat.values[13]), true);

	//TOP
	planes[3] = Plane(waxis - yaxis, (mat.values[15] - mat.values[13]), true);

	//FAR
	planes[4] = Plane(waxis - zaxis, (mat.values[15] - mat.values[14]), true);

	//NEAR
	planes[5] = Plane(waxis + zaxis, (mat.values[15] + mat.values[14]), true);
}

bool Frustum::InsideFrustum(SceneNode&n)	{
	for(int p = 0; p < 6; p++ )	{
		//if(!planes[p].PointInPlane(n.GetWorldTransform().GetPositionVector())) {
		if(!planes[p].SphereInPlane(n.GetWorldTransform().GetPositionVector(),n.GetBoundingRadius())) {
			return false;
		}
	}
	return true;
}

bool	Frustum::AABBInsideFrustum(T3Vector3 &position, const T3Vector3 &size)	const {
	for(int p = 0; p < 6; p++ )	{
		if(planes[p].PointInPlane(T3Vector3(position.x -size.x , position.y +size.y,position.z +size.z))) {
			continue;
		}
		if(planes[p].PointInPlane(T3Vector3(position.x +size.x , position.y +size.y,position.z +size.z))) {
			continue;
		}
		if(planes[p].PointInPlane(T3Vector3(position.x -size.x , position.y -size.y,position.z +size.z))) {
			continue;
		}
		if(planes[p].PointInPlane(T3Vector3(position.x +size.x , position.y -size.y,position.z +size.z))) {
			continue;
		}

		if(planes[p].PointInPlane(T3Vector3(position.x -size.x , position.y +size.y,position.z -size.z))) {
			continue;
		}
		if(planes[p].PointInPlane(T3Vector3(position.x +size.x , position.y +size.y,position.z -size.z))) {
			continue;
		}
		if(planes[p].PointInPlane(T3Vector3(position.x -size.x , position.y -size.y,position.z -size.z))) {
			continue;
		}
		if(planes[p].PointInPlane(T3Vector3(position.x +size.x , position.y -size.y,position.z -size.z))) {
			continue;
		}

		return false;
	}

	return true;
}

#endif