#pragma once



namespace Math
{
	class Vector4;

	class FOG_API Vector3
	{
	public:
		inline Vector3() { mVec = SplatZero(); }
		inline Vector3(float x, float y, float z) { mVec = DirectX::XMVectorSet(x, y, z, z); }
		inline Vector3(const DirectX::XMFLOAT3& v) { mVec = DirectX::XMLoadFloat3(&v); }
		inline Vector3(const Vector3& v) { mVec = v; }
		inline Vector3(Scalar s) { mVec = s; }
		inline Vector3(Vector4 vec);
		inline Vector3(DirectX::FXMVECTOR vec) { mVec = vec; }
		static Vector3 Zero() { return SplatZero(); }
		static Vector3 Identity() { return SplatOne(); }
		static Vector3 OneX() { return CreateXUnitVector(); }
		static Vector3 OneY() { return CreateYUnitVector(); }
		static Vector3 OneZ() { return CreateZUnitVector(); }

		Scalar GetX() const { return Scalar(DirectX::XMVectorSplatX(mVec)); }
		Scalar GetY() const { return Scalar(DirectX::XMVectorSplatY(mVec)); }
		Scalar GetZ() const { return Scalar(DirectX::XMVectorSplatZ(mVec)); }
		void SetX(Scalar x) { mVec = DirectX::XMVectorPermute<4, 1, 2, 3>(mVec, x); }
		void SetY(Scalar y) { mVec = DirectX::XMVectorPermute<0, 5, 2, 3>(mVec, y); }
		void SetZ(Scalar z) { mVec = DirectX::XMVectorPermute<0, 1, 6, 3>(mVec, z); }
		
		Vector3 operator* (Vector3 v2) const { return Vector3(DirectX::XMVectorMultiply(mVec, v2)); }
		Vector3 operator* (Scalar  v2) const { return *this * Vector3(v2); }
		Vector3 operator+ (Vector3 v) const { return Vector3(DirectX::XMVectorAdd(mVec, v)); }
		Vector3 operator- () const { return Vector3(DirectX::XMVectorNegate(mVec)); }
		Vector3 operator- (Vector3 v) const { return Vector3(DirectX::XMVectorSubtract(mVec, v)); }
		Vector3 operator* (float  v2) const { return *this * Scalar(v2); }

		Vector3& operator+= (Vector3 v) { *this = *this + v; return *this; }

		operator DirectX::XMVECTOR() const { return mVec; }
		operator DirectX::XMFLOAT3() const { DirectX::XMFLOAT3 res;  DirectX::XMStoreFloat3(&res, mVec); return res; }

	private:
		DirectX::XMVECTOR mVec;
	};

	class Matrix4;

	class FOG_API Vector4
	{
	public:
		inline Vector4() { mVec = SplatZero(); }
		inline Vector4(float x, float y, float z, float w) { mVec = DirectX::XMVectorSet(x, y, z, w); }
		inline Vector4(const DirectX::XMFLOAT4& v) { mVec = DirectX::XMLoadFloat4(&v); }
		inline Vector4(Vector3 xyz, float w) { mVec = DirectX::XMVectorSetW(xyz, w); }
		inline Vector4(const Vector4& v) { mVec = v; }
		inline Vector4(const Vector3& v) { mVec = v; }
		inline Vector4(const Scalar& s) { mVec = s; }
		inline explicit Vector4(Vector3 xyz) { mVec = SetWToOne(xyz); }
		inline explicit Vector4(DirectX::FXMVECTOR vec) { mVec = vec; }

		Scalar GetX() const { return Scalar(DirectX::XMVectorSplatX(mVec)); }
		Scalar GetY() const { return Scalar(DirectX::XMVectorSplatY(mVec)); }
		Scalar GetZ() const { return Scalar(DirectX::XMVectorSplatZ(mVec)); }
		Scalar GetW() const { return Scalar(DirectX::XMVectorSplatW(mVec)); }

		inline Vector4 operator- () const { return Vector4(DirectX::XMVectorNegate(mVec)); }
		inline Vector4 operator+ (Vector4 v2) const { return Vector4(DirectX::XMVectorAdd(mVec, v2)); }
		inline Vector4 operator- (Vector4 v2) const { return Vector4(DirectX::XMVectorSubtract(mVec, v2)); }
		inline Vector4 operator* (Vector4 v2) const { return Vector4(DirectX::XMVectorMultiply(mVec, v2)); }
		inline Vector4 operator/ (Vector4 v2) const { return Vector4(DirectX::XMVectorDivide(mVec, v2)); }
		inline Vector4 operator* (Scalar  v2) const { return *this * Vector4(v2); }
		inline Vector4 operator/ (Scalar  v2) const { return *this / Vector4(v2); }
		inline Vector4 operator* (float   v2) const { return *this * Scalar(v2); }
		inline Vector4 operator/ (float   v2) const { return *this / Scalar(v2); }

		operator DirectX::XMVECTOR() const { return mVec; }
		operator DirectX::XMFLOAT4() const { DirectX::XMFLOAT4 res;  DirectX::XMStoreFloat4(&res, mVec); return res; }
		Vector4 operator* (Matrix4 mat) const;

	private:
		DirectX::XMVECTOR mVec;
	};


	inline Vector3::Vector3(Vector4 vec) : mVec((DirectX::XMVECTOR)vec) {}

	class FOG_API BoolVector
	{
	public:
		BoolVector(DirectX::FXMVECTOR vec) { mVec = vec; }
		operator DirectX::XMVECTOR() const { return mVec; }
	protected:
		DirectX::XMVECTOR mVec;
	};


}