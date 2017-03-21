#ifndef TACORE_TAMATRIX_H
#define TACORE_TAMATRIX_H

#include <iostream>

namespace TACore
{
	template <class T>
	class TAMatrix
	{
	public:

		virtual ~TAMatrix();
		TAMatrix();

		void init(int rows, int cols);
		void clear();

		T* getRow(int row) const;
		void setRow(T* rowData, int row);

		T getVal(int row, int col) const;
		void setVal(int row, int col, T val);

		int rows() const;
		int cols() const;

		void print(std::ostream& o) const;

		bool saveBinary(const std::string& filePath);
		bool loadBinary(const std::string& filePath);

	private:
		T*		m_ppMatrix;
		int		m_nRows;
		int		m_nCols;
	};
}

#endif