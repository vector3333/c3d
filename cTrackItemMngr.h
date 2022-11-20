#ifndef CTRACKITEMMNGR_H_
#define CTRACKITEMMNGR_H_

#include "C3D.h"


class cTrackItemMngr
{
public:
	cTrackItemMngr(void);
	~cTrackItemMngr(void);

	void startNewStrand(void);
	
	void mouseMoved( const OIS::MouseEvent &arg );
	void mousePressed( OIS::MouseButtonID id );
	void mouseReleased( OIS::MouseButtonID id );

	void frameRenderingQueued(float fTimeSinceLastFrame);

	bool getGizmoIsActiv(void);
	void showGizmoForSelectionBuffer( bool bShowGizmo );

	void cancelCurrentAction(void);

	void setMngrMode_TIM( std::string sNewMngrMode );
	std::string getCurrentMngrMode(void);

	void updateItemType(void);

	void equalizeTrack(int iLiftValue);

private:
	enum enTrackItemMngrMode
	{
	  TIMM_OFF,
	  TIMM_START_NEW_STRAND,
	  TIMM_ADD_NEXT_TRACK_ITEM
	};
	enum enTrackItemMngrMode eTrackItemMngrMode;
	
	void switchToAddNextItemMode(void);
	void _addNextTrackItem(void);
	void _removeAndDestroyLastItemCreated(void);
	int	 _getMultipartCount(void);
	void _setItemType( std::string sNewItemType );
	void _addNewItem_scrollItemTypeList( int iMouseMoveZ );
	int _addTP(void); // returns the number of additional created TP

	void _moveAndRotateStrand( const OIS::MouseEvent &arg );
	void _showGizmoForSelectionBuffer( bool bShowGizmo );

	void _setLastTPOfStrandActiv(void);

	// Selection Entitys
	Ogre::Entity *eStraights;
	//Ogre::Entity *eCurvesLeft;
	//Ogre::Entity *eCurvesRight;

	Ogre::Entity *eCurves15L;
	//Ogre::Entity *eCurves30L;
	Ogre::Entity *eCurves30L_R1;
	Ogre::Entity *eCurves30L_R2;
	Ogre::Entity *eCurves30L_R3;
	
	Ogre::Entity *eCurves60L;

	Ogre::Entity *eCurves15R;
	//Ogre::Entity *eCurves30R;
	Ogre::Entity *eCurves30R_R1;
	Ogre::Entity *eCurves30R_R2;
	Ogre::Entity *eCurves30R_R3;
	
	Ogre::Entity *eCurves60R;

	Ogre::Entity *selBufEntity;

	cTP *ptrLastCreatedTP;
	Ogre::SceneNode *ptrActvItemNode;
	Ogre::SceneNode *nMainTrackNodeParent;
	Ogre::SceneNode *nGizmoPositionNode;
	Ogre::SceneNode *ptrLiftNode;

	float fLiftValue;

	Ogre::Entity *eCompass_BG;
	Ogre::Entity *eCompass_Chart;
	Ogre::Entity *eCompass_Arrow;

	Ogre::SceneNode *ptrCompassCenterNode;
	Ogre::SceneNode *ptrCompassArrowNode;
	
	// Listen
	std::string sInsertItemType;
	int iActvItemTypeID;
		
	////////////////////////////////////////////////////////////////////
	int iItemTypeListIndex_0;
	
	int iItemTypeListIndex_15L;
	//int iItemTypeListIndex_30L;
	int iItemTypeListIndex_30L_R1;
	int iItemTypeListIndex_30L_R2;
	int iItemTypeListIndex_30L_R3;

	int iItemTypeListIndex_60L;

	int iItemTypeListIndex_15R;
	//int iItemTypeListIndex_30R;
	int iItemTypeListIndex_30R_R1;
	int iItemTypeListIndex_30R_R2;
	int iItemTypeListIndex_30R_R3;

	int iItemTypeListIndex_60R;
	////////////////////////////////////////////////////////////////////
	int iDefaultItemTypeListIndex_0;
	
	int iDefaultItemTypeListIndex_15L;
	//int iDefaultItemTypeListIndex_30L;
	int iDefaultItemTypeListIndex_30L_R1;
	int iDefaultItemTypeListIndex_30L_R2;
	int iDefaultItemTypeListIndex_30L_R3;

	int iDefaultItemTypeListIndex_60L;

	int iDefaultItemTypeListIndex_15R;
	//int iDefaultItemTypeListIndex_30R;
	int iDefaultItemTypeListIndex_30R_R1;
	int iDefaultItemTypeListIndex_30R_R2;
	int iDefaultItemTypeListIndex_30R_R3;

	int iDefaultItemTypeListIndex_60R;
	////////////////////////////////////////////////////////////////////

	std::vector<int> vecStraights;
	
	std::vector<int> vecCurves15L;
	//std::vector<int> vecCurves30L;
	std::vector<int> vecCurves30L_R1;
	std::vector<int> vecCurves30L_R2;
	std::vector<int> vecCurves30L_R3;
	
	std::vector<int> vecCurves60L;

	std::vector<int> vecCurves15R;
	//std::vector<int> vecCurves30R;
	std::vector<int> vecCurves30R_R1;
	std::vector<int> vecCurves30R_R2;
	std::vector<int> vecCurves30R_R3;

	std::vector<int> vecCurves60R;
	
	float fEndHeightLastTP;

	bool bGizmoIsActiv;

	void _createHairpinCurve(int iDirection);
	void _checkAutoAdd_shoulders( int iNumberOfNewCreatedTP );

	bool bAutoAdd_SH_L;
	bool bAutoAdd_SH_R;

	float fEqualizeTrack;
	bool bTrackIsEqualized;

	bool bTIM_LMB_isDown;
	bool bTIM_RMB_isDown;

	// Farbe des zu erstellenden Teils (blinken)
	void _colorBlink(float fTimeSinceLastFrame);
	bool bTIM_colorBlink_ON;
	
	float fBlink_RGBmulti;
	
	float iBlink_Rcv;
	float iBlink_Gcv;
	float iBlink_Bcv;
	float iBlink_Acv;

	bool bColorUp;
		
	// initialize
	void _createTypeListings_D124(void);
};

#endif