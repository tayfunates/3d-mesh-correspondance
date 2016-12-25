#include "HKSDescExtraction.h"
#include "HeatKernelSignatureDesc.h"
#include <core/NDimVector.h>
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
		this->m_nNoEigenVal = 125;
		this->m_fTMin = -1.0; //If not set from the user, this will be calculated by the extractor
		this->m_fTMax = -1.0; //If not set from the user, this will be calculated by the extractor
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
		this->m_nNoEigenVal = 125;
		this->m_fTMin = -1.0; //If not set from the user, this will be calculated by the extractor
		this->m_fTMax = -1.0; //If not set from the user, this will be calculated by the extractor
		this->m_nNumberOfTimeSamples = 100;
		if (this->m_mLaplacian != NULL)
		{
			delete this->m_mLaplacian;
			this->m_mLaplacian = NULL;
		}
	}

	Result HKSDescExtraction::extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		//We, now, know that the mesh is a triangular mesh
		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		Result res;
		res = createLaplacianMatrix(mesh, this->m_LaplacianUsed);
		if (res != TACore::TACORE_OK)
		{
			return res;
		}

		arma::cx_vec eigval;
		arma::cx_mat eigvec;
		res = calcEigenDecomposition(this->m_nNoEigenVal, &eigval, &eigvec);

		if (res != TACore::TACORE_OK)
		{
			return res;
		}
		
		if (this->m_fTMin < 0 && this->m_fTMax < 0)
		{
			this->m_fTMin = (4.0 * log(10 / eigval(eigval.size() - 1).real()));
			this->m_fTMax = (4.0 * log(10 / eigval(1).real()));
		}

		const double tIncrement = (this->m_fTMax - this->m_fTMin) / (this->m_nNumberOfTimeSamples - 1);

		const size_t verSize = triMesh->verts.size();
		outFeatures = std::vector<LocalFeaturePtr>(verSize);

		for (size_t v = 0; v < verSize; v++)
		{
			//Initialize descriptor of the vertex
			HeatKernelSignatureDesc *pDesc = new HeatKernelSignatureDesc(v);
			pDesc->m_vDescriptor = std::vector<double>(this->m_nNumberOfTimeSamples);

			double currT = this->m_fTMin;
			for (size_t t = 0; t < this->m_nNumberOfTimeSamples; t++)
			{
				const double logScaleCurrT = pow(2.0, currT);
				double descVal = 0.0;
				for (size_t eig = 0; eig < this->m_nNoEigenVal; eig++)
				{
					const double sqEigFunc = eigvec(v, eig).real() * eigvec(v, eig).real();
					descVal += exp(eigval(eig).real() * logScaleCurrT * (-1.0)) * sqEigFunc;
				}
				pDesc->m_vDescriptor[t] = descVal;
				currT += tIncrement;
			}
			outFeatures[v] = LocalFeaturePtr(pDesc);
		}

		return TACORE_OK;
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

	void HKSDescExtraction::setMinTimeVal(const double& minTime)
	{
		this->m_fTMin = minTime;
	}

	double HKSDescExtraction::getMinTimeVal() const
	{
		return this->m_fTMin;
	}

	void HKSDescExtraction::setMaxTimeVal(const double& maxTime)
	{
		this->m_fTMax = maxTime;
	}

	double HKSDescExtraction::getMaxTimeVal() const
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

	Result HKSDescExtraction::calcEigenDecomposition(const unsigned int& noEigenVals, arma::cx_vec *pEigVals, arma::cx_mat *pEigVecs)
	{
		if (noEigenVals < 2)
		{
			std::cerr << "Number of eigen values and vectors must be greater than or equal to 2!" << std::endl;
			return TACore::TACORE_BAD_ARGS;
		}
		if (this->m_mLaplacian == NULL)
		{
			return TACore::TACORE_INVALID_OPERATION;
		}

		const bool ret = arma::eigs_gen(*pEigVals, *pEigVecs, *(this->m_mLaplacian), noEigenVals, "sm");
		if (ret == false)
		{
			std::cout << "eigs_gen is failed to converge to find the eigen values of the Laplacian matrix" << std::endl;
			return TACore::TACORE_ERROR;
		}

		return TACore::TACORE_OK;
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


		std::vector<unsigned int> vRowIndices;
		std::vector<unsigned int> vColIndices;
		std::vector<double> vValues;
		for (size_t i = 0; i < numberOfVertices; i++)
		{
			double sum_w = 0.0;
			for (int j = 0; j < (int) triMesh->verts[i]->vertList.size(); j++) 
			{
				double w_ij = cotangentWeight(triMesh, i, triMesh->verts[i]->vertList[j]);
				sum_w += w_ij;
			}
			for (int j = 0; j < (int) triMesh->verts[i]->vertList.size(); j++) 
			{
				double w_ij = cotangentWeight(triMesh, i, triMesh->verts[i]->vertList[j]);

				if (i != triMesh->verts[i]->vertList[j]) {
					double entry = double((-1.0)*w_ij / sum_w);
					vRowIndices.push_back(i);
					vColIndices.push_back(triMesh->verts[i]->vertList[j]);
					vValues.push_back(entry);
				}
			}
			vRowIndices.push_back(i);
			vColIndices.push_back(i);
			vValues.push_back(1.0);
		}


		arma::umat locations(2, vRowIndices.size());
		arma::vec values(vValues);
		for (size_t i = 0; i < vRowIndices.size(); i++)
		{
			locations(0, i) = vRowIndices[i];
			locations(1, i) = vColIndices[i];
		}
		this->m_mLaplacian = new arma::SpMat<double>(locations, values, true);

		return TACore::TACORE_OK;
	}


	double HKSDescExtraction::cotangentWeight(TriangularMesh* mesh, int iv, int jv)
	{
		int noTri = 0;
		double cots = 0.0;
		for (size_t i = 0; i < mesh->verts[iv]->triList.size(); i++) {
			Triangle* ivTri = mesh->tris[mesh->verts[iv]->triList[i]];
			int b, n1 = iv, n2 = jv;
			bool shareTriangle = false;

			if (ivTri->v1i == jv) {
				noTri++;
				shareTriangle = true;
				if (ivTri->v2i == iv) {
					b = ivTri->v3i;
				}
				else {
					b = ivTri->v2i;
				}
			}
			else if (ivTri->v2i == jv) {
				noTri++;
				shareTriangle = true;
				if (ivTri->v1i == iv) {
					b = ivTri->v3i;
				}
				else {
					b = ivTri->v1i;
				}
			}
			else if (ivTri->v3i == jv) {
				noTri++;
				shareTriangle = true;
				if (ivTri->v1i == iv) {
					b = ivTri->v2i;
				}
				else {
					b = ivTri->v1i;
				}
			}
			if (shareTriangle) {
				Vertex *vn1 = mesh->verts[n1];
				Vertex *vn2 = mesh->verts[n2];
				TACore::Vector3D base(mesh->verts[b]->coords[0], mesh->verts[b]->coords[1], mesh->verts[b]->coords[2]);
				TACore::Vector3D ne1(vn1->coords[0], vn1->coords[1], vn1->coords[2]);
				TACore::Vector3D ne2(vn2->coords[0], vn2->coords[1], vn2->coords[2]);

				//TO DO: Use get angle class of NDimVector
				TACore::Vector3D v1 = ne1 - base; v1.normalize();
				TACore::Vector3D v2 = ne2 - base; v2.normalize();

				double dot = (v1 % v2);
				double angle = acos(dot);

				cots += 1.0 / tan(angle);
			}
		}
		return cots / noTri;
	}
}