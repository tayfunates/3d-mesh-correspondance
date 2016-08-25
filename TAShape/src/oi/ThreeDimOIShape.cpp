#include <oi/ThreeDimOIShape.h>

namespace TAShape
{
	TACore::Result ThreeDimOIShape::load(const std::string& pPath)
	{
		TACORE_CHECK_ARGS(pPath != "");
		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimOIShape::save(const std::string& pPath)
	{
		TACORE_CHECK_ARGS(pPath != "");
		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimOIShape::clear()
	{
		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimOIShape::show()
	{
		return TACore::TACORE_OK;
	}
}