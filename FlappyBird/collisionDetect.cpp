
#include "geometry.h"


// ���μ����ײ���
bool utility::CollideDetect(const Rectangle &r1, const Rectangle &r2) {
	//printf("r1:\n x: %f, %f\n r2:\n x: %f, %f\n", r1.projectToX().first, r1.projectToX().second, r2.projectToX().first, r2.projectToX().second);
	//printf("r1:\n y: %f, %f\n r2:\n y: %f, %f\n", r1.projectToY().first, r1.projectToY().second, r2.projectToY().first, r2.projectToY().second);

	// �ж�X�ϵ�ͶӰ�Ƿ����ص�
	bool isXOverlap =
		!(r1.projectToX().second < r2.projectToX().first ||
			r1.projectToX().first > r2.projectToX().second);
	if (!isXOverlap)
		return false;

	// �ж�Y�ϵ�ͶӰ�Ƿ����ص�
	bool isYOverlap =
		!(r1.projectToY().second < r2.projectToY().first ||
			r1.projectToY().first > r2.projectToY().second);

	//printf("\n\n\n\n\n\n\n\n\n");
	//printf("r1:\n x: %f, %f\n r2:\n x: %f, %f\n", r1.projectToX().first, r1.projectToX().second, r2.projectToX().first, r2.projectToX().second);
	//printf("r1:\n y: %f, %f\n r2:\n y: %f, %f\n", r1.projectToY().first, r1.projectToY().second, r2.projectToY().first, r2.projectToY().second);


	return isYOverlap;
}