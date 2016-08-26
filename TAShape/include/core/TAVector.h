#ifndef _TA_VECTOR_
#define _TA_VECTOR_

#include <iostream>

namespace TAShape
{
	/**
	* @brief	A simple 3D vector class.
	*
	*/
	class TAVector
	{
	public:

		/**
		* @brief						Constructor
		*/
		TAVector();

		/**
		* @brief						Constructor
		* @param	_x_					x direction of the vector
		* @param	_y_					y direction of the vector
		* @param	_z_					z direction of the vector
		*/
		TAVector(const float& _x_, const float& _y_, const float& _z_);

		/**
		* @brief						Constructor
		* @param	_other_				Vector of the type TAVector to be copied into this vector
		*/
		TAVector(const TAVector& _other_);

		/**
		* @brief						Constructor
		*								Creates the vector between the points _p1_ and _p2_ (_p1_ ------> _p2_)
		* @param	_p1_				Source point of the vector of the type TAVector
		* @param	_p2_				Destination point of the vector of the type TAVector
		*/
		TAVector(const TAVector& _p1_, const TAVector& _p2_);

		/**
		* @brief						+ operator
		*								Returns the summation of this vector and _other_vector
		* @param	_other_				Vector of the type TAVector to be summed
		* @return						Result vector
		*/
		TAVector operator+(const TAVector& _other_);

		/**
		* @brief						- operator
		*								Returns the subtraction of this vector and _other_vector
		* @param	_other_				Vector of the type TAVector to be subtracted
		* @return						Result vector
		*/
		TAVector operator-(const TAVector& _other_);

		/**
		* @brief						* operator
		*								Returns the scaled version of this vector by a scale factor _scale_
		* @param	_scale_				Scale Factor
		* @return						Result vector
		*/
		TAVector operator*(const float& _scale_);

		/**
		* @brief						* operator
		*								Returns the cross product this X _other_
		* @param	_other_				Vector of the type TAVector in right side of the cross product
		* @return						Result vector
		*/
		TAVector operator*(const TAVector& _other_);

		/**
		* @brief						% operator
		*								Returns the dot product this o _other_
		* @param	_other_				Vector of the type TAVector in right side of the dot product
		* @return						Result dot product
		*/
		float operator%(const TAVector& _other_); //Dot Product

		/**
		* @brief						Returns the norm of the vector
		* @return						Result norm
		*/
		float norm() const;

		/**
		* @brief						Normalizes the vector if the norm is greater than 0
		*/
		void normalize();

		/**
		* @brief						Prints the vector to standard output in a good format
		*/
		void print() const;

		/**
		* @brief						Gets the angle between two vectors in radians
		* @param	_v1_				Vector 1 of the type TAVector
		* @param	_v2_				Vector 2 of the type TAVector
		* @return						Angle between two vectors
		*/
		static float getAngleBetweenTwoVectors(const TAVector& _v1_, const TAVector& _v2_);

		float x;
		float y;
		float z;
	};
}

#endif
