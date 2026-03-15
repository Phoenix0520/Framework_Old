#pragma once

#include "Framework.h"

class Math
{
public:
	static const float PI;
	static const float EPSILON;

	static float Modulo(float val1, float val2);

	static UINT Factorial(UINT val);
	static UINT Permutation(UINT elem, UINT choose);
	static UINT Combination(UINT elem, UINT choose);

	static Vector3 Mul(const Vector3& v1, const Vector3& v2);
	static Vector3 RandomDirection(const float& min, const float& max);

	static Vector3 RotateVector(const Quaternion& q, const Vector3& look);
	static Quaternion DirectionToQuaternion(const Vector3& dir);
	static Vector3 DirectionToVector3(const Vector3& dir);

	static float ToRadian(float degree);
	static Vector3 ToRadian(Vector3 vec);
	static float ToDegree(float radian);
	static Vector3 ToDegree(Vector3 vec);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);
	
	static D3DXVECTOR2 RandomVec2(float r1, float r2);
	static D3DXVECTOR3 RandomVec3(float r1, float r2);
	static D3DXVECTOR4 RandomVec4(float r1, float r2);
	static D3DXCOLOR RandomColor3();
	static D3DXCOLOR RandomColor4();

	static float Clamp(float value, float min, float max);
	static float Lerp(float value1, float value2, float t);
	static Matrix Lerp(const Matrix& mat1, const Matrix& mat2, float t);

	static float Normalize(float value, float min = 0.0f, float max = 1.0f);

	static void LerpMatrix(OUT D3DXMATRIX& out, const D3DXMATRIX& m1, const D3DXMATRIX& m2, float amount);

	static D3DXQUATERNION LookAt(const D3DXVECTOR3& origin, const D3DXVECTOR3& target, const D3DXVECTOR3& up);
	static float Gaussian(float val, UINT blurCount);

	static void MatrixDecompose(const D3DXMATRIX& m, OUT Vector3& S, OUT Vector3& R, OUT Vector3& T);
};