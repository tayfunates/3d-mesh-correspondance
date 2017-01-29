#include "core/TAMatrix.h"
#include <cstring>

namespace TACore
{
	template <class T>
	TAMatrix<T>::~TAMatrix()
	{
		std::cout << "HEDE" << std::endl;
		clear();
	}

	template <class T>
	TAMatrix<T>::TAMatrix()
	{
		m_ppMatrix = NULL;
		m_nCols = 0;
		m_nRows = 0;
	}

	template <class T>
	void TAMatrix<T>::init(int rows, int cols)
	{
		m_ppMatrix = new T*[rows];
		for (int r = 0; r < rows; r++)
		{
			m_ppMatrix[r] = new T[cols];
		}
		m_nCols = cols;
		m_nRows = rows;
	}

	template <class T>
	void TAMatrix<T>::clear()
	{
		if (m_ppMatrix != NULL)
		{
			for (int r = 0; r < m_nRows; r++)
			{
				if (m_ppMatrix[r] != NULL)
				{
					delete[] m_ppMatrix[r];
					m_ppMatrix[r] = NULL;
				}
			}
			delete[] m_ppMatrix;
			m_ppMatrix = NULL;
		}
		m_nCols = 0;
		m_nRows = 0;
	}

	template <class T>
	T* TAMatrix<T>::getRow(int row) const
	{
		return m_ppMatrix[row];
	}

	template <class T>
	void TAMatrix<T>::setRow(T* rowData, int row)
	{
		std::memcpy(m_ppMatrix[row], rowData, m_nCols * sizeof(T));
	}

	template <class T>
	T TAMatrix<T>::getVal(int row, int col) const
	{
		return m_ppMatrix[row][col];
	}

	template <class T>
	void TAMatrix<T>::setVal(int row, int col, T val)
	{
		m_ppMatrix[row][col] = val;
	}

	template <class T>
	void TAMatrix<T>::print(std::ostream& o) const
	{
		o << "( " << std::endl;
		for (int r = 0; r< m_nRows; r++)
		{
			o << "( ";
			for (int c = 0; c < m_nCols; c++)
			{
				o << m_ppMatrix[r][c] << " ";
			}
			o << " )";
			o << std::endl;
		}
		o << " )" << std::endl;
	}

	template <class T>
	int TAMatrix<T>::rows() const
	{
		return m_nRows;
	}

	template <class T>
	int TAMatrix<T>::cols() const
	{
		return m_nCols;
	}

	template class TAMatrix<float>;
}