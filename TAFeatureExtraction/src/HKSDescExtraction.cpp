#include "HKSDescExtraction.h"
#include "HeatKernelSignatureDesc.h"
#include <core/NDimVector.h>
#include <core/TriangularMesh.h>
#include <set>
#include <numeric>

#ifdef ENABLE_TIMING_REPORTS
#include <core/Timer.h>
#endif

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
		this->m_fTMin = 1.0;
		this->m_fTMax = 1000000;
		this->m_nNumberOfTimeSamples = 100;
		this->m_bCalcTimeValsFromEigenVals = true;
		this->m_mLaplacian = NULL;
	}

	HKSDescExtraction::HKSDescExtraction(const HKSDescExtraction& other)
	{
		this->m_LaplacianUsed = other.m_LaplacianUsed;
		this->m_nNoEigenVal = other.m_nNoEigenVal;
		this->m_fTMin = other.m_fTMin;
		this->m_fTMax = other.m_fTMax;
		this->m_nNumberOfTimeSamples = other.m_nNumberOfTimeSamples;
		this->m_bCalcTimeValsFromEigenVals = other.m_bCalcTimeValsFromEigenVals;
		this->m_mLaplacian = other.m_mLaplacian; //Shallow copy
	}

	void HKSDescExtraction::clear()
	{
		this->m_LaplacianUsed = STAR_LAPLACIAN;
		this->m_nNoEigenVal = 125;
		this->m_fTMin = 1.0;
		this->m_fTMax = 1000000.0;
		this->m_nNumberOfTimeSamples = 100;
		this->m_bCalcTimeValsFromEigenVals = true;
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

		std::vector<double> ringAreas;
		triMesh->calcRingAreasOfVertices(ringAreas);

		Result res;
		res = createLaplacianMatrix(mesh, this->m_LaplacianUsed, ringAreas);
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
#ifdef ENABLE_TIMING_REPORTS
		TACore::Timer timer;
#endif
		double minT = this->m_fTMin;
		double maxT = this->m_fTMax;
		if (this->m_bCalcTimeValsFromEigenVals)
		{
			minT = (abs(4 * log(10) / eigval(eigval.size() - 1).real()));
			maxT = (abs(4 * log(10) / eigval(1).real()));
		}

		const double logTMin = log(minT);
		const double logTMax = log(maxT);

		const double tIncrement = (logTMax - logTMin) / (this->m_nNumberOfTimeSamples - 1);

		double sumRingAreas = std::accumulate(ringAreas.begin(), ringAreas.end(), 0.0);
		double sqrtSumRingAreas = sqrt(sumRingAreas);

		const size_t verSize = triMesh->verts.size();
		outFeatures = std::vector<LocalFeaturePtr>(verSize);

		for (size_t v = 0; v < verSize; v++)
		{
			if (v < verSize - 1)
			{
				std::cout << "%" << (100 * v) / verSize << " completed for calculating hks descriptor" << "\r";
			}
			else
			{
				std::cout << "%" <<100 << " completed for calculating hks descriptor" << "\n";
			}

			//Initialize descriptor of the vertex
			HeatKernelSignatureDesc *pDesc = new HeatKernelSignatureDesc(v);
			pDesc->m_vDescriptor = std::vector<double>(this->m_nNumberOfTimeSamples);

			double currT = logTMin;
			for (size_t t = 0; t < this->m_nNumberOfTimeSamples; t++)
			{
				const double logScaleCurrT = exp(currT);
				double heatTrace = 0.0;
				double descVal = 0.0;
				for (size_t eig = 0; eig < this->m_nNoEigenVal; eig++)
				{
					const double expInvEigVal = exp(sumRingAreas * abs(eigval(eig).real()) * logScaleCurrT * (-1.0));
					const double EigFuncVal = sqrtSumRingAreas * eigvec(v, eig).real();
					const double sqEigFuncVal = EigFuncVal * EigFuncVal;
					descVal += (expInvEigVal * sqEigFuncVal);
					heatTrace += expInvEigVal;
				}
				pDesc->m_vDescriptor[t] = descVal / heatTrace;
				currT += tIncrement;
			}
			outFeatures[v] = LocalFeaturePtr(pDesc);
		}
#ifdef ENABLE_TIMING_REPORTS
		std::cout << "Time Passed to Calculate HKS from Eigenvalues and Eigenvectors: " << timer.seconds() << " secs." << std::endl;
#endif

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

	void HKSDescExtraction::setUseEigenValuesForTimeBoundaries(const bool& calcTimeFromEigenVals)
	{
		this->m_bCalcTimeValsFromEigenVals = calcTimeFromEigenVals;
	}

	bool HKSDescExtraction::getUseEigenValuesForTimeBoundaries() const
	{
		return this->m_bCalcTimeValsFromEigenVals;
	}

	Result HKSDescExtraction::createLaplacianMatrix(PolygonMesh *mesh, const TypeOfLaplacian& typeLap, const std::vector<double>& vertexRingAreas)
	{
#ifdef ENABLE_TIMING_REPORTS
		TACore::Timer timer;
#endif
		Result res = TACore::TACORE_OK;
		if (typeLap == HKSDescExtraction::STAR_LAPLACIAN)
		{
			res = createStarLaplacianMatrix(mesh, vertexRingAreas);
		}
		if (typeLap == HKSDescExtraction::DISCRETE_LAPLACIAN)
		{
			res = createDiscreteLaplacianMatrix(mesh, vertexRingAreas);
		}
#ifdef ENABLE_TIMING_REPORTS
		std::cout << "Time Passed to Create Laplacian: " << timer.seconds() << " secs." << std::endl;
#endif
		return res;
	}

	Result HKSDescExtraction::calcEigenDecomposition(const unsigned int& noEigenVals, arma::cx_vec *pEigVals, arma::cx_mat *pEigVecs)
	{
#ifdef ENABLE_TIMING_REPORTS
		TACore::Timer timer;
#endif
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
#ifdef ENABLE_TIMING_REPORTS
		std::cout << "Time Passed to Calculate the Eigenvalues of the Laplacian: " << timer.seconds() << " secs." << std::endl;
#endif
		return TACore::TACORE_OK;
	}

	Result HKSDescExtraction::createStarLaplacianMatrix(PolygonMesh *mesh, const std::vector<double>& vertexRingAreas)
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
			if (i < numberOfVertices-1)
			{
				std::cout << "%" << (100 * i) / numberOfVertices << " completed for creating laplacian" << "\r";
			}
			else
			{
				std::cout << "%" << 100 << " completed for creating laplacian" << "\n";
			}
			
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
					double entry = (double((-1.0)*w_ij / sum_w));
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

		//Normalize the laplacian
		double sumRingAreas = std::accumulate(vertexRingAreas.begin(), vertexRingAreas.end(), 0.0);
		*(this->m_mLaplacian) = *(this->m_mLaplacian) / sumRingAreas;

		return TACore::TACORE_OK;
	}

	Result HKSDescExtraction::createDiscreteLaplacianMatrix(PolygonMesh *mesh, const std::vector<double>& vertexRingAreas)
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
		const double FOUR_PI = 4.0 * 3.14159265358979323846;

		std::vector<unsigned int> vRowIndices;
		std::vector<unsigned int> vColIndices;
		std::vector<double> vValues;

		const double epsilon = 1e-5;

		for (size_t i = 0; i < numberOfVertices; i++)
		{
			if (i < numberOfVertices - 1)
			{
				std::cout << "%" << (100 * i) / numberOfVertices << " completed for creating laplacian" << "\r";
			}
			else
			{
				std::cout << "%" << 100 << " completed for creating laplacian" << "\n";
			}

			Vertex *vi = triMesh->verts[i];

			const std::vector<int>& vertexTriList = vi->triList;
			std::set<int> neighborEdgeIndicesList;
			for (size_t t = 0; t < vertexTriList.size(); t++)
			{
				neighborEdgeIndicesList.insert(triMesh->tris[vertexTriList[t]]->e1);
			}
			double neighborEdgeLengthSum = 0.0;
			for (std::set<int>::iterator it = neighborEdgeIndicesList.begin(); it != neighborEdgeIndicesList.end(); ++it) {
				neighborEdgeLengthSum += triMesh->edges[*it]->length;
			}
			const double h = neighborEdgeLengthSum / neighborEdgeIndicesList.size(); //Adaptive h according to the edge lengths around the vertex
			const double hh = h * h;
			const double pi = 3.1415926535897;
			const double fourOverPIhhhh = 4.0 / (pi * hh * hh);

			const double epsilon = 1e-10;

			double diagonalEntry = 0.0;
			for (size_t j = 0; j < numberOfVertices; j++)
			{
				if (i != j)
				{
					Vertex *vj = triMesh->verts[j];
					const float eucDistanceBetweenTwo = triMesh->eucDistanceBetween(vi, vj);
					const double entry = (vertexRingAreas[j] / 3.0) * exp((-1.0 * eucDistanceBetweenTwo * eucDistanceBetweenTwo) / hh) * fourOverPIhhhh;

					if (entry > epsilon)
					{
						vRowIndices.push_back(i);
						vColIndices.push_back(j);
						vValues.push_back(entry);
						diagonalEntry += (-1.0) * entry;
					}
				}
				
			}

			vRowIndices.push_back(i);
			vColIndices.push_back(i);
			vValues.push_back(diagonalEntry);
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