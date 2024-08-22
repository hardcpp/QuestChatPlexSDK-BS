#pragma once

#include <System/DateTime.hpp>
#include <System/TimeSpan.hpp>
#include <UnityEngine/Color.hpp>
#include <UnityEngine/Vector2.hpp>
#include <UnityEngine/Vector3.hpp>
#include <UnityEngine/Quaternion.hpp>
#include <UnityEngine/Mathf.hpp>

#include <numbers>

constexpr System::TimeSpan operator- (const System::DateTime& p_A, const System::DateTime& p_B)
{
    return System::DateTime::op_Subtraction(p_A, p_B);
}

constexpr bool operator> (const System::DateTime& p_A, const System::DateTime& p_B)
{
    return System::DateTime::op_GreaterThan(p_A, p_B);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

constexpr UnityEngine::Color operator+ (const UnityEngine::Color& p_A, const UnityEngine::Color& p_B)
{
    return UnityEngine::Color::op_Addition(p_A, p_B);
}
constexpr UnityEngine::Color operator+= (UnityEngine::Color& p_A, const UnityEngine::Color& p_B)
{
    return p_A = UnityEngine::Color::op_Addition(p_A, p_B);
}

constexpr UnityEngine::Color operator- (const UnityEngine::Color& p_A, const UnityEngine::Color& p_B)
{
    return UnityEngine::Color::op_Subtraction(p_A, p_B);
}
constexpr UnityEngine::Color operator-= (UnityEngine::Color& p_A, const UnityEngine::Color& p_B)
{
    return p_A = UnityEngine::Color::op_Subtraction(p_A, p_B);
}

constexpr UnityEngine::Color operator* (const UnityEngine::Color& p_A, const UnityEngine::Color& p_B)
{
    return UnityEngine::Color::op_Multiply(p_A, p_B);
}
constexpr UnityEngine::Color operator* (const UnityEngine::Color& p_A, const float& p_B)
{
    return UnityEngine::Color::op_Multiply(p_A, p_B);
}
constexpr UnityEngine::Color operator* (const float& p_A, const UnityEngine::Color& p_B)
{
    return UnityEngine::Color::op_Multiply(p_A, p_B);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

constexpr UnityEngine::Vector2 operator+ (const UnityEngine::Vector2& p_A, const UnityEngine::Vector2& p_B)
{
    return UnityEngine::Vector2::op_Addition(p_A, p_B);
}
constexpr UnityEngine::Vector2 operator+= (UnityEngine::Vector2& p_A, const UnityEngine::Vector2& p_B)
{
    return p_A = UnityEngine::Vector2::op_Addition(p_A, p_B);
}

constexpr UnityEngine::Vector2 operator- (const UnityEngine::Vector2& p_A, const UnityEngine::Vector2& p_B)
{
    return UnityEngine::Vector2::op_Subtraction(p_A, p_B);
}
constexpr UnityEngine::Vector2 operator-= (UnityEngine::Vector2& p_A, const UnityEngine::Vector2& p_B)
{
    return p_A = UnityEngine::Vector2::op_Subtraction(p_A, p_B);
}

constexpr UnityEngine::Vector2 operator* (const UnityEngine::Vector2& p_A, const float& p_B)
{
    return UnityEngine::Vector2::op_Multiply(p_A, p_B);
}
constexpr UnityEngine::Vector2 operator* (const float& p_A, const UnityEngine::Vector2& p_B)
{
    return UnityEngine::Vector2::op_Multiply(p_A, p_B);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

constexpr UnityEngine::Vector3 operator+ (const UnityEngine::Vector3& p_A, const UnityEngine::Vector3& p_B)
{
    return UnityEngine::Vector3::op_Addition(p_A, p_B);
}
constexpr UnityEngine::Vector3 operator+= (UnityEngine::Vector3& p_A, const UnityEngine::Vector3& p_B)
{
    return p_A = UnityEngine::Vector3::op_Addition(p_A, p_B);
}

constexpr UnityEngine::Vector3 operator- (const UnityEngine::Vector3& p_A, const UnityEngine::Vector3& p_B)
{
    return UnityEngine::Vector3::op_Subtraction(p_A, p_B);
}
constexpr UnityEngine::Vector3 operator-= (UnityEngine::Vector3& p_A, const UnityEngine::Vector3& p_B)
{
    return p_A = UnityEngine::Vector3::op_Subtraction(p_A, p_B);
}

constexpr UnityEngine::Vector3 operator* (const UnityEngine::Vector3& p_A, const float& p_B)
{
    return UnityEngine::Vector3::op_Multiply(p_A, p_B);
}
constexpr UnityEngine::Vector3 operator* (const float& p_A, const UnityEngine::Vector3& p_B)
{
    return UnityEngine::Vector3::op_Multiply(p_A, p_B);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

constexpr UnityEngine::Quaternion operator* (const UnityEngine::Quaternion& p_A, const UnityEngine::Quaternion& p_B)
{
    return UnityEngine::Quaternion::op_Multiply(p_A, p_B);
}

constexpr UnityEngine::Vector3 operator* (const UnityEngine::Quaternion& p_A, const UnityEngine::Vector3& p_B)
{
    return UnityEngine::Quaternion::op_Multiply(p_A, p_B);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

static UnityEngine::Vector3 Vector3RotateTowards(UnityEngine::Vector3 p_Current, UnityEngine::Vector3 p_Target, float p_MaxRadiansDelta, float p_MaxMagnitudeDelta)
{
    const float Deg2Rad = static_cast<float>(std::numbers::pi) / 180.0f;
    const float Rad2Deg = 57.29578f;

    auto l_FromDirection    = p_Current.get_normalized();
    auto l_ToDirection      = p_Target.get_normalized();
    auto l_AngleRadians     = UnityEngine::Mathf::Acos(UnityEngine::Vector3::Dot(l_FromDirection, l_ToDirection));

    if (l_AngleRadians < std::numeric_limits<float>::epsilon())
        return p_Current;

    auto l_ClampedAngleRad  = UnityEngine::Mathf::Min(p_MaxRadiansDelta, l_AngleRadians);
    auto l_Axis             = UnityEngine::Vector3::Cross(l_FromDirection, l_ToDirection);

    if (l_Axis.get_sqrMagnitude() < std::numeric_limits<float>::epsilon())
        l_Axis = UnityEngine::Vector3::get_up();

    auto l_Rotation         = UnityEngine::Quaternion::AngleAxis(l_ClampedAngleRad * Rad2Deg, l_Axis);
    auto l_RotatedVector    = l_Rotation * l_FromDirection;
    auto l_CurrentMagnitude = p_Current.get_magnitude();
    auto l_TargetMagnitude  = p_Target.get_magnitude();
    auto l_ClampedMagnitude = UnityEngine::Mathf::MoveTowards(l_CurrentMagnitude, l_TargetMagnitude, p_MaxMagnitudeDelta);

    return l_RotatedVector * l_ClampedMagnitude;
}