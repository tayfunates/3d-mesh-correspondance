#include "TAFeatureFileIO.h"
#include "PatchBasedShapeDistributionHistogram.h"
#include <iostream>

namespace TAFea
{
	TACore::Result TAFeatureFileIO::writePBSDInBinary(const std::vector<LocalFeaturePtr>& feas, const std::string& filePath)
	{
		std::ofstream out(filePath, std::ios::out | std::ios::binary);
		TACore::Result res = TACore::TACORE_OK;
		if (out.is_open())
		{
			const size_t feasSize = feas.size();
			out.write((char*)&(feasSize), sizeof(feasSize));

			for (size_t i = 0; i < feasSize; i++)
			{
				PatchBasedShapeDistributionHistogram* feaPtr = (PatchBasedShapeDistributionHistogram*)(feas[i]).get();
				res = writeBinary(feaPtr->m_vDescriptor, out);

				if (res != TACore::TACORE_OK)
				{
					break;
				}
			}
			out.close();
		}
		else
		{
			std::cerr << "Error: Specified File name: " << filePath << " cannot be opened!" << std::endl;
			res = TACore::TACORE_FILE_ERROR;
		}
		return res;
	}

	TACore::Result TAFeatureFileIO::readPBSDInBinary(const std::string& filePath, std::vector<LocalFeaturePtr>& feas)
	{
		std::ifstream inp(filePath, std::ios::in | std::ios::binary);
		TACore::Result res = TACore::TACORE_OK;
		if (inp.is_open())
		{
			feas.clear();
			size_t feasSize;
			inp.read((char*)(&feasSize), sizeof(feasSize));
			
			for (size_t i = 0; i < feasSize; i++)
			{	
				feas.push_back(LocalFeaturePtr(new PatchBasedShapeDistributionHistogram(i)));
				PatchBasedShapeDistributionHistogram* feaPtr = (PatchBasedShapeDistributionHistogram*)(feas[i]).get();
				res = readBinary(inp, feaPtr->m_vDescriptor);

				if (res != TACore::TACORE_OK)
				{
					break;
				}
			}
			inp.close();
		}
		else
		{
			std::cerr << "Error: Specified File name: " << filePath << " cannot be opened!" << std::endl;
			res = TACore::TACORE_FILE_ERROR;
		}
		return res;
	}

	template <class T>
	TACore::Result TAFeatureFileIO::writeBinary(const std::vector<std::vector<T> >& vec, std::ofstream& out)
	{
		TACore::Result res = TACore::TACORE_OK;
		const size_t vecSize = vec.size();
		out.write((char*)&(vecSize), sizeof(vecSize));

		for (size_t i = 0; i < vecSize; i++)
		{
			res = writeBinary(vec[i], out);

			if (res != TACore::TACORE_OK)
			{
				break;
			}
		}
		return res;
	}

	template <class T>
	TACore::Result TAFeatureFileIO::writeBinary(const std::vector<T>& vec, std::ofstream& out)
	{
		TACore::Result res = TACore::TACORE_OK;
		const size_t vecSize = vec.size();
		out.write((char*)&(vecSize), sizeof(vecSize));

		for (size_t i = 0; i < vecSize; i++)
		{
			out.write((char*)&(vec[i]), sizeof(T));
		}
		return res;
	}

	template <class T>
	TACore::Result TAFeatureFileIO::readBinary(std::ifstream& inp, std::vector<std::vector<T> >& vec)
	{
		TACore::Result res = TACore::TACORE_OK;
		size_t vecSize;
		inp.read((char*)(&vecSize), sizeof(vecSize));

		vec = std::vector<std::vector<T> >(vecSize, std::vector<T>());
		for (size_t i = 0; i < vecSize; i++)
		{
			res = readBinary(inp, vec[i]);

			if (res != TACore::TACORE_OK)
			{
				break;
			}
		}
		return res;
	}

	template <class T>
	TACore::Result TAFeatureFileIO::readBinary(std::ifstream& inp, std::vector<T>& vec)
	{
		TACore::Result res = TACore::TACORE_OK;
		size_t vecSize;
		inp.read((char*)(&vecSize), sizeof(vecSize));

		vec = std::vector<T>(vecSize);
		for (size_t i = 0; i < vecSize; i++)
		{
			inp.read((char*)&(vec[i]), sizeof(T));
		}
		return res;
	}
}