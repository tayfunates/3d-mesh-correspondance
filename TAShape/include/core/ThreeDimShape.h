#ifndef THREE_DIMENSIONAL_SHAPE_H
#define THREE_DIMENSIONAL_SHAPE_H

#include <core/Defs.h>

namespace TAShape
{
	/**
	* @brief	Abstract class for holding 3D shapes
	*			3D shapes from different libs like igl, pcl, open inventor must implement this class interface
	*			Each shape implementation must be in different projects so that dependencies are separated. 
	*/
	class ThreeDimShape
	{
	public:
		virtual ~ThreeDimShape() {}

		/**
		* @brief			Abstract method which loads the shape from a file provided by a path
		*					Different extensions are provided for different libs.
		*					To learn which extensions are used, check the method implementation inside the specific class.
		* @param			[in] pPath Path to shape file.
		* @return			TACORE_OK successfully loaded.
		*/
		virtual TACore::Result load(const std::string& pPath) = 0;

		/**
		* @brief			Abstract method which saves the shape to a file provided by a path
		*					Different extensions are provided for different libs.
		*					To learn which extensions are used, check the method implementation inside the specific class.
		* @param			[in] pPath Path to shape file.
		* @return			TACORE_OK successfully loaded.
		*/
		virtual TACore::Result save(const std::string& pPath) = 0;

		/**
		* @brief			Abstract method which clears the shape if dynamically alocated.
		*					For statically allocated shapes, wait for its lifescope.
		* @return			TACORE_OK successfully cleared.
		*/
		virtual TACore::Result clear() = 0;

		/**
		* @brief			Abstract method which checks whether the shape is empty or not.
		* @return			true if the shape is empty.
		* @return			false otherwise
		*/
		virtual bool empty() const = 0;

		/**
		* @brief			Abstract method which shows the shape in the visualisation tool provided by a specific lib
		*					This method is intended to be used in run time.
		*					Specific user interrupts may be required to continue the execution
		* @return			TACORE_OK successfully showed.
		*/
		virtual TACore::Result show() = 0;
	};
} 

#endif
