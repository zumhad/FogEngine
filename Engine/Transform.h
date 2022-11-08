#pragma once

namespace Math
{
    class FOG_API OrthogonalTransform
    {
    public:
        inline OrthogonalTransform() : m_rotation(Quaternion::Identity()), m_translation(Vector3::Zero()) {}
        inline OrthogonalTransform(Quaternion rotate) : m_rotation(rotate), m_translation(Vector3::Zero()) {}
        inline OrthogonalTransform(Vector3 translate) : m_rotation(Quaternion::Identity()), m_translation(translate) {}
        inline OrthogonalTransform(Quaternion rotate, Vector3 translate) : m_rotation(rotate), m_translation(translate) {}
        inline OrthogonalTransform(const Matrix3& mat) : m_rotation(mat), m_translation(Vector3::Zero()) {}
        inline OrthogonalTransform(const Matrix3& mat, Vector3 translate) : m_rotation(mat), m_translation(translate) {}

        static OrthogonalTransform Identity() { return OrthogonalTransform(); }
        inline explicit OrthogonalTransform(const DirectX::XMMATRIX& mat) { *this = OrthogonalTransform(Matrix3(mat), Vector3(mat.r[3])); }

        inline void SetRotation(Quaternion q) { m_rotation = q; }
        inline void SetTranslation(Vector3 v) { m_translation = v; }

        inline Quaternion GetRotation() const { return m_rotation; }
        inline Vector3 GetTranslation() const { return m_translation; }

        static inline OrthogonalTransform MakeXRotation(float angle) { return OrthogonalTransform(Quaternion(Vector3::OneX(), angle)); }
        static inline OrthogonalTransform MakeYRotation(float angle) { return OrthogonalTransform(Quaternion(Vector3::OneY(), angle)); }
        static inline OrthogonalTransform MakeZRotation(float angle) { return OrthogonalTransform(Quaternion(Vector3::OneZ(), angle)); }
        static inline OrthogonalTransform MakeTranslation(Vector3 translate) { return OrthogonalTransform(translate); }

        inline Vector3 operator* (Vector3 vec) const { return m_rotation * vec + m_translation; }
        inline Vector4 operator* (Vector4 vec) const 
        {
            return
                Vector4(SetWToZero(m_rotation * Vector3((DirectX::XMVECTOR)vec))) +
                Vector4(SetWToOne(m_translation)) * vec.GetW();
        }

        inline OrthogonalTransform operator* (const OrthogonalTransform& xform) const 
        {
            return OrthogonalTransform(m_rotation * xform.m_rotation, m_rotation * xform.m_translation + m_translation);
        }

        inline OrthogonalTransform operator~ () const 
        {
            Quaternion invertedRotation = ~m_rotation;
            return OrthogonalTransform(invertedRotation, invertedRotation * -m_translation);
        }

    private:

        Quaternion m_rotation;
        Vector3 m_translation;
    };
}