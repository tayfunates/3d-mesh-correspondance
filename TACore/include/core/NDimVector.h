#ifndef TACORE_N_DIM_VECTOR_H
#define TACORE_N_DIM_VECTOR_H

namespace TACore
{
	template <class T, int N>
	class NDimVector
	{
	public:
		T m_Vec[N];
		int m_NoDims;

		NDimVector()
		{
			m_NoDims = N;
			for (int i = 0; i<m_NoDims; i++)
			{
				m_Vec[i] = 0;
			}
		}

		NDimVector(const T& v1)
		{
			m_NoDims = 1;
			m_Vec[0] = v1;
		}

		NDimVector(const T& v1, const T& v2)
		{
			m_NoDims = 2;
			m_Vec[0] = v1;
			m_Vec[1] = v2;
		}

		NDimVector(const T& v1, const T& v2, const T& v3)
		{
			m_NoDims = 3;
			m_Vec[0] = v1;
			m_Vec[1] = v2;
			m_Vec[2] = v3;
		}

		//Constructor initializing object with an array of type T
		NDimVector(T arr[])
		{
			m_NoDims = N;
			for (int i = 0; i<m_NoDims; i++)
			{
				m_Vec[i] = arr[i];
			}
		}
		~NDimVector() {}

		//Copy constructor
		NDimVector(const NDimVector<T, N>& other)
		{
			m_NoDims = N;
			for (int i = 0; i<m_NoDims; i++)
			{
				m_Vec[i] = other.m_Vec[i];
			}
		}

		//Constructor of the vector between two points p1 and p1. i.e. p1 -------> p2
		NDimVector(const NDimVector<T, N>& p1, const NDimVector<T, N>& p2)
		{
			m_NoDims = N;
			for (int i = 0; i<m_NoDims; i++)
			{
				m_Vec[i] = p2.m_Vec[i] - p1.m_Vec[i];
			}
		}

		//Assignment operator
		NDimVector& operator=(const NDimVector<T, N>& other)
		{
			if (&other == this) return *this;
			for (int i = 0; i<m_NoDims; i++)
			{
				m_Vec[i] = other.m_Vec[i];
			}

			return *this;
		}

		//Overloading <<operator
		friend std::ostream& operator<<(std::ostream& o, const NDimVector& vec)
		{
			o << "( ";
			for (int i = 0; i<vec.m_NoDims; i++)
			{
				o << vec.m_Vec[i];
				if (i != vec.m_NoDims - 1)
					o << ", ";
			}
			o << " )";
			return o;
		}

		//Adds two vector
		NDimVector<T, N> operator+(const NDimVector<T, N>& other) const
		{
			NDimVector ret;
			for (int i = 0; i<m_NoDims; i++)
			{
				ret.m_Vec[i] = m_Vec[i] + other.m_Vec[i];
			}
			return ret;
		}
		//Subtract a vector other from this vector
		NDimVector<T, N> operator-(const NDimVector<T, N>& other) const
		{
			NDimVector ret;
			for (int i = 0; i<m_NoDims; i++)
			{
				ret.m_Vec[i] = m_Vec[i] - other.m_Vec[i];
			}
			return ret;

		}
		//Returns the scaled version of this vector
		NDimVector<T, N> operator*(const double& scale) const
		{
			NDimVector ret;
			for (int i = 0; i<m_NoDims; i++)
			{
				ret.m_Vec[i] = m_Vec[i] * scale;
			}
			return ret;
		}

		//Returns the norm of the vector
		double norm() const
		{
			double sum = 0.0;
			for (int i = 0; i<m_NoDims; i++)
			{
				sum += m_Vec[i] * m_Vec[i];
			}
			return sqrt(sum);
		}


		//Returns the square of the norm of the vector
		double sqNorm() const
		{
			double sum = 0.0;
			for (int i = 0; i<m_NoDims; i++)
			{
				sum += m_Vec[i] * m_Vec[i];
			}
			return sum;
		}

		//Normalizes the vector if its norm is greater than 0
		bool normalize()
		{
			const double nrm = norm();
			if (nrm <= 0.0) return false;

			for (int i = 0; i<m_NoDims; i++)
			{
				m_Vec[i] = m_Vec[i] / nrm;
			}
			return true;
		}

		//Returns the normalized version of this vector
		NDimVector<T, N> normalize(bool& res) const
		{
			NDimVector ret;
			const double nrm = norm();
			if (nrm <= 0.0)
			{
				res = false;
				return ret;
			}
			res = true;
			for (int i = 0; i<m_NoDims; i++)
			{
				ret.m_Vec[i] = m_Vec[i] / nrm;
			}
			return ret;
		}

		//Takes the dot product between two vectors
		double operator%(const NDimVector<T, N>& other) const
		{
			double sum = 0.0;
			for (int i = 0; i<m_NoDims; i++)
			{
				sum += m_Vec[i] * other.m_Vec[i];
			}
			return sum;
		}

		//Gets the angle between two vector in radians
		static double getAngleBetweenTwo(const NDimVector<T, N>& v1, const NDimVector<T, N>& v2)
		{
			NDimVector temp1(v1); temp1.normalize();
			NDimVector temp2(v2); temp2.normalize();
			return acos(temp1 % temp2);
		}

		//Takes cross product if the dimensions of both vectors are 3
		NDimVector<T, N> operator*(const NDimVector<T, N>& other) const
		{
			NDimVector ret;
			if (m_NoDims != 3) return ret;

			ret.m_Vec[0] = m_Vec[1] * other.m_Vec[2] - m_Vec[2] * other.m_Vec[1];
			ret.m_Vec[1] = m_Vec[2] * other.m_Vec[0] - m_Vec[0] * other.m_Vec[2];
			ret.m_Vec[2] = m_Vec[0] * other.m_Vec[1] - m_Vec[1] * other.m_Vec[0];

			return ret;
		}

		//Gets the euclidaen distance between two vectors
		static double getEucDistBetweenTwo(const NDimVector<T, N>& v1, const NDimVector<T, N>& v2)
		{
			double sum = 0.0;
			for (int i = 0; i<v1.m_NoDims; i++)
			{
				double dist1D = v1.m_Vec[i] - v2.m_Vec[i];
				dist1D *= dist1D;
				sum += dist1D;
			}
			return sqrt(sum);
		}

		//Gets the scalar projection of a vector onto another. i.e this onto other
		double getScalarProjectionOntoVector(const NDimVector<T, N>& other)
		{
			const double nrm = other.norm();
			if (nrm <= 0) return -1.0;
			return ((*this) % other) / nrm;
		}

		//Gets the vector projection of a vector onto another. i.e this onto other
		NDimVector<T, N> getVectorProjectionOntoVector(const NDimVector<T, N>& other, bool& res)
		{
			NDimVector ret;
			double sqNrm = other.sqNorm();
			if (sqNrm <= 0.0)
			{
				res = false;
				return ret;
			}
			res = true;
			return other * (((*this) % other) / sqNrm);
		}
	};

	typedef NDimVector<int, 1> Vector1I;
	typedef NDimVector<int, 2> Vector2I;
	typedef NDimVector<int, 3> Vector3I;

	typedef NDimVector<float, 1> Vector1F;
	typedef NDimVector<float, 2> Vector2F;
	typedef NDimVector<float, 3> Vector3F;

	typedef NDimVector<double, 1> Vector1D;
	typedef NDimVector<double, 2> Vector2D;
	typedef NDimVector<double, 3> Vector3D;
}

#endif