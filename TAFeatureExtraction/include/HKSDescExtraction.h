#ifndef HEAT_KERNEL_SIGNATURE_DESC_EXTRACTION_H
#define HEAT_KERNEL_SIGNATURE_DESC_EXTRACTION_H

#include "TAFeatureExtraction.h"
#include <armadillo>

namespace TAShape
{
	class TriangularMesh;
}

namespace TAFeaExt
{
	//Heat Kernel Signature for a single vertex in a mesh
	//Technical Details of the feature is described in http://www.lix.polytechnique.fr/~maks/papers/hks.pdf
	//[Currently] Time interval is decided automatically on default from the eigen values as proposed in the paper.
	//Set time interval values in case you do not want the default calculated ones
	//HKSDescExtraction is a PerVertexFeatureExtraction
	class HKSDescExtraction : public PerVertexFeatureExtraction
	{
	public:

		enum TypeOfLaplacian
		{
			STAR_LAPLACIAN = 1 //< http://www.cs.princeton.edu/courses/archive/fall10/cos526/papers/sorkine05.pdf
		};

		virtual ~HKSDescExtraction();
		HKSDescExtraction();
		HKSDescExtraction(const HKSDescExtraction& other);

		virtual Result extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures);
		virtual Result extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr);

		/**
		* @brief	Create the laplacian matrix from the polygon mesh according to TypeOfLaplacian and saves it into the object
		*			[Currently] Only STAR Laplacian is supported
		* @param	mesh Polygonial mesh from which the laplacian matrix will be created
		* @param	typeLap Type of the laplacian matrix to be extracted
		* @return	TACORE_OK if everything goes fine.
		*/
		Result createLaplacianMatrix(PolygonMesh *mesh, const TypeOfLaplacian& typeLap);

		/**
		* @brief	Calculates the eigen values and vectors of the laplacian saved in the object.
		* @param	noEigenVals Number of eigen values and vectors to be calculated
		* @param [out] pEigVals Eigenvalues calculated
		* @param [out] pEigVecs Eigenvectors calculated
		* @return	TACORE_OK if everything goes fine
		* @return	TACORE_BAD_ARGS if noEigenVals is smaller than 2
		* @return	TACORE_INVALID_OPERATION if laplacian matrix is not constructed
		* @return	TACORE_ERROR if decomposition fails to converge
		*/
		Result calcEigenDecomposition(const unsigned int& noEigenVals, arma::cx_vec *pEigVals, arma::cx_mat *pEigVecs);

		/**
		* @brief	Clears the object by setting default parameters and deleting heap space
		* @return	void
		*/
		void clear();

		//Setters
		void setTypeOfLaplacian(const TypeOfLaplacian& typeLap);
		void setNumberOfEigenVals(const unsigned int& noEigenVals);
		void setMinTimeVal(const double& minTime);
		void setMaxTimeVal(const double& maxTime);
		void setNumberOfTimeSamples(const unsigned int& noTimeSamples);

		//Getters
		TypeOfLaplacian getTypeOfLaplacian() const;
		unsigned int getNumberOfEigenVals() const;
		double getMinTimeVal() const;
		double getMaxTimeVal() const;
		unsigned int getNumberOfTimeSamples() const;

	private:

		TypeOfLaplacian m_LaplacianUsed;		//< Type of the laplacian used in the extraction
		unsigned int m_nNoEigenVal;				//< Number of eigen vals and vects extracted from the laplacian matrix
		double m_fTMin;							//< Minimum value of the time interval
		double m_fTMax;							//< Maximum value of the time interval
		unsigned int m_nNumberOfTimeSamples;	//< Number of time values in the interval

		arma::SpMat<double> *m_mLaplacian;		//< Laplacian matrix, it is a class member because multiple executions for different time intervals should be enabled without re-construction.

		/**
		* @brief	Create the STAR laplacian matrix from the polygon mesh saves it into the object
		*			[Currently] Only Triangular mesh is supported
		* @param	mesh Polygonial mesh from which the laplacian matrix will be created
		* @return	TACORE_OK if everything goes fine.
		*/
		Result createStarLaplacianMatrix(PolygonMesh *mesh);

		/**
		* @brief	Computes the cotangent weigh for w_ij for STAR Laplacian Matrixss
		* @param	iv The index of the source vertex
		* @param	jv The index of the neighbor vertex
		* @return	Cotangent weight between the vertices iv and jn
		*/
		double cotangentWeight(TriangularMesh* mesh, int iv, int jv);

	};
}

#endif
