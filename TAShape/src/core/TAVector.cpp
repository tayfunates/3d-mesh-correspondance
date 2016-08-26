#include <core/TAVector.h>
#include <cmath>

namespace TAShape
{
	TAVector::TAVector()
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}

	TAVector::TAVector(const float& _x_, const float& _y_, const float& _z_)
	{
		this->x = _x_;
		this->y = _y_;
		this->z = _z_;
	}

	TAVector::TAVector(const TAVector& _other_)
	{
		this->x = _other_.x;
		this->y = _other_.y;
		this->z = _other_.z;
	}

	TAVector::TAVector(const TAVector& _p1_, const TAVector& _p2_)
	{
		this->x = _p2_.x - _p1_.x;
		this->y = _p2_.y - _p1_.y;
		this->z = _p2_.z - _p1_.z;
	}

	TAVector TAVector::operator+(const TAVector& _other_)
	{
		return TAVector(this->x + _other_.x,
			this->y + _other_.y,
			this->z + _other_.z);
	}

	TAVector TAVector::operator-(const TAVector& _other_)
	{
		return TAVector(this->x - _other_.x,
			this->y - _other_.y,
			this->z - _other_.z);
	}

	TAVector TAVector::operator*(const float& _scale_)
	{
		return TAVector(this->x * _scale_,
			this->y * _scale_,
			this->z * _scale_);
	}

	TAVector TAVector::operator*(const TAVector& _other_)
	{

		return TAVector(this->y*_other_.z - this->z*_other_.y,
			this->z*_other_.x - this->x*_other_.z,
			this->x*_other_.y - this->y*_other_.x);
	}

	float TAVector::operator%(const TAVector& _other_)
	{
		return this->x * _other_.x + this->y * _other_.y + this->z * _other_.z;
	}

	void TAVector::print() const
	{
		std::cout << "X: " << this->x << " Y: " << this->y << " Z: " << this->z << std::endl;
	}

	float TAVector::norm() const
	{
		return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
	}

	void TAVector::normalize()
	{
		const float norm = this->norm();
		if (norm <= 0.0f) return;

		this->x = this->x / norm;
		this->y = this->y / norm;
		this->z = this->z / norm;
	}

	float TAVector::getAngleBetweenTwoVectors(const TAVector& _v1_, const TAVector& _v2_)
	{
		TAVector temp1(_v1_); temp1.normalize();
		TAVector temp2(_v2_); temp2.normalize();
		return acos(temp1 % temp2);
	}
}

