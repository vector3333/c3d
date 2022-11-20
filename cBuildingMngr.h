#ifndef CBUILDINGMNGR_H_
#define CBUILDINGMNGR_H_

#include "C3D.h"


class cBuildingMngr
{
public:
	cBuildingMngr(void);
	~cBuildingMngr(void);

	void createNewBuilding( std::string sBldToCreate );
	void addFloor(void);
	void removeSelectedFloor(void);

	void startMoveAndRotateBuilding(void);
	void startRotateBuilding(void);
	void startChangeBuildingHeight(void);

	void mouseMoved( const OIS::MouseEvent &arg );
	void mousePressed( OIS::MouseButtonID id );
	void mouseReleased( OIS::MouseButtonID id );

	void cancelCurrentAction(void);

private:
	enum enBuildingMngrMode
	{
	  BMM_OFF,
	  BMM_CREATE_NEW_BLD,
	  BMM_ADD_FLOOR,
	  BMM_MOVE_AND_ROTATE_BUILDING,
	  BMM_ROTATE_BUILDING,
	  BMM_CHANGE_BLD_HEIGHT
	};
	enum enBuildingMngrMode eBuildingMngrMode;

	int iActvStackID;
	Ogre::SceneNode *ptrActvBldNode;

	void _moveAndRotateBuildingStack( const OIS::MouseEvent &arg );
	void _stopCreateNewBuilding( bool bTakeChanges );
	void _removeAndDestroyLastCreatedBuildingStack(void);

	void _toggleFloorBuildingType( int iMouseMoveZ );
	void _stopAddFloor( bool bTakeChanges );

	void _rotateBuilding( int iMouseMoveZ );
	void _stopRotateBuilding( bool bTakeChanges );

	void _moveAndRotateBuilding( const OIS::MouseEvent &arg );
	void _stopMoveAndRotateBuilding( bool bTakeChanges );

	void _changeBuildingHeight( const OIS::MouseEvent &arg );
	void _stopChangeBuildingHeight( bool bTakeChanges );
	
	Ogre::Vector3 v3RememberLastBuildingStackPosition;
	Ogre::Quaternion quatRememberLastBuildingRotation;
};

#endif
