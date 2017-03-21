#include "core/TAMatrix.h"
#include <cstring>
#include <fstream>

namespace TACore
{
	template <class T>
	TAMatrix<T>::~TAMatrix()
	{
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
		m_ppMatrix = new T [rows*cols];
		m_nCols = cols;
		m_nRows = rows;
	}

	template <class T>
	void TAMatrix<T>::clear()
	{
		if (m_ppMatrix != NULL)
		{
			delete[] m_ppMatrix;
			m_ppMatrix = NULL;
		}
		m_nCols = 0;
		m_nRows = 0;
	}

	template <class T>
	T* TAMatrix<T>::getRow(int row) const
	{
		return m_ppMatrix + (row * m_nCols);
	}

	template <class T>
	void TAMatrix<T>::setRow(T* rowData, int row)
	{
		std::memcpy(getRow(row), rowData, m_nCols * sizeof(T));
	}

	template <class T>
	T TAMatrix<T>::getVal(int row, int col) const
	{
		return m_ppMatrix[row * m_nCols + col];
	}

	template <class T>
	void TAMatrix<T>::setVal(int row, int col, T val)
	{
		m_ppMatrix[row * m_nCols + col] = val;
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
				o << m_ppMatrix[r * m_nCols + c] << " ";
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

	template <class T>
	bool TAMatrix<T>::saveBinary(const std::string& filePath)
	{
		bool res = true;
		std::ofstream out(filePath, std::ios::out | std::ios::binary);
		if (out.is_open())
		{
			out.write((char*)&(m_nRows), sizeof(m_nRows));
			out.write((char*)&(m_nCols), sizeof(m_nCols));

			for (int i = 0; i < m_nRows; i++)
			{
				for (int j = 0; j < m_nCols; j++)
				{
					out.write((char*)&(m_ppMatrix[i * m_nCols + j]), sizeof(T));
				}
			}

			out.close();
		}
		else
		{
			res = false;
		}
		return res;
	}

	template <class T>
	bool TAMatrix<T>::loadBinary(const std::string& filePath)
	{
		bool res = true;
		std::ifstream inp(filePath, std::ios::in | std::ios::binary);
		if (inp.is_open())
		{
			clear();
			int rows, cols;
			inp.read((char*)(&rows), sizeof(rows));
			inp.read((char*)(&cols), sizeof(cols));

			init(rows, cols);

			for (int i = 0; i < m_nRows; i++)
			{
				for (int j = 0; j < m_nCols; j++)
				{
					inp.read((char*)&(m_ppMatrix[i * m_nCols + j]), sizeof(T));
				}
			}

			inp.close();
		}
		else
		{
			res = false;
		}
		return res;
	}

	template class TAMatrix<float>;
}