#ifndef TA_FEATURE_H
#define TA_FEATURE_H

namespace TAFea
{
	enum FeatureType
	{
		FEA_TYPE_GLOBAL,
		FEA_TYPE_LOCAL
	};

	enum LocalFeaSource
	{
		LOCAL_FEA_SOURCE_VERTEX,
		LOCAL_FEA_SOURCE_FACE
	};

	//GeometricFeature
	//[TO DO:] Comment
	class GeometricFeature
	{
	public:
		virtual ~GeometricFeature() {}
		GeometricFeature() {}
		GeometricFeature(const GeometricFeature& other) {}

		virtual FeatureType getFeatureType() const = 0;
	private:
	};

	//GlobalFeature
	//[TO DO:] Comment
	class GlobalFeature : public GeometricFeature
	{
	public:
		virtual ~GlobalFeature() {}
		GlobalFeature() {}
		GlobalFeature(const GlobalFeature& other) {}

		virtual FeatureType getFeatureType() const { return FEA_TYPE_GLOBAL; }
	private:
	};

	//LocalFeature
	//[TO DO:] Comment
	class LocalFeature : public GeometricFeature
	{
	public:
		virtual ~LocalFeature() {}
		LocalFeature() { m_ID = -1;  }
		LocalFeature(const int& id) { m_ID = id;  }
		LocalFeature(const LocalFeature& other) { m_ID = other.m_ID;  }

		virtual FeatureType getFeatureType() const { return FEA_TYPE_LOCAL;  }
		virtual LocalFeaSource getLocalFeaSource() const = 0;
	private:
	protected:
		int m_ID;
	};

	//PerVertexFeature
	//[TO DO:] Comment
	class PerVertexFeature : public LocalFeature
	{
	public:
		virtual ~PerVertexFeature() {}
		PerVertexFeature() { m_ID = -1; }
		PerVertexFeature(const int& vertexid) { m_ID = vertexid; }
		PerVertexFeature(const PerVertexFeature& other) { m_ID = other.m_ID;  }

		virtual LocalFeaSource getLocalFeaSource() const { return LOCAL_FEA_SOURCE_VERTEX; }
	private:
	};

	//PerFaceFeature
	//[TO DO:] Comment
	class PerFaceFeature : public LocalFeature
	{
	public:
		virtual ~PerFaceFeature() {}
		PerFaceFeature() { m_ID = -1; }
		PerFaceFeature(const int& faceid) { m_ID = faceid; }
		PerFaceFeature(const PerFaceFeature& other) { m_ID = other.m_ID; }

		virtual LocalFeaSource getLocalFeaSource() const { return LOCAL_FEA_SOURCE_FACE; }
	private:
	};
}

#endif //TA_FEATURE_H