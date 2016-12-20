#include <core/ColorPalette.h>

namespace TACore
{
	ColorPalette::ColorPalette()
	{
		ColorPalette(0.01f, 0.05f, 0.15f, 0.0001f);
	}

	ColorPalette::ColorPalette(float _k1_, float _k2_, float _k3_, float _c_res_)
	{
		this->k1 = _k1_;
		this->k2 = _k2_;
		this->k3 = _k3_;
		this->c_res = _c_res_;
		std::vector<unsigned char> R;
		palette.push_back(R);
		std::vector<unsigned char> G;
		palette.push_back(G);
		std::vector<unsigned char> B;
		palette.push_back(B);
		for (float i = 0.0f; i < 1.0f; i += this->c_res)
		{
			if (i > k3)
			{
				palette[0].push_back((unsigned char)(((1.0f - i) / (1.0f - k3))*255.0f));
				palette[1].push_back(0);
				palette[2].push_back(0);
			}
			else if (i > k2)
			{
				unsigned char tmp = (unsigned char)(((i - k2) / (k3 - k2))*255.0f);
				palette[0].push_back(tmp);
				palette[1].push_back(255 - tmp);
				palette[2].push_back(0);
			}
			else if (i > k1)
			{
				unsigned char tmp = (unsigned char)(((i - k1) / (k2 - k1))*255.0f);
				palette[0].push_back(0);
				palette[1].push_back(tmp);
				palette[2].push_back(255 - tmp);
			}
			else
			{
				unsigned char tmp = (unsigned char)(((-1.0f / k1)*i + 1.0f)*255.0f);
				palette[0].push_back(tmp);
				palette[1].push_back(tmp);
				palette[2].push_back(255);
			}
		}
	}

	ColorPalette::ColorPalette(float _c_res_)
	{
		this->c_res = _c_res_;
		std::vector<unsigned char> R;
		palette.push_back(R);
		std::vector<unsigned char> G;
		palette.push_back(G);
		std::vector<unsigned char> B;
		palette.push_back(B);
		for (float i = 0.0f; i < 1.0f; i += this->c_res)
		{
			float r, g, b;
			const int gsteps = 5;
			float gradient[gsteps + 1][3] = {
					{ float(0.75), float(0.75), float(0.75) },    // lt gray
					{ float(0.30), float(0.60), float(1.00) },    // lt blue
					{ float(0.10), float(0.90), float(0.90) },    // turquoise
					{ float(0.90), float(0.90), float(0.00) },    // yellow
					{ float(1.00), float(0.00), float(0.00) },    // red
					{ float(1.00), float(1.00), float(1.00) }     // white
			};

			int left = int(i * gsteps);
			int right = (left == gsteps ? gsteps : left + 1);

			float rweight = (i * gsteps) - left;
			float lweight = 1.0f - rweight;

			r = (gradient[left][0] * lweight) + (gradient[right][0] * rweight);
			g = (gradient[left][1] * lweight) + (gradient[right][1] * rweight);
			b = (gradient[left][2] * lweight) + (gradient[right][2] * rweight);
			palette[0].push_back((unsigned char)(255 * r));
			palette[1].push_back((unsigned char)(255 * g));
			palette[2].push_back((unsigned char)(255 * b));

		}
	}

	void ColorPalette::getColor(float mag, unsigned char& r, unsigned char& g, unsigned char& b)
	{
		const int gsteps = 5;
		float gradient[gsteps + 1][3] = {
				{ float(0.75), float(0.75), float(0.75) },    // lt gray
				{ float(0.30), float(0.60), float(1.00) },    // lt blue
				{ float(0.10), float(0.90), float(0.90) },    // turquoise
				{ float(0.90), float(0.90), float(0.00) },    // yellow
				{ float(1.00), float(0.00), float(0.00) },    // red
				{ float(1.00), float(1.00), float(1.00) }     // white
		};

		int left = int(mag * gsteps);
		int right = (left == gsteps ? gsteps : left + 1);

		float rweight = (mag * gsteps) - left;
		float lweight = 1.0f - rweight;

		r = unsigned char(((gradient[left][0] * lweight) + (gradient[right][0] * rweight)) * 255.0f);
		g = unsigned char(((gradient[left][1] * lweight) + (gradient[right][1] * rweight)) * 255.0f);
		b = unsigned char(((gradient[left][2] * lweight) + (gradient[right][2] * rweight)) * 255.0f);
	}


	ColorPalette::~ColorPalette()
	{
	}


	const std::vector<std::vector<unsigned char> >& ColorPalette::getPalette()
	{
		return this->palette;
	}
}