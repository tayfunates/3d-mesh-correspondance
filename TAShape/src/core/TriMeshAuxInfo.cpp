#include <core/TriMeshAuxInfo.h>
#include <core/StringUtil.h>
#include <fstream>
#include <iostream>
#include <iomanip>

namespace TAShape
{
	TriMeshAuxInfo::TriMeshAuxInfo()
	{
		m_lfMaxEucDistanceBetweenTwoVertices = 0.0;
		m_lfMaxAreaOfTriangleConstructedByThreeVertices = 0.0;
		m_lfMaxVolumeOfTetrahedronConstructedByForVertices = 0.0;
	}

	TriMeshAuxInfo::~TriMeshAuxInfo()
	{

	}

	TACore::Result TriMeshAuxInfo::save(const std::string& pathToAuxInfoFile) const
	{
		TACore::Result res = TACore::TACORE_OK;
		std::ofstream out(pathToAuxInfoFile, std::ios::out);

		if (out.is_open())
		{
			out << std::setprecision(16) << "MaxPossibleEucDistanceBetweenTwoVertices" << m_FileDelimeter << m_lfMaxEucDistanceBetweenTwoVertices << std::endl;
			out << std::setprecision(16) << "MaxPossibleAreaConstructedByThreeVertices" << m_FileDelimeter << m_lfMaxAreaOfTriangleConstructedByThreeVertices << std::endl;
			out << std::setprecision(16) << "MaxPossibleVolumeConstructedByFourVertices" << m_FileDelimeter << m_lfMaxVolumeOfTetrahedronConstructedByForVertices << std::endl;

			out.close();
		}
		else
		{
			res = TACore::TACORE_FILE_ERROR;
		}
		return res;
	}

	TACore::Result TriMeshAuxInfo::load(const std::string& pathToAuxInfoFile)
	{
		TACore::Result res = TACore::TACORE_OK;
		std::ifstream inp(pathToAuxInfoFile, std::ios::in);

		if (inp.is_open())
		{
			std::string line;
			inp >> line;

			std::vector<std::string> split;

			TACore::StringUtil::split(line, m_FileDelimeter, split);
			inp >> line;
			TACore::StringUtil::split(line, m_FileDelimeter, split);
			inp >> line;
			TACore::StringUtil::split(line, m_FileDelimeter, split);

			m_lfMaxEucDistanceBetweenTwoVertices = TACore::StringUtil::strToVal<double>(split[1]);
			m_lfMaxAreaOfTriangleConstructedByThreeVertices = TACore::StringUtil::strToVal<double>(split[3]);
			m_lfMaxVolumeOfTetrahedronConstructedByForVertices = TACore::StringUtil::strToVal<double>(split[5]);

			inp.close();
		}
		else
		{
			res = TACore::TACORE_FILE_ERROR;
		}
		return res;
	}

	void TriMeshAuxInfo::print() const
	{
		std::cout << std::setprecision(16) << "MaxPossibleEucDistanceBetweenTwoVertices" << m_FileDelimeter << m_lfMaxEucDistanceBetweenTwoVertices << std::endl;
		std::cout << std::setprecision(16) << "MaxPossibleAreaConstructedByThreeVertices" << m_FileDelimeter << m_lfMaxAreaOfTriangleConstructedByThreeVertices << std::endl;
		std::cout << std::setprecision(16) << "MaxPossibleVolumeConstructedByFourVertices" << m_FileDelimeter << m_lfMaxVolumeOfTetrahedronConstructedByForVertices << std::endl;
	}
}