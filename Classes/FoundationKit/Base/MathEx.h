/****************************************************************************
  Copyright (c) 2015 libo All rights reserved.
 
  losemymind.libo@gmail.com
****************************************************************************/

// https://github.com/mosra/magnum/tree/master/src/Magnum/Math


#ifndef LOSEMYMIND_MATHEX_H
#define LOSEMYMIND_MATHEX_H
#pragma once
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Base/MathContent.h"

NS_FK_BEGIN

/// Provides constants and static methods for trigonometric, 
/// logarithmic and other common mathematical functions.
namespace MathEx
{

    /// returns the maximum of two values
    template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    max(T v1, T v2)
    {
        return v1 > v2 ? v1 : v2;
    }

    /// returns the minimum of two values
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type >
    FORCEINLINE T min(T v1, T v2)
    {
        return v1 < v2 ? v1 : v2;
    }

	/// Returns the absolute value of val.
    template< typename T>
    FORCEINLINE T abs(T val, typename std::enable_if< std::is_arithmetic<T>::value>::type* = nullptr)
	{
        return std::abs(val);
    }

	/// Returns the arc-cosine of val - the angle in radians whose cosine is val.
	template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	acos(T val)
	{
        return std::acos(val);
    }

	/// Returns the arc-sine of val - the angle in radians whose sine is val.
	template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	asin(const T val)
	{ 
        return std::asin(val);
    }

	/// Returns the arc-tangent of val - the angle in radians whose tangent is val.
	template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	atan(const T val)
	{ 
        return std::atan(val); 
    }

	/// Returns the angle in radians whose Tan is y/x.
	template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	atan2(const T y, const T x)
	{ 
        return std::atan2(y, x); 
    }

	/// Returns the smallest integer greater to or equal to val.
	template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	ceil(const T val)
	{ 
        return std::ceil(val); 
    }

	/// Returns the smallest integer greater to or equal to val.
	template< typename T >
    FORCEINLINE int ceilToInt(const T val, typename std::enable_if< std::is_arithmetic<T>::value >::type* = nullptr)
	{
        return static_cast<int>(std::ceil(val)); 
    }

	/// Returns the largest integer smaller to or equal to val.
	template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	floor(T val)
	{
		return std::floor(val);
	}

	/// Returns the largest integer smaller to or equal to val.
	template< typename T >
    FORCEINLINE int floorToInt(T val, typename std::enable_if< std::is_arithmetic<T>::value >::type* = nullptr)
	{
		return static_cast<int>(std::floor(val));
	}

	/// Clamps val value between a minimum and maximum value.
	template< typename T>
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	clamp(T val, T minVal, T maxVal)
	{
        return std::min(std::max(val, minVal), maxVal);
	}

	/// Clamps value between 0 and 1 and returns value.
	template< typename T>
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	clamp01(T val)
	{ 
        return std::min(std::max(val, 0), 1);
    }

	/// Returns the cosine of angle f in radians.
	template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	cos(T val)
	{ 
        return std::cos(val);
    }

	/// Returns e raised to the specified power.
	template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
	exp(T power)
	{ 
        return std::exp(power);
    }

    /// Returns the natural (base e) logarithm of val specified value.
    template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    log(T val)
    {
        return std::log(val);
    }

    /// Returns the base 10 logarithm of val specified value.
    template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    log10(T val0)
    {
        return std::log10(val0);
    }


    /// Returns the sign of val.
    template <typename T>
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    sign(T val)
    {
        return (val >(T)0) ? (T)1 : ((val < (T)0) ? (T)-1 : (T)0);
    }

    /// Returns the sine of angle val in radians.
    template <typename T>
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    sin(T val)
    {
        return std::sin(val);
    }

    /// Returns square root of val.
    template <typename T>
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    sqrt(T val)
    {
        return std::sqrt(val);
    }

    /// Returns the tangent of angle val in radians.
    template <typename T>
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    tan(T val)
    {
        return std::tan(val);
    }

    /// Returns f raised to power p.
    template< typename T, typename U >
    FORCEINLINE T pow(T f, U p,
        typename std::enable_if< std::is_arithmetic<T>::value >::type* = nullptr,
        typename std::enable_if< std::is_arithmetic<U>::value >::type* = nullptr)
    {
        return std::pow(f, p);
    }

   /**
    * Compares two values if they are similar.
    * Due to floating point imprecision it is not
    * recommended to compare floats using the equal
    * operator. eg. 1.0 == 10.0 / 10.0 might not return true.
    */
    template< typename T1, typename T2 >
    FORCEINLINE bool approximately(const T1 a, const T2 b,
        typename std::enable_if< std::is_arithmetic<T1>::value >::type* = nullptr,
        typename std::enable_if< std::is_arithmetic<T2>::value >::type* = nullptr)
    {
        return roughlyEqual(a, b, 0.1f);
    }

	/// Interpolates between from and to by t. t is clamped between 0 and 1.
	template< typename T, typename U, typename V>
    FORCEINLINE float Lerp(T from, U to, V t,
        typename std::enable_if< std::is_arithmetic<T>::value >::type* = nullptr,
        typename std::enable_if< std::is_arithmetic<U>::value >::type* = nullptr,
        typename std::enable_if< std::is_arithmetic<V>::value >::type* = nullptr)
	{ 
		return t >= 1 ? to : t < 0 ? from : from + (to - from) * t; 
	}

   /**
    * Compares two floating point values if they are similar.
    * @param[in] v1 First value.
    * @param[in] v2 Second value.
    * @param[in] threshold The threshold of similarity
    * @return True if the values are similar, otherwise false.
    */
	template< typename T, typename U, typename V>
    FORCEINLINE bool roughlyEqual(const T v1, const U v2, const V threshold = 0.01f)
	{ 
        return std::abs(v1 - v2) <= threshold;  
    }

	/// Returns f rounded to the nearest integer.
    template <typename T>
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    round(T val)
    {
#if TARGET_PLATFORM == PLATFORM_ANDROID
        return (val > T(0)) ? std::floor(val + T(0.5)) : std::ceil(val - T(0.5));
#else
        return std::round(val);
#endif
    }

    FORCEINLINE bool equal(const float& v1, const float& v2)
    {
        return (std::fabs(v1 - v2) < 1E-12);
    }

    FORCEINLINE bool equal(const double& v1, const double& v2)
    {
        return (std::fabs(v1 - v2) < 1E-12);
    }

    template< typename T >
    FORCEINLINE bool fuzzyEqual(T v1, T v2, T var)
    {
        return(v1 - var <= v2 && v2 <= v1 + var);
    }

    /// Return true if the parameter is equal to zero
    FORCEINLINE bool isZero(double val)
    {
        return ((-MinDouble < val) && (val < MinDouble));
    }

    /// Returns if the value is powered by two.
    FORCEINLINE bool isPowerOfTwo(int val)
    {
        return (val > 0) && ((val & (val - 1)) == 0);
    }

    FORCEINLINE double sigmoid(double input, double response = 1.0)
    {
        return (1.0 / (1.0 + exp(-input / response)));
    }

    // return true is the third parameter is in the range described by the first two
    template< typename T, typename U, typename V>
    FORCEINLINE bool inRange(T start, U end, V val)
    {
        if (start < end)
        {
            return ((val > start) && (val < end));
        }
        else
        {
            return ((val < start) && (val > end));
        }
    }


   /** Converts radians to degrees.
    *  This method uses double precission internally,
    *  though it returns single float
    *  Factor = 180 / pi
    *  @param[in] radians The angle in radians.
    *  @return    The angle in degrees.
    */
    FORCEINLINE float radiansToDegrees(float radians)
    {
        return static_cast<float>(radians * 57.295779513082320876798154814105);
    }

   /** Converts degrees to radians.
    *  This method uses double precission internally,
    *  though it returns single float
    *  Factor = pi / 180
    *  @param[in]  degrees  The angle in degrees.
    *  @return     The angle in radians.
    */
    FORCEINLINE float degreesToRadians(float degrees)
    {
        return static_cast<float>(degrees * 0.017453292519943295769236907684886);
    }

	template<typename T>
	double average(const std::vector<T>& vec,
        typename std::enable_if< std::is_arithmetic<T>::value >::type* = nullptr)
	{
		double dAverage = 0.0;
		for (auto& i : vec )
		{
			dAverage += static_cast<double>(i);
		}
		return dAverage / static_cast<double>(vec.size());
	}

	/// Get the next power of two after val value.
	int nextPowerOfTwo(int val);

	/// Returns the closest power of two to val value.
	int closestPowerOfTwo(int val);

	double standardDeviation(const std::vector<double>& vec);

} // namespace Math

NS_FK_END

#endif // LOSEMYMIND_MATHEX_H

















