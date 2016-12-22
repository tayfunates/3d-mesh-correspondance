#include "HKSDescExtraction.h"
#include <core/TriangularMesh.h>

namespace TAFeaExt
{
	HKSDescExtraction::~HKSDescExtraction()
	{
		clear();
	}

	HKSDescExtraction::HKSDescExtraction()
	{
		this->m_LaplacianUsed = STAR_LAPLACIAN;
		this->m_nNoEigenVal = 300;
		this->m_fTMin = -1.0f; //If not set from the user, this will be calculated by the extractor
		this->m_fTMax = -1.0f; //If not set from the user, this will be calculated by the extractor
		this->m_nNumberOfTimeSamples = 100;
		this->m_mLaplacian = NULL;
	}

	HKSDescExtraction::HKSDescExtraction(const HKSDescExtraction& other)
	{
		this->m_LaplacianUsed = other.m_LaplacianUsed;
		this->m_nNoEigenVal = other.m_nNoEigenVal;
		this->m_fTMin = other.m_fTMin;
		this->m_fTMax = other.m_fTMax;
		this->m_nNumberOfTimeSamples = other.m_nNumberOfTimeSamples;
		this->m_mLaplacian = other.m_mLaplacian; //Shallow copy
	}

	void HKSDescExtraction::clear()
	{
		this->m_LaplacianUsed = STAR_LAPLACIAN;
		this->m_nNoEigenVal = 300;
		this->m_fTMin = -1.0f; //If not set from the user, this will be calculated by the extractor
		this->m_fTMax = -1.0f; //If not set from the user, this will be calculated by the extractor
		this->m_nNumberOfTimeSamples = 100;
		if (this->m_mLaplacian != NULL)
		{
			delete this->m_mLaplacian;
			this->m_mLaplacian = NULL;
		}
	}

	Result HKSDescExtraction::extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures)
	{
		//Test the input mesh type.
		//Currently, it has to be a triangular mesh
		return TACore::TACORE_OK;
	}

	Result HKSDescExtraction::extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		//We, now, know that the mesh is a triangular mesh
		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		return TACORE_OK;
	}

	void HKSDescExtraction::setTypeOfLaplacian(const HKSDescExtraction::TypeOfLaplacian& typeLap)
	{
		this->m_LaplacianUsed = typeLap;
	}

	HKSDescExtraction::TypeOfLaplacian HKSDescExtraction::getTypeOfLaplacian() const
	{
		return this->m_LaplacianUsed;
	}

	void HKSDescExtraction::setNumberOfEigenVals(const unsigned int& noEigenVals)
	{
		this->m_nNoEigenVal = noEigenVals;
	}

	unsigned int HKSDescExtraction::getNumberOfEigenVals() const
	{
		return this->m_nNoEigenVal;
	}

	void HKSDescExtraction::setMinTimeVal(const float& minTime)
	{
		this->m_fTMin = minTime;
	}

	float HKSDescExtraction::getMinTimeVal() const
	{
		return this->m_fTMin;
	}

	void HKSDescExtraction::setMaxTimeVal(const float& maxTime)
	{
		this->m_fTMax = maxTime;
	}

	float HKSDescExtraction::getMaxTimeVal() const
	{
		return this->m_fTMax;
	}

	void HKSDescExtraction::setNumberOfTimeSamples(const unsigned int& noTimeSamples)
	{
		this->m_nNumberOfTimeSamples = noTimeSamples;
	}

	unsigned int HKSDescExtraction::getNumberOfTimeSamples() const
	{
		return this->m_nNumberOfTimeSamples;
	}

	Result HKSDescExtraction::createLaplacianMatrix(PolygonMesh *mesh, const TypeOfLaplacian& typeLap)
	{
		Result res = TACore::TACORE_OK;
		if (typeLap == HKSDescExtraction::STAR_LAPLACIAN)
		{
			res = createStarLaplacianMatrix(mesh);
		}
		return res;
	}

	Result HKSDescExtraction::createStarLaplacianMatrix(PolygonMesh *mesh)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		//We, now, know that the mesh is a triangular mesh
		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		if (this->m_mLaplacian != NULL)
		{
			delete this->m_mLaplacian;
		}

		const size_t numberOfVertices = triMesh->verts.size();
		this->m_mLaplacian = new Eigen::SparseMatrix<float>(numberOfVertices, numberOfVertices);
	}
}