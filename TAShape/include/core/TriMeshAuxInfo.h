#ifndef TA_TRI_MESH_AUX_INFO_H
#define TA_TRI_MESH_AUX_INFO_H

#include <core/Defs.h>

namespace TAShape
{
	class TriMeshAuxInfo
	{
	public:
		TriMeshAuxInfo();
		virtual ~TriMeshAuxInfo();

		double m_lfMaxEucDistanceBetweenTwoVertices;				//< Maximum euc distance between two vertices of the mesh;
		double m_lfMaxAreaOfTriangleConstructedByThreeVertices;		//< Maximum area of the triangle constructed from any three vertices of the mesh
		double m_lfMaxVolumeOfTetrahedronConstructedByForVertices;	//< Maximum volume of the tetrahedron constructed from any four vertices of the mesh

		/**
		* @brief			Saves the object to a file in ASCII format
		*					File format is as follows:
		*					Key1=Value1
		*					Key2=Value2
		*					...
		*						
		* @param			[in] pathToAuxInfoFile Path to file
		* @return			TACORE_OK if successfully saved.
		* @return			TACORE_FILE_ERROR if the file cannot be opened
		*/
		TACore::Result save(const std::string& pathToAuxInfoFile) const;

		/**
		* @brief			Loads the object from a file in ASCII format
		*					File format is as follows:
		*					Key1=Value1
		*					Key2=Value2
		*					...
		*
		* @param			[in] pathToAuxInfoFile Path to file
		* @return			TACORE_OK if successfully loaded.
		* @return			TACORE_FILE_ERROR if the file cannot be opened
		*/
		TACore::Result load(const std::string& pathToAuxInfoFile);

		/**
		* @brief			Prints the aux info of the mesh
		*/
		void print() const;

	private:
		const static char m_FileDelimeter = '=';
		
	};
}

#endif