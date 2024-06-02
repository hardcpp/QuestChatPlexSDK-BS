#pragma once

#include <System/DateTime.hpp>
#include <System/TimeSpan.hpp>
#include <UnityEngine/Color.hpp>
#include <UnityEngine/Vector2.hpp>
#include <UnityEngine/Vector3.hpp>
#include <UnityEngine/Quaternion.hpp>

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