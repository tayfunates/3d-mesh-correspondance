#ifndef TACORE_COLOR_PALETTE_H
#define TACORE_COLOR_PALETTE_H

#include <vector>

namespace TACore
{
	class ColorPalette
	{
	public:
		ColorPalette();
		ColorPalette(float _c_res_);
		ColorPalette(float _k1_, float _k2_, float _k3_, float _c_res_);
		virtual ~ColorPalette();

		/**
		* @brief							Gets the color palette representing the RGB components
		*									Colors are represented between 0-255 inside the vector
		* @return							const and reference to vector of color palette
		*/
		const std::vector<std::vector<unsigned char> >& getPalette();

		/**
		* @brief							Gets the accurate color of a specific color by calculating using
		*									given magnitude value. It does NOT used pre-calculated palette to
		*									get the color. It is more accurate but slower when compared to
		*									using pre-calculated color palette of the tool.
		*
		* @param	mag						Scaled and transformed magnitude value of the spectogram.
		* @param	[out] r		 			Calculated red color.
		* @param	[out] g	 				Calculated green color.
		* @param	[out] b		 			Calculated blue color.
		*/
		static void getColor(float mag, unsigned char& r, unsigned char& g, unsigned char& b);

	private:
		std::vector<std::vector<unsigned char> > palette;
		std::vector<float>						 magnitude_powers;
		//k1, k2, k3 and c_res must be between 0 and 1. Constructor will scale color values to 0-255 while initializing the palette vector
		float k1;
		float k2;
		float k3;
		float c_res; //color resolution, high resolution of colors when it is near to 0.
	};
}

#endif
