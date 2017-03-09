#ifndef TA_FEATURE_FILE_IO_H
#define TA_FEATURE_FILE_IO_H

#include <TAFeature.h>
#include <core/Defs.h>
#include <vector>
#include <fstream>

namespace TAFea
{
	class TAFeatureFileIO
	{
	public:
		static TACore::Result writePBSDInBinary(const std::vector<LocalFeaturePtr>& feas, const std::string& filePath);
		static TACore::Result readPBSDInBinary(const std::string& filePath, std::vector<LocalFeaturePtr>& feas);

	private:	
		template <class T>
		static TACore::Result writeBinary(const std::vector<T>& vec, std::ofstream& out);
		
		template <class T>
		static TACore::Result writeBinary(const std::vector<std::vector<T> >& vec, std::ofstream& out);

		template <class T>
		static TACore::Result readBinary(std::ifstream& inp, std::vector<T>& vec);

		template <class T>
		static TACore::Result readBinary(std::ifstream& inp, std::vector<std::vector<T> >& vec);
		
	};
}

#endif