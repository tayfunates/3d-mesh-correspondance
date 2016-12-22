#ifndef HEAT_KERNEÞ_SIGNATURE_DESC_EXTRACTION_H
#define HEAT_KERNEÞ_SIGNATURE_DESC_EXTRACTION_H

#include "TAFeatureExtraction.h"
#include "Eigen/SparseCore"

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
		* @brief	Clears the object by setting default parameters and deleting heap space
		* @return	void
		*/
		void clear();

		//Setters
		void setTypeOfLaplacian(const TypeOfLaplacian& typeLap);
		void setNumberOfEigenVals(const unsigned int& noEigenVals);
		void setMinTimeVal(const float& minTime);
		void setMaxTimeVal(const float& maxTime);
		void setNumberOfTimeSamples(const unsigned int& noTimeSamples);

		//Getters
		TypeOfLaplacian getTypeOfLaplacian() const;
		unsigned int getNumberOfEigenVals() const;
		float getMinTimeVal() const;
		float getMaxTimeVal() const;
		unsigned int getNumberOfTimeSamples() const;

	private:

		TypeOfLaplacian m_LaplacianUsed; //< Type of the laplacian used in the extraction
		unsigned int m_nNoEigenVal; //< Number of eigen vals and vects extracted from the laplacian matrix
		float m_fTMin; //< Minimum value of the time interval
		float m_fTMax; //< Maximum value of the time interval
		unsigned int m_nNumberOfTimeSamples; //< Number of time values in the interval

		Eigen::SparseMatrix<float> *m_mLaplacian;	// < Laplacian matrix, it is a class member because multiple executions for different time intervals should be enabled without re-construction.

		/**
		* @brief	Create the STAR laplacian matrix from the polygon mesh saves it into the object
		*			[Currently] Only Triangular mesh is supported
		* @param	mesh Polygonial mesh from which the laplacian matrix will be created
		* @return	TACORE_OK if everything goes fine.
		*/
		Result createStarLaplacianMatrix(PolygonMesh *mesh);

	};
}

#endif
