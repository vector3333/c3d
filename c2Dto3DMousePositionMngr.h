#ifndef C2DTO3DMOUSEPOSITIONMNGR_H_
#define C2DTO3DMOUSEPOSITIONMNGR_H_

#include "C3D.h"


class c2Dto3DMousePositionMngr
{
public:
	c2Dto3DMousePositionMngr();
	~c2Dto3DMousePositionMngr();

	Ogre::Vector3 get3DMouseCoordinate( int iMousePosX, int iMousePosY );
	void activate2Dto3DMoveAndRotateSelectedObject(bool bActivate, std::string sObjectType, Ogre::Camera *pActvCamera);
	void setMouseClickPosition( int iMousePosX, int iMousePosY );
	void deleteLastCreatedObject(void);

	Ogre::SceneNode* get3DCoordinateNodePtr(void);

	void reset_fLastBuildingCreatedWithRotation(void);

private:
	Ogre::MovablePlane *p2Dto3DPlane;
	Ogre::Camera *pMouseRayCamera;
	
	Ogre::SceneNode* p2Dto3DPlaneNode;
	Ogre::SceneNode *p3DCoordinateNode;
	Ogre::Entity *p3DCoordinateEntity;

	Ogre::SceneNode *pNewCreatedObjectNode;

	int iClickMousePosX;
	int iClickMousePosY;

	float fHeightOffsetY;

	std::string sLastCreatedObject;

	float fLastBuildingCreatedWithRotation;
};


#endif

