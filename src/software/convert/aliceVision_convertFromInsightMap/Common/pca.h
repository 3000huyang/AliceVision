
#ifndef pca_h__
#define pca_h__

#include "common_global.h"
#include "numeric.h"

INSIGHT_NAME_SPACE


struct  PCA3d
{
	Vec3 meanPt;
	Vec3 eigenValues;
	Mat3 eigenVectors;

	bool operator()(std::vector<Vec3> &pts);
};

INSIGHT_NAME_SPACE_END
#endif // pca_h__