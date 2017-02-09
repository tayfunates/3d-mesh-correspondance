This repository contains a collection of Digital Geometry Processing libraries to be used in achieving multiple shape correspondence. Libraries and their brief descriptions are placed below:

TACore: Contains core classes to be used by other libraries.
TAShape: Contains classes to represent a 3-dimensional shape using different formats. Currently, PCL and Open Inventor libraries are used  to represent the shape digitally.
TAFeatureExtraction: Contains an inheritance hierarchy to extract global or local features from 3-dimensional shape. Following feature types are implemented:
	Heat Kernel Signature (see http://www.lix.polytechnique.fr/~maks/papers/hks.pdf )
	Average Geodesic Distance
	Intrinsic Wave Descriptor (see http://www.staff.uni-mainz.de/wandm/papers/eg11_plansac.pdf )
	
TAFeatureExtraction.h includes the basic classes which are on top of the hierarchy.
PatchBasedPerVertexFeatureExtraction.h includes an interface for multi-scale 3-dimensional per vertex features

TAFeature: Contains feature types extracted by TAFeatureExtraction
TAPaper: Contains Matlab, C++ codes from different academic sources.