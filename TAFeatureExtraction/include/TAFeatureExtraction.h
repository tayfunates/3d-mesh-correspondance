#ifndef TA_FEATURE_EXTRACTION_H
#define TA_FEATURE_EXTRACTION_H

#include <vector>
#include <core/Defs.h>
#include <core/PolygonMesh.h>
#include <TAFeature.h>

using namespace TACore;
using namespace TAShape;
using namespace TAFea;

namespace TAFeaExt
{
	enum FeatureExtractionType
	{
		FEA_EXT_TYPE_GLOBAL,
		FEA_EXT_TYPE_LOCAL
	};

	enum LocalFeaExtSourceType
	{
		LOCAL_FEA_EXT_SOURCE_VERTEX,
		LOCAL_FEA_EXT_SOURCE_FACE
	};

	//FeatureExtraction
	//[TO DO:] Comment
	class FeatureExtraction
	{
	public:
		virtual ~FeatureExtraction() {}
		FeatureExtraction() {}
		FeatureExtraction(const FeatureExtraction& other) {}

		virtual FeatureExtractionType getFeatureExtractionType() const = 0;
	protected:
	private:
	};

	//GlobalFeatureExtraction
	//[TO DO:] Comment
	class GlobalFeatureExtraction : public FeatureExtraction
	{
	public:
		virtual ~GlobalFeatureExtraction() {}
		GlobalFeatureExtraction() {}
		GlobalFeatureExtraction(const GlobalFeatureExtraction& other) {}

		virtual FeatureExtractionType getFeatureExtractionType() const { return FEA_EXT_TYPE_GLOBAL; }
		virtual Result extract(PolygonMesh *mesh, GlobalFeature **outFeature) = 0;

	protected:
	private:

	};

	//LocalFeatureExtraction
	//[TO DO:] Comment
	class LocalFeatureExtraction : public FeatureExtraction
	{
	public:
		virtual ~LocalFeatureExtraction() {}
		LocalFeatureExtraction() {}
		LocalFeatureExtraction(const LocalFeatureExtraction& other) {}

		virtual FeatureExtractionType getFeatureExractionType() const { return FEA_EXT_TYPE_LOCAL; }
		virtual LocalFeaExtSourceType getLocalFeatureExtractionSourceType() const = 0;
		virtual Result extract(PolygonMesh *mesh, std::vector<LocalFeature*>& outFeatures) = 0;
		virtual Result extract(PolygonMesh *mesh, const int& id, LocalFeature **outFeature) = 0;

	protected:
	private:
	};

	//PerVertexFeatureExtraction
	//[TO DO:] Comment
	class PerVertexFeatureExtraction : public LocalFeatureExtraction
	{
	public:
		virtual ~PerVertexFeatureExtraction() {}
		PerVertexFeatureExtraction() {}
		PerVertexFeatureExtraction(const PerVertexFeatureExtraction& other) {}

		virtual LocalFeaExtSourceType getLocalFeatureExtractionSourceType() const { return LOCAL_FEA_EXT_SOURCE_VERTEX; }
	protected:
	private:
	};

	//PerFaceFeatureExtraction
	//[TO DO:] Comment
	class PerFaceFeatureExtraction : public LocalFeatureExtraction
	{
	public:
		virtual ~PerFaceFeatureExtraction() {}
		PerFaceFeatureExtraction() {}
		PerFaceFeatureExtraction(const PerFaceFeatureExtraction& other) {}

		virtual LocalFeaExtSourceType getLocalFeatureExtractionSourceType() const { return LOCAL_FEA_EXT_SOURCE_FACE;  }
	protected:
	private:
	};
}

#endif //TA_FEATURE_EXTRACTION_H