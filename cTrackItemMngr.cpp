#include "cTrackItemMngr.h"


cTrackItemMngr::cTrackItemMngr(void)
{
	eTrackItemMngrMode = TIMM_OFF;

	eStraights = appPtr->mSceneMgrWorld->createEntity( "entAddItem_0", "TIM_addItem_0.mesh");
	
	eCurves15L = appPtr->mSceneMgrWorld->createEntity( "entAddItem_15L", "TIM_addItem_15L.mesh");
	//eCurves30L = appPtr->mSceneMgrWorld->createEntity( "entAddItem_30L", "TIM_addItem_30L.mesh");
	eCurves30L_R1 = appPtr->mSceneMgrWorld->createEntity( "entAddItem_30L_R1", "TIM_addItem_30L_R1.mesh");
	eCurves30L_R2 = appPtr->mSceneMgrWorld->createEntity( "entAddItem_30L_R2", "TIM_addItem_30L_R2.mesh");
	eCurves30L_R3 = appPtr->mSceneMgrWorld->createEntity( "entAddItem_30L_R3", "TIM_addItem_30L_R3.mesh");
	
	eCurves60L = appPtr->mSceneMgrWorld->createEntity( "entAddItem_60L", "TIM_addItem_60L.mesh");

	eCurves15R = appPtr->mSceneMgrWorld->createEntity( "entAddItem_15R", "TIM_addItem_15R.mesh");
	//eCurves30R = appPtr->mSceneMgrWorld->createEntity( "entAddItem_30R", "TIM_addItem_30R.mesh");
	eCurves30R_R1 = appPtr->mSceneMgrWorld->createEntity( "entAddItem_30R_R1", "TIM_addItem_30R_R1.mesh");
	eCurves30R_R2 = appPtr->mSceneMgrWorld->createEntity( "entAddItem_30R_R2", "TIM_addItem_30R_R2.mesh");
	eCurves30R_R3 = appPtr->mSceneMgrWorld->createEntity( "entAddItem_30R_R3", "TIM_addItem_30R_R3.mesh");

	eCurves60R = appPtr->mSceneMgrWorld->createEntity( "entAddItem_60R", "TIM_addItem_60R.mesh");

	ptrLastCreatedTP = NULL;
	ptrActvItemNode = NULL;
	selBufEntity = NULL;

	nMainTrackNodeParent = T[0]->MainTrackNode->getParentSceneNode();
	nGizmoPositionNode = appPtr->mSceneMgrWorld->getRootSceneNode()->createChildSceneNode( "nGizmoPositionNode" );
	//nGizmoPositionNode->setScale(1,1,1);

	ptrLiftNode = NULL;
	fLiftValue = 0.01f;
	fEndHeightLastTP = 0.0f;

	fEqualizeTrack = 0.0f;
	bTrackIsEqualized = false;

	bAutoAdd_SH_L = true;
	bAutoAdd_SH_R = true;

	bTIM_LMB_isDown = false;
	bTIM_RMB_isDown = false;

	bGizmoIsActiv = false;

	// Farbe des zu erstellenden Teils (blinken)
	bTIM_colorBlink_ON = false;
	bColorUp = true;

	fBlink_RGBmulti = 1.0f/256;
	
	iBlink_Rcv = 128.0f;
	iBlink_Gcv = 128.0f;
	iBlink_Bcv = 255.0f;
	iBlink_Acv = 1.0f;

	// Compass
	eCompass_BG = appPtr->mSceneMgrWorld->createEntity( "entCompass_BG", "Compass_BG.mesh");
	eCompass_Chart = appPtr->mSceneMgrWorld->createEntity( "entCompass_Chart", "Compass_Chart.mesh");
	eCompass_Arrow = appPtr->mSceneMgrWorld->createEntity( "entCompass_Arrow", "Compass_Arrow.mesh");

	ptrCompassCenterNode = appPtr->mSceneMgrWorld->getRootSceneNode()->createChildSceneNode( "nCompassPositionNode" );
	ptrCompassArrowNode = appPtr->mSceneMgrWorld->getRootSceneNode()->createChildSceneNode( "nCompassArrowNode" );

	ptrCompassCenterNode->attachObject( eCompass_BG );
	ptrCompassCenterNode->attachObject( eCompass_Chart );
	ptrCompassArrowNode->attachObject( eCompass_Arrow );

	ptrCompassCenterNode->getParentSceneNode()->removeChild( ptrCompassCenterNode );
	ptrCompassArrowNode->getParentSceneNode()->removeChild( ptrCompassArrowNode );

	_createTypeListings_D124();
}

cTrackItemMngr::~cTrackItemMngr(void)
{
	eStraights->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eStraights );

	eCurves15L->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves15L );
	//eCurves30L->detachFromParent();
	//appPtr->mSceneMgrWorld->destroyEntity( eCurves30L );
	eCurves30L_R1->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves30L_R1 );
	eCurves30L_R2->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves30L_R2 );
	eCurves30L_R3->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves30L_R3 );

	eCurves60L->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves60L );
	
	/*eCurvesLeft->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurvesLeft );*/

	eCurves15R->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves15R );
	//eCurves30R->detachFromParent();
	//appPtr->mSceneMgrWorld->destroyEntity( eCurves30R );
	eCurves30R_R1->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves30R_R1 );
	eCurves30R_R2->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves30R_R2 );
	eCurves30R_R3->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves30R_R3 );

	eCurves60R->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurves60R );
	
	/*eCurvesRight->detachFromParent();
	appPtr->mSceneMgrWorld->destroyEntity( eCurvesRight );*/

	ptrActvItemNode = NULL;
}

void cTrackItemMngr::mouseMoved( const OIS::MouseEvent &arg )
{
	if ( eTrackItemMngrMode == TIMM_START_NEW_STRAND )
	{
		// neuen Strang positionieren und Startrichtung festlegen (drehen)
		_moveAndRotateStrand( arg );
	}
	else if ( eTrackItemMngrMode == TIMM_ADD_NEXT_TRACK_ITEM )
	{
		updateItemType();
		
		if ( arg.state.Z.rel != 0 )
			_addNewItem_scrollItemTypeList( arg.state.Z.rel );
	}
	
	return;
}
void cTrackItemMngr::mousePressed( OIS::MouseButtonID mbPressed )
{
	if ( mbPressed == OIS::MB_Left )
		bTIM_LMB_isDown = true;
	if ( mbPressed == OIS::MB_Right )
		bTIM_RMB_isDown = true;

	if ( bTIM_LMB_isDown && bTIM_RMB_isDown )
	{
		bTIM_LMB_isDown = bTIM_LMB_isDown;
	}
	
	/*if ( eTrackItemMngrMode == TIMM_START_NEW_STRAND )
	{
		if ( mbPressed == OIS::MB_Left )
		{

		}
		else if ( mbPressed == OIS::MB_Middle )
		{

		}
		else if ( mbPressed == OIS::MB_Right )
		{

		}
	}
	else if ( eTrackItemMngrMode == TIMM_ADD_NEXT_TRACK_ITEM )
	{
		if ( mbPressed == OIS::MB_Left )
		{

		}
		else if ( mbPressed == OIS::MB_Middle )
		{

		}
		else if ( mbPressed == OIS::MB_Right )
		{

		}
	}*/
	
	return;
}
void cTrackItemMngr::mouseReleased( OIS::MouseButtonID mbReleased )
{
	if ( eTrackItemMngrMode == TIMM_START_NEW_STRAND )
	{
		if ( mbReleased == OIS::MB_Left )
		{
			// NEW_STRAND-Modus beenden und in Modus ADD_TRACK_ITEM wechseln
			switchToAddNextItemMode();
		}
		else if ( mbReleased == OIS::MB_Middle )
		{

		}
		else if ( mbReleased == OIS::MB_Right )
		{
			cancelCurrentAction();
		}
	}
	else if ( eTrackItemMngrMode == TIMM_ADD_NEXT_TRACK_ITEM )
	{
		if ( mbReleased == OIS::MB_Left )
		{
			// naechstes TP erstellen
			_addNextTrackItem();
			appPtr->bUpdateTP_Pins = true;
			//T[0]->S[iActvStrand]->updateTPConnectors();
		}
		else if ( mbReleased == OIS::MB_Middle )
		{

		}
		else if ( mbReleased == OIS::MB_Right )
		{
			if ( bTIM_LMB_isDown )
			{
				cancelCurrentAction();
			}
			else
			{
				// zuerst das Vorschau-TP loeschen
				_removeAndDestroyLastItemCreated();
				
				if ( !T[0]->S[iActvStrand]->TP.empty() )
				{
					// Falls Ueberfahrt: Endhoehe anpassen
					int iNewLastIndex = T[0]->S[iActvStrand]->TP.size()-1;
					if ( T[0]->S[iActvStrand]->TP[iNewLastIndex]->get_TPID() == 502 ) // CrossingNew_Up wird geloescht
						fEndHeightLastTP -= 11.0f;
					else if ( T[0]->S[iActvStrand]->TP[iNewLastIndex]->get_TPID() == 504 ) // CrossingNew_Down wird geloescht
						fEndHeightLastTP += 11.0f;
				
					// das zuletzt erstellte Teil loeschen
					_removeAndDestroyLastItemCreated();
					_setLastTPOfStrandActiv();
				}
				else
				{
					cancelCurrentAction();
				}
			}
		}
	}

	if ( mbReleased == OIS::MB_Left )
		bTIM_LMB_isDown = false;
	if ( mbReleased == OIS::MB_Right )
		bTIM_RMB_isDown = false;
	
	return;
}

void cTrackItemMngr::frameRenderingQueued(float fTimeSinceLastFrame)
{
	if ( bTIM_colorBlink_ON )
	{
		_colorBlink( fTimeSinceLastFrame );
	}
	
	return;
}

bool cTrackItemMngr::getGizmoIsActiv(void)
{
	return bGizmoIsActiv;
}

void cTrackItemMngr::setMngrMode_TIM( std::string sNewMngrMode )
{
	if ( sNewMngrMode == "TIMM_OFF" )
		eTrackItemMngrMode = TIMM_OFF;
	else if ( sNewMngrMode == "TIMM_START_NEW_STRAND" )
		eTrackItemMngrMode = TIMM_START_NEW_STRAND;
	else if ( sNewMngrMode == "TIMM_ADD_NEXT_TRACK_ITEM" )
		eTrackItemMngrMode = TIMM_ADD_NEXT_TRACK_ITEM;
	else
		return; // Unbekannter Modus
	
	return;
}
std::string cTrackItemMngr::getCurrentMngrMode(void)
{
	std::string sCurrentMode = "Unbekannt";
	
	if ( eTrackItemMngrMode == TIMM_OFF )
		sCurrentMode = "TIMM_OFF";
	else if ( eTrackItemMngrMode == TIMM_START_NEW_STRAND )
		sCurrentMode = "TIMM_START_NEW_STRAND";
	else if ( eTrackItemMngrMode == TIMM_ADD_NEXT_TRACK_ITEM )
		sCurrentMode = "TIMM_ADD_NEXT_TRACK_ITEM";
	else
		sCurrentMode = sCurrentMode; // Unbekannter Modus
		
	return sCurrentMode;
}

void cTrackItemMngr::updateItemType(void)
{
	selBufEntity = appPtr->getBufferEntity();
	if ( appPtr->getBufferEntity() != NULL )
	{
		_setItemType( selBufEntity->getName() );
	}

	return;
}

void cTrackItemMngr::startNewStrand(void)
{
	// Ein neuer Strang beginnt
	eTrackItemMngrMode = TIMM_START_NEW_STRAND;
	
	// Listen auf DefaultIndex
	//iItemTypeListIndex_0 = iDefaultItemTypeListIndex_0;
	//iItemTypeListIndex_1 = iDefaultItemTypeListIndex_1;
	//iItemTypeListIndex_2 = iDefaultItemTypeListIndex_2;

	iItemTypeListIndex_0 = iDefaultItemTypeListIndex_0;
	iItemTypeListIndex_15L = iDefaultItemTypeListIndex_15L;
	//iItemTypeListIndex_30L = iDefaultItemTypeListIndex_30L;
	iItemTypeListIndex_30L_R1 = iDefaultItemTypeListIndex_30L_R1;
	iItemTypeListIndex_30L_R2 = iDefaultItemTypeListIndex_30L_R2;
	iItemTypeListIndex_30L_R3 = iDefaultItemTypeListIndex_30L_R3;
		
	iItemTypeListIndex_60L = iDefaultItemTypeListIndex_60L;

	iItemTypeListIndex_15R = iDefaultItemTypeListIndex_15R;
	//iItemTypeListIndex_30R = iDefaultItemTypeListIndex_30R;
	iItemTypeListIndex_30R_R1 = iDefaultItemTypeListIndex_30R_R1;
	iItemTypeListIndex_30R_R2 = iDefaultItemTypeListIndex_30R_R2;
	iItemTypeListIndex_30R_R3 = iDefaultItemTypeListIndex_30R_R3;

	iItemTypeListIndex_60R = iDefaultItemTypeListIndex_60R;
	
	// neues Teil erstellen ( ein neuer Strang beginnt immer mit einer Standardgeraden )
	iActvItemTypeID = 0;
	sInsertItemType = "entAddItem_0";
	
	// Testweise auskommentiert
	//appPtr->cRttShadowPlane1->setRTTShadowVisible(false);
	
	// neuen Strang erstellen
	T[0]->addNewStrand();
	iActvStrand = T[0]->S.size()-1;

	// erstes TP im neuen Strang erstellen
	T[0]->S[iActvStrand]->TP.push_back( new cTP( gl_sSystemName, vecStraights[iDefaultItemTypeListIndex_0] ));
	iActvTP1 = iActvTP2 = T[0]->S[iActvStrand]->TP.size()-1;

	ptrLastCreatedTP = T[0]->S[iActvStrand]->TP[iActvTP1];
	ptrActvItemNode = T[0]->S[iActvStrand]->getPtrToStrandNode();

	appPtr->bUpdateTP_Pins = true;
	//T[0]->S[iActvStrand]->updateTPConnectors();

	ptrLiftNode = ptrLastCreatedTP->getStartNodePtr();
	ptrLiftNode->translate( 0, fLiftValue, 0, Ogre::Node::TS_LOCAL );

	fEndHeightLastTP = 0.0f;

	//_checkAutoAdd_shoulders(1);

	const OIS::MouseState mouseState = appPtr->mMouse->getMouseState();
	ptrActvItemNode->_setDerivedPosition( appPtr->c2Dto3DMousePositionMngr1->get3DMouseCoordinate( mouseState.X.abs, mouseState.Y.abs ));

	// Compass
	appPtr->mSceneMgrWorld->getRootSceneNode()->addChild( ptrCompassCenterNode );
	ptrCompassCenterNode->_setDerivedPosition( ptrActvItemNode->_getDerivedPosition() );

	ptrLastCreatedTP->getStartNodePtr()->addChild( ptrCompassArrowNode );

	//bTIM_colorBlink_ON = true;

	return;
}
void cTrackItemMngr::_addNextTrackItem(void)
{
	// Gizmo an zuletzt erstellten Entity ausrichten
	nGizmoPositionNode->_setDerivedPosition( ptrLastCreatedTP->getEndNodePtr1()->_getDerivedPosition() );
	nGizmoPositionNode->_setDerivedOrientation( ptrLastCreatedTP->getEndNodePtr1()->_getDerivedOrientation() );
	
	ptrLiftNode->translate( 0, -fLiftValue, 0, Ogre::Node::TS_LOCAL );
	
	if ( ptrLastCreatedTP->get_TPID() == 502 )
	{
		fEndHeightLastTP += 11.0f;
		iItemTypeListIndex_0++;	// naechstes TP "CrossingDown" vorschlagen
	}
	else if ( ptrLastCreatedTP->get_TPID() == 504 )
	{
		fEndHeightLastTP -= 11.0f;
		iItemTypeListIndex_0 = iDefaultItemTypeListIndex_0;
	}
	// Nach einfuegen einer Startampel auf S3 umschalten
	else if ( ptrLastCreatedTP->get_TPID() >= 301 && ptrLastCreatedTP->get_TPID() <= 306 )
	{
		iItemTypeListIndex_0 = iDefaultItemTypeListIndex_0;
	}
	else if ( ptrLastCreatedTP->get_TPID() == 35 ||		/*create_S3_starter4*/
		      ptrLastCreatedTP->get_TPID() == 201 ||	/*create_ControlUnit*/
			  ptrLastCreatedTP->get_TPID() == 1010 ||	/*create_CheckLane*/
			  ptrLastCreatedTP->get_TPID() == 10021 ||	/*create_PitInRight*/
			  ptrLastCreatedTP->get_TPID() == 10022 ||	/*create_PitOutRight*/
			  ptrLastCreatedTP->get_TPID() == 4546 ||	/*DoubleLaneChangeSection*/
			  ptrLastCreatedTP->get_TPID() == 5012 ||	/*LaneChangeSectionLeft*/
			  ptrLastCreatedTP->get_TPID() == 5022 )	/*LaneChangeSectionRight*/
	{
		iItemTypeListIndex_0 = iDefaultItemTypeListIndex_0;
	}
	else if ( ptrLastCreatedTP->get_TPID() == 3012 || ptrLastCreatedTP->get_TPID() == 3022 ) // wenn Hairpin
	{
		// nach einfuegen einer Haarnadelkurve automatisch auf DefaultIndex von 60X
		iItemTypeListIndex_60L = iDefaultItemTypeListIndex_60L;
		iItemTypeListIndex_60R = iDefaultItemTypeListIndex_60R;

		// nach einfuegen einer Haarnadelkurve automatisch als naechstes eine S3 vorschlagen
		iActvItemTypeID = 0;
		iItemTypeListIndex_0 = iDefaultItemTypeListIndex_0;
	}
	
	// Blink-Material ausschalten
	int iTPIndex = T[0]->S[iActvStrand]->TP.size()-1;
	for ( int i=_getMultipartCount(); i>=1; i-- )
	{
		T[0]->S[iActvStrand]->TP[iTPIndex]->setStandardMaterial();
		iTPIndex--;
	}
	
	// Ptr auf NULL <-- Wichtig, da sonst das zuletzt erstellte TP geloescht wird !
	ptrLastCreatedTP = NULL;
	
	// neues TP anfuegen
	_addNewItem_scrollItemTypeList( 0 );

	gl_bUpdateCamera = true;
	gl_bUpdateGridSize = true;

	return;
}
int cTrackItemMngr::_addTP(void)
{
	int iNumberOfSecondaryItemsCreatet = 0;
	
	if ( iActvItemTypeID == 0 )
	{
		switch ( vecStraights[iItemTypeListIndex_0] )
		{
			case 4546:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 4545 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// DoubleLaneChangeSection
			case 5012:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 5011 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// LaneChangeSectionLeft
			case 5022:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 5021 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// LaneChangeSectionRight

			case 502:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 501 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// CrossingNewUp
			case 504:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 503 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// CrossingNewDown
		}

		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecStraights[iItemTypeListIndex_0] ));
	}
	else if ( iActvItemTypeID == 601 )
	{
		switch ( vecCurves60L[iItemTypeListIndex_60L] )
		{
			case 11601:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 5011 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// LCC - Kurventeil - aussen nach innen
			case 31601:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 5021 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// LCC - Kurventeil - innen nach aussen

			case 3012:		_createHairpinCurve(1);
							iNumberOfSecondaryItemsCreatet = 4;													break;		// Hairpin_L
		}

		if ( vecCurves60L[iItemTypeListIndex_60L] != 3012 ) // wenn keine Hairpin
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves60L[iItemTypeListIndex_60L] ));
	}
	else if ( iActvItemTypeID == 602 )
	{
		switch ( vecCurves60R[iItemTypeListIndex_60R] )
		{
			case 21602:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 5021 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// LCC - Kurventeil - aussen nach innen
			case 41602:		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 5011 ));
							iNumberOfSecondaryItemsCreatet = 1;													break;		// LCC - LaneChangeSectionLeft

			case 3022:		_createHairpinCurve(2);
							iNumberOfSecondaryItemsCreatet = 4;													break;		// Hairpin_R
		}

		if ( vecCurves60R[iItemTypeListIndex_60R] != 3022 ) // wenn keine Hairpin
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves60R[iItemTypeListIndex_60R] ));
	}

	if ( iNumberOfSecondaryItemsCreatet > 0 )
	{
		//T[0]->S[iActvStrand]->TP[ T[0]->S[iActvStrand]->TP.size()-1 ]->setCreateModeMaterial();
	}
	
	return iNumberOfSecondaryItemsCreatet+1;
}

void cTrackItemMngr::switchToAddNextItemMode(void)
{
	// Modus wechseln
	eTrackItemMngrMode = TIMM_ADD_NEXT_TRACK_ITEM;

	// Gizmo an zuletzt erstellten Entity ausrichten
	nGizmoPositionNode->_setDerivedPosition( ptrActvItemNode->_getDerivedPosition() );
	nGizmoPositionNode->_setDerivedOrientation( ptrActvItemNode->_getDerivedOrientation() );

	// Compass ausblenden
	ptrCompassCenterNode->getParentSceneNode()->removeChild( ptrCompassCenterNode );
	ptrCompassArrowNode->getParentSceneNode()->removeChild( ptrCompassArrowNode );

	// Randstreifen hinzufuegen, wenn automatisch anfuegen ausgewaehlt ist
	_checkAutoAdd_shoulders(1);

	// BlinkiBlinki auf "AN"
	bTIM_colorBlink_ON = true;

	bGizmoIsActiv = true;

	appPtr->show_toolTipInfoPanel( "createNewObject__addNextTrackItem" );
	
	return;
}

void cTrackItemMngr::_removeAndDestroyLastItemCreated(void)
{
	if ( ptrLastCreatedTP != NULL )
	{
		appPtr->NEW_NEW_deselectSelectedEntitys();
	
		int iNumTPtoRemove = _getMultipartCount();
		
		for ( int i=1; i<=iNumTPtoRemove; i++ )
		{
			delete T[0]->S[iActvStrand]->TP[ T[0]->S[iActvStrand]->TP.size()-1 ];
			T[0]->S[iActvStrand]->TP[ T[0]->S[iActvStrand]->TP.size()-1 ] = NULL;
			T[0]->S[iActvStrand]->TP.pop_back();
		}

		ptrLastCreatedTP = NULL;
		ptrActvItemNode = NULL;
		
		gl_bUpdateShadowTex = true;
		appPtr->bUpdateTP_Pins = true;
	}

	return;
}
int cTrackItemMngr::_getMultipartCount(void)
{
	int iTPCount = 1;
	
	switch ( ptrLastCreatedTP->get_TPID() )
	{
		// D124 Geraden
		case 4546:		iTPCount = 2;			break;		// DoubleLaneChangeSection
		case 5012:		iTPCount = 2;			break;		// LaneChangeSectionLeft
		case 5022:		iTPCount = 2;			break;		// LaneChangeSectionRight

		case 502:		iTPCount = 2;			break;		// CrossingNewUp
		case 504:		iTPCount = 2;			break;		// CrossingNewDown
	
		case 11601:		iTPCount = 2;			break;		// DoubleLaneChangeSection
		case 31601:		iTPCount = 2;			break;		// LaneChangeSectionLeft
		
		// D124 Kurven
		case 21602:		iTPCount = 2;			break;		// LCC - Kurventeil - aussen nach innen
		case 41602:		iTPCount = 2;			break;		// LCC - Kurventeil - innen nach aussen

		case 3012:		iTPCount = 5;			break;		// Hairpin_L
		
		case 3022:		iTPCount = 5;			break;		// Hairpin_R
	}
	
	return iTPCount;
}

void cTrackItemMngr::_setItemType( std::string sNewItemType )
{
	if ( sInsertItemType != sNewItemType )
	{
		if ( ptrLastCreatedTP )
		{
			// wenn zuletzt eine Haarnadelkurve ausgewaehlt war auf Standard Index zuruecksetzen
			if ( ptrLastCreatedTP->get_TPID() == 3012 )
				iItemTypeListIndex_60L = iDefaultItemTypeListIndex_60L;
			else if ( ptrLastCreatedTP->get_TPID() == 3022 )
				iItemTypeListIndex_60R = iDefaultItemTypeListIndex_60R;
		
			// HighBankedCurves
			else if ( ptrLastCreatedTP->get_TPID() == 301301 )
				iItemTypeListIndex_30L_R1 = iDefaultItemTypeListIndex_30L_R1;
			else if ( ptrLastCreatedTP->get_TPID() == 301302 )
				iItemTypeListIndex_30R_R1 = iDefaultItemTypeListIndex_30R_R1;

			else if ( ptrLastCreatedTP->get_TPID() == 302301 )
				iItemTypeListIndex_30L_R2 = iDefaultItemTypeListIndex_30L_R2;
			else if ( ptrLastCreatedTP->get_TPID() == 302302 )
				iItemTypeListIndex_30R_R2 = iDefaultItemTypeListIndex_30R_R2;

			else if ( ptrLastCreatedTP->get_TPID() == 303301 )
				iItemTypeListIndex_30L_R3 = iDefaultItemTypeListIndex_30L_R3;
			else if ( ptrLastCreatedTP->get_TPID() == 303302 )
				iItemTypeListIndex_30R_R3 = iDefaultItemTypeListIndex_30R_R3;

			else if ( ptrLastCreatedTP->get_TPID() == 304151 )
				iItemTypeListIndex_15L = iDefaultItemTypeListIndex_15L;
			else if ( ptrLastCreatedTP->get_TPID() == 304152 )
				iItemTypeListIndex_15R = iDefaultItemTypeListIndex_15R;
		
			_removeAndDestroyLastItemCreated();
		}
		
		if ( sNewItemType == "entAddItem_15L" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 151;
			_addNewItem_scrollItemTypeList( 0 );
		}
		else if ( sNewItemType == "entAddItem_30L_R1" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 3011;
			_addNewItem_scrollItemTypeList( 0 );
		}
		else if ( sNewItemType == "entAddItem_30L_R2" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 3012;
			_addNewItem_scrollItemTypeList( 0 );
		}
		else if ( sNewItemType == "entAddItem_30L_R3" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 3013;
			_addNewItem_scrollItemTypeList( 0 );
		}
		else if ( sNewItemType == "entAddItem_60L" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 601;
			_addNewItem_scrollItemTypeList( 0 );
		}

		else if ( sNewItemType == "entAddItem_15R" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 152;
			_addNewItem_scrollItemTypeList( 0 );
		}
		else if ( sNewItemType == "entAddItem_30R_R1" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 3021;
			_addNewItem_scrollItemTypeList( 0 );
		}
		else if ( sNewItemType == "entAddItem_30R_R2" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 3022;
			_addNewItem_scrollItemTypeList( 0 );
		}
		else if ( sNewItemType == "entAddItem_30R_R3" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 3023;
			_addNewItem_scrollItemTypeList( 0 );
		}
		else if ( sNewItemType == "entAddItem_60R" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 602;
			_addNewItem_scrollItemTypeList( 0 );
		}

		else // if ( sNewItemType == "entAddItem_0" )
		{
			sInsertItemType = sNewItemType;
			iActvItemTypeID = 0;
			_addNewItem_scrollItemTypeList( 0 );
		}
	}

	return;
}
void cTrackItemMngr::_addNewItem_scrollItemTypeList( int iMouseMoveZ )
{
	int iAddIndex = 0;
	
	if ( iMouseMoveZ > 0 )
		iAddIndex = 1;
	else if ( iMouseMoveZ < 0 )
		iAddIndex = -1;

	bool bUpdateLastCreatedTP = false;
	int iNumberOfNewCreatedTP = 1;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if ( iActvItemTypeID == 0 )
	{
		iItemTypeListIndex_0 += iAddIndex;
		
		if ( iItemTypeListIndex_0 >= 1 && iItemTypeListIndex_0 <= vecStraights.size()-1 )
		{
			// Ueberfahrt down nur dann einfuegen, wenn seine Endhoehe nach dem einfuegen >= 0.0f ist
			if ( vecStraights[iItemTypeListIndex_0] == 504 )
			{
				if ( fEndHeightLastTP < 11.0f )
					iItemTypeListIndex_0--;
			}

			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
			
			// pruefen, ob ein 2. TP erstellt werden muss (z.B. fuer LaneChangeSection)
			iNumberOfNewCreatedTP = _addTP();
			
			//T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecStraights[iItemTypeListIndex_0] ));
			bUpdateLastCreatedTP = true;
		}
		else if ( iItemTypeListIndex_0 < 1 )
		{
			iItemTypeListIndex_0 = 1;
		}
		else // if ( iItemTypeListIndex_0 > vecStraights.size()-1 )
		{
			iItemTypeListIndex_0 = vecStraights.size()-1;
		} 
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if ( iActvItemTypeID == 151 )
	{
		iItemTypeListIndex_15L += iAddIndex;
		
		if (iItemTypeListIndex_15L < 1 )
		{
			iItemTypeListIndex_15L = 1;
		}
		else if ( iItemTypeListIndex_15L > vecCurves15L.size()-1 )
		{
			iItemTypeListIndex_15L = vecCurves15L.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves15L[iItemTypeListIndex_15L] ));
			bUpdateLastCreatedTP = true;
		}
	}
	else if ( iActvItemTypeID == 152 )
	{
		iItemTypeListIndex_15R += iAddIndex;
		
		if (iItemTypeListIndex_15R < 1 )
		{
			iItemTypeListIndex_15R = 1;
		}
		else if ( iItemTypeListIndex_15R > vecCurves15R.size()-1 )
		{
			iItemTypeListIndex_15R = vecCurves15R.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves15R[iItemTypeListIndex_15R] ));
			bUpdateLastCreatedTP = true;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if ( iActvItemTypeID == 3011 )
	{
		iItemTypeListIndex_30L_R1 += iAddIndex;
		
		if (iItemTypeListIndex_30L_R1 < 1 )
		{
			iItemTypeListIndex_30L_R1 = 1;
		}
		else if ( iItemTypeListIndex_30L_R1 > vecCurves30L_R1.size()-1 )
		{
			iItemTypeListIndex_30L_R1 = vecCurves30L_R1.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves30L_R1[iItemTypeListIndex_30L_R1] ));
			bUpdateLastCreatedTP = true;
		}
	}
	else if ( iActvItemTypeID == 3012 )
	{
		iItemTypeListIndex_30L_R2 += iAddIndex;
		
		if (iItemTypeListIndex_30L_R2 < 1 )
		{
			iItemTypeListIndex_30L_R2 = 1;
		}
		else if ( iItemTypeListIndex_30L_R2 > vecCurves30L_R2.size()-1 )
		{
			iItemTypeListIndex_30L_R2 = vecCurves30L_R2.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves30L_R2[iItemTypeListIndex_30L_R2] ));
			bUpdateLastCreatedTP = true;
		}
	}
	else if ( iActvItemTypeID == 3013 )
	{
		iItemTypeListIndex_30L_R3 += iAddIndex;
		
		if (iItemTypeListIndex_30L_R3 < 1 )
		{
			iItemTypeListIndex_30L_R3 = 1;
		}
		else if ( iItemTypeListIndex_30L_R3 > vecCurves30L_R3.size()-1 )
		{
			iItemTypeListIndex_30L_R3 = vecCurves30L_R3.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves30L_R3[iItemTypeListIndex_30L_R3] ));
			bUpdateLastCreatedTP = true;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if ( iActvItemTypeID == 3021 )
	{
		iItemTypeListIndex_30R_R1 += iAddIndex;
		
		if (iItemTypeListIndex_30R_R1 < 1 )
		{
			iItemTypeListIndex_30R_R1 = 1;
		}
		else if ( iItemTypeListIndex_30R_R1 > vecCurves30R_R1.size()-1 )
		{
			iItemTypeListIndex_30R_R1 = vecCurves30R_R1.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves30R_R1[iItemTypeListIndex_30R_R1] ));
			bUpdateLastCreatedTP = true;
		}
	}
	else if ( iActvItemTypeID == 3022 )
	{
		iItemTypeListIndex_30R_R2 += iAddIndex;
		
		if (iItemTypeListIndex_30R_R2 < 1 )
		{
			iItemTypeListIndex_30R_R2 = 1;
		}
		else if ( iItemTypeListIndex_30R_R2 > vecCurves30R_R2.size()-1 )
		{
			iItemTypeListIndex_30R_R2 = vecCurves30R_R2.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves30R_R2[iItemTypeListIndex_30R_R2] ));
			bUpdateLastCreatedTP = true;
		}
	}
	else if ( iActvItemTypeID == 3023 )
	{
		iItemTypeListIndex_30R_R3 += iAddIndex;
		
		if (iItemTypeListIndex_30R_R3 < 1 )
		{
			iItemTypeListIndex_30R_R3 = 1;
		}
		else if ( iItemTypeListIndex_30R_R3 > vecCurves30R_R3.size()-1 )
		{
			iItemTypeListIndex_30R_R3 = vecCurves30R_R3.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves30R_R3[iItemTypeListIndex_30R_R3] ));
			bUpdateLastCreatedTP = true;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if ( iActvItemTypeID == 601 )
	{
		iItemTypeListIndex_60L += iAddIndex;
		
		if (iItemTypeListIndex_60L < 1 )
		{
			iItemTypeListIndex_60L = 1;
		}
		else if ( iItemTypeListIndex_60L > vecCurves60L.size()-1 )
		{
			iItemTypeListIndex_60L = vecCurves60L.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			// pruefen, ob ein 2. TP erstellt werden muss (z.B. fuer LaneChangeCurve )
			iNumberOfNewCreatedTP = _addTP();
			
			//T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves60L[iItemTypeListIndex_60L] ));
			bUpdateLastCreatedTP = true;
		}
	}
	else if ( iActvItemTypeID == 602 )
	{
		iItemTypeListIndex_60R += iAddIndex;
		
		if (iItemTypeListIndex_60R < 1 )
		{
			iItemTypeListIndex_60R = 1;
		}
		else if ( iItemTypeListIndex_60R > vecCurves60R.size()-1 )
		{
			iItemTypeListIndex_60R = vecCurves60R.size()-1;
		}
		else
		{
			// ptrLastCreatedTP loeschen
			_removeAndDestroyLastItemCreated();
		
			// pruefen, ob ein 2. TP erstellt werden muss (z.B. fuer LaneChangeCurve )
			iNumberOfNewCreatedTP = _addTP();
			
			//T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, vecCurves60R[iItemTypeListIndex_60R] ));
			bUpdateLastCreatedTP = true;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if ( bUpdateLastCreatedTP )
	{
		ptrLastCreatedTP = T[0]->S[iActvStrand]->TP[ T[0]->S[iActvStrand]->TP.size()-1 ];
		
		int iLiftIndex = T[0]->S[iActvStrand]->TP.size() - iNumberOfNewCreatedTP;
		ptrLiftNode = T[0]->S[iActvStrand]->TP[iLiftIndex]->getStartNodePtr();
		ptrLiftNode->translate( 0, fLiftValue, 0, Ogre::Node::TS_LOCAL );
				
		//ptrLastCreatedTP->setCreateModeMaterial();

		if ( ptrLastCreatedTP->get_TPID() == 10022 )
		{
			// PitOut nach rechts versetzen
			ptrLastCreatedTP->set_Stowing( -9.9f );
		}

		// add Pitlane to PitIn / PitOut
		if ( ptrLastCreatedTP->get_TPID() == 10021 || ptrLastCreatedTP->get_TPID() == 10022 )
		{
			ptrLastCreatedTP->add_PitLaneLeft();
		}

		_checkAutoAdd_shoulders( iNumberOfNewCreatedTP );
		appPtr->bUpdateTP_Pins = true;
		//T[0]->S[iActvStrand]->updateTPConnectors();
	}

	return;
}

void cTrackItemMngr::_checkAutoAdd_shoulders( int iNumberOfNewCreatedTP )
{
	if ( ptrLastCreatedTP->get_TPID() != 3012 && ptrLastCreatedTP->get_TPID() != 3022 )
	{
		unsigned int iIndex = T[0]->S[iActvStrand]->TP.size()-1;

		for ( int i=iNumberOfNewCreatedTP; i>=1; i-- )
		{
			if ( bAutoAdd_SH_L )
				T[0]->S[iActvStrand]->TP[iIndex]->add_shoulder_Left();
			if ( bAutoAdd_SH_R )
				T[0]->S[iActvStrand]->TP[iIndex]->add_shoulder_Right();

			iIndex--;
		}
	}
	
	return;
}

void cTrackItemMngr::_moveAndRotateStrand( const OIS::MouseEvent &arg )
{
	ptrActvItemNode->_setDerivedPosition( appPtr->c2Dto3DMousePositionMngr1->get3DMouseCoordinate( arg.state.X.abs, arg.state.Y.abs ));
	ptrCompassCenterNode->_setDerivedPosition( ptrActvItemNode->_getDerivedPosition() );

	if ( arg.state.Z.rel != 0 )
	{
		float fRotate = ptrActvItemNode->getOrientation().getYaw().valueDegrees();
		ptrActvItemNode->setOrientation( 1, 0, 0, 0 );

		if ( fRotate <= 0.0f )
			fRotate += 360.0f;

		float fRotationValue = 15.0f;
		if ( gl_bKeyPressed_LShift || gl_bKeyPressed_RShift )
		{
			fRotationValue = 5.0f;
		}

		if ( arg.state.Z.rel < 0 )
		{
			fRotate = fRotate + fRotationValue + 0.2f;
			int iFullRotation = fRotate/fRotationValue;
			fRotate = fRotationValue*iFullRotation;
			
			// um x Grad im Uhrzeigersinn drehen
			ptrActvItemNode->yaw( Ogre::Degree( fRotate ), Ogre::Node::TS_LOCAL );
		}
		else // if ( arg.state.Z.abs > 0 )
		{
			int iFullRotation = (fRotate-0.2)/fRotationValue;
			fRotate = fRotationValue*iFullRotation;

			// um x Grad im Uhrzeigersinn drehen
			ptrActvItemNode->yaw( Ogre::Degree( fRotate ), Ogre::Node::TS_LOCAL );
		}
	}

	return;
}

void cTrackItemMngr::showGizmoForSelectionBuffer( bool bShowGizmo )
{
	if ( bShowGizmo )
	{
		// Alle Track-Entitys ausblenden
		nMainTrackNodeParent->removeChild( T[0]->MainTrackNode );
			
		// EditModeGizmo fuer SelectionBuffer sichtbar machen
		nGizmoPositionNode->attachObject( eStraights );
		//nGizmoPositionNode->attachObject( eCurvesLeft );
		//nGizmoPositionNode->attachObject( eCurvesRight );

		nGizmoPositionNode->attachObject( eCurves15L );
		//nGizmoPositionNode->attachObject( eCurves30L );
		nGizmoPositionNode->attachObject( eCurves30L_R1 );
		nGizmoPositionNode->attachObject( eCurves30L_R2 );
		nGizmoPositionNode->attachObject( eCurves30L_R3 );

		nGizmoPositionNode->attachObject( eCurves60L );

		nGizmoPositionNode->attachObject( eCurves15R );
		//nGizmoPositionNode->attachObject( eCurves30R );
		nGizmoPositionNode->attachObject( eCurves30R_R1 );
		nGizmoPositionNode->attachObject( eCurves30R_R2 );
		nGizmoPositionNode->attachObject( eCurves30R_R3 );
		
		nGizmoPositionNode->attachObject( eCurves60R );
	}
	else
	{
		// Alle Track-Entitys einblenden
		nMainTrackNodeParent->addChild( T[0]->MainTrackNode );
			
		// EditModeGizmo fuer SelectionBuffer ausblenden
		nGizmoPositionNode->detachAllObjects();
	}
	
	return;
}

void cTrackItemMngr::cancelCurrentAction(void)
{
	// Diese Funktion wird aufgerufen, wenn die Maus ausserhalb des mainWindow geklickt wurde.
	
	if ( eTrackItemMngrMode == TIMM_OFF )
	{
		
	}
	else if ( eTrackItemMngrMode == TIMM_START_NEW_STRAND )
	{
		// "START NEW STRAND" abbrechen und das zuletzt erstellte Teil loeschen
		_removeAndDestroyLastItemCreated();
		eTrackItemMngrMode = TIMM_OFF;
		appPtr->eMouseState_Mode = appPtr->MMODE_OFF;

		// Compass ausblenden
		ptrCompassCenterNode->getParentSceneNode()->removeChild( ptrCompassCenterNode );
		ptrCompassArrowNode->getParentSceneNode()->removeChild( ptrCompassArrowNode );
	}
	else if ( eTrackItemMngrMode == TIMM_ADD_NEXT_TRACK_ITEM )
	{
		// das zuletzt erstellte Teil loeschen
		_removeAndDestroyLastItemCreated();
		bGizmoIsActiv = false;
		eTrackItemMngrMode = TIMM_OFF;
		appPtr->eMouseState_Mode = appPtr->MMODE_OFF;
	}

	// LMB & RMB auschalten
	bTIM_LMB_isDown = false;
	bTIM_RMB_isDown = false;
	appPtr->hide_toolTipInfoPanel();
	appPtr->cRttShadowPlane1->setRTTShadowVisible(true);
	gl_bUpdateShadowTex = true;
	appPtr->bUpdateTP_Pins = true;
	//T[0]->S[iActvStrand]->updateTPConnectors();
	bTIM_colorBlink_ON = false;
	
	return;
}

void cTrackItemMngr::_setLastTPOfStrandActiv(void)
{
	if ( !T[0]->S[iActvStrand]->TP.empty() )
	{
		int iLastIndex = T[0]->S[iActvStrand]->TP.size()-1;

		ptrLastCreatedTP = T[0]->S[iActvStrand]->TP[ iLastIndex ];
		
		// Gizmo an zuletzt erstellten Entity ausrichten
		nGizmoPositionNode->_setDerivedPosition( ptrLastCreatedTP->getStartNodePtr()->_getDerivedPosition() );
		nGizmoPositionNode->_setDerivedOrientation( ptrLastCreatedTP->getStartNodePtr()->_getDerivedOrientation() );

		appPtr->bUpdateTP_Pins = true;
	}
	else
	{
		// cancel
		cancelCurrentAction();
	}
		
	return;
}

void cTrackItemMngr::_createHairpinCurve(int iDirection)
{
	if ( iDirection == 1 )
	{
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 3011 ));
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 36011 ));
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 36021 ));
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 36031 ));
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 3012 ));
	}
	else // if ( iDirection == 2 )
	{
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 3021 ));
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 36012 ));
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 36022 ));
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 36032 ));
		T[0]->S[iActvStrand]->TP.push_back(new cTP( gl_sSystemName, 3022 ));
	}
	
	// Randstreifen hinzufuegen
	int iAcTP = T[0]->S[iActvStrand]->TP.size()-1;
	if ( iDirection == 1 )
	{
		T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Right();
		
		if ( bAutoAdd_SH_L )
			T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Left();
		else
			T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulderEnd_atPosition(1);
	}
	else // if ( iDirection == 2 )
	{
		T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Left();
		
		if ( bAutoAdd_SH_R )
			T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Right();
		else
			T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulderEnd_atPosition(2);
	}

	iAcTP--;
	T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Left();
	T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Right();
	
	iAcTP--;
	T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Left();
	T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Right();
	
	iAcTP--;
	T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Left();
	T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Right();
	
	iAcTP--;
	if ( iDirection == 1 )
	{
		T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Right();
		
		if ( bAutoAdd_SH_L )
			T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Left();
		else
			T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulderStart_atPosition(3);
	}
	else // if ( iDirection == 2 )
	{
		T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Left();
		
		if ( bAutoAdd_SH_R)
			T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulder_Right();
		else
			T[0]->S[iActvStrand]->TP[iAcTP]->add_shoulderStart_atPosition(4);
	}

	return;
}

void cTrackItemMngr::equalizeTrack( int iLiftValue )
{
	// Test
	int iIndex = T[0]->S[1]->TP.size()-1;
	float fLift = 0.1f;		// min=/*0.001f*/

	if ( iLiftValue < 0 )
		fLift *=-1;

	fEqualizeTrack += fLift;
	if ( fEqualizeTrack < 0 )
	{
		fEqualizeTrack = 0.0f;
	}
	else
	{
		for ( int i=iIndex; i>=0; i-- )
		{
			T[0]->S[1]->TP[i]->getStartNodePtr()->translate( 0, fLift, 0, Ogre::Node::TS_LOCAL );
		}
	}
	
	return;
}

void cTrackItemMngr::_colorBlink( float fTimeSinceLastFrame )
{
	if ( iBlink_Rcv >= 160.0f )
		bColorUp = false;
	else if ( iBlink_Rcv <= 10.0f )
		bColorUp = true;

	float iIncrease = 150.0f * fTimeSinceLastFrame;
	if ( iIncrease < 0.5f )
		iIncrease = iIncrease;

	if ( !bColorUp )
		iIncrease *= -1;

	iBlink_Rcv += iIncrease;
	iBlink_Gcv += iIncrease;

	/*iBlink_Acv += 0.1f;
	if ( iBlink_Acv > 200.0f )
		iBlink_Acv = 0.1f;*/
		
	Ogre::ColourValue cvCreateMode(iBlink_Rcv*fBlink_RGBmulti, iBlink_Gcv*fBlink_RGBmulti, iBlink_Bcv*fBlink_RGBmulti, iBlink_Acv);
	
	//int iNumberOfBlinkItems = _getMultipartCount();
	int iLastTPIndex = T[0]->S[iActvStrand]->TP.size()-1;
	std::deque<Ogre::MaterialPtr> deqMatPtr;


	for ( int i=_getMultipartCount(); i>=1; i-- )
	{
		deqMatPtr.push_front( T[0]->S[iActvStrand]->TP[iLastTPIndex]->deqMainEntity[0]->getSubEntity(0)->getMaterial()->clone( "_createModeMaterial_" + Ogre::StringConverter::toString( i )) );
		deqMatPtr[0]->getTechnique(0)->getPass(0)->setEmissive( cvCreateMode );
		deqMatPtr[0]->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_TRANSPARENT_COLOUR /*SBT_TRANSPARENT_ALPHA*/ );
		T[0]->S[iActvStrand]->TP[iLastTPIndex]->deqMainEntity[0]->getSubEntity(0)->setMaterial( deqMatPtr[0] );
		iLastTPIndex--;
	}

	while ( !deqMatPtr.empty() )
	{
		deqMatPtr[0].setNull();
		deqMatPtr.pop_front();
	}


	//Ogre::MaterialPtr matEditMode = ptrLastCreatedTP->deqMainEntity[0]->getSubEntity(0)->getMaterial()->clone( "_createModeMaterial" );
	//matEditMode->getTechnique(0)->getPass(0)->setEmissive( cvCreateMode );
	
	//matEditMode->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_TRANSPARENT_COLOUR /*SBT_TRANSPARENT_ALPHA*/ );
	//matEditMode->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	/*Ogre::TextureUnitState *t = matEditMode->getTechnique(0)->getPass(0)->createTextureUnitState();
	
	float alphaLevel = 0.5f;
	t->setAlphaOperation(LBX_SOURCE1, LBS_MANUAL, LBS_CURRENT, alphaLevel);*/
	
		
	//Ogre::Technique *bestTech = matEditMode->getBestTechnique();
		
	//ptrLastCreatedTP->deqMainEntity[0]->getSubEntity(0)->setMaterial( matEditMode );

	//matEditMode.setNull();

	return;
}

// initialize
void cTrackItemMngr::_createTypeListings_D124(void)
{
	sInsertItemType = "entAddItem_0";
	
	// Index0
	vecStraights.push_back( 0 );
	
	// Startampeln
	vecStraights.push_back( 305 );
	vecStraights.push_back( 306 );
	vecStraights.push_back( 304 );
	vecStraights.push_back( 303 );
	vecStraights.push_back( 301 );
	vecStraights.push_back( 302 );
	
	vecStraights.push_back( 201 );		// ControlUnit
	vecStraights.push_back( 35 );		// S3_starter4

	vecStraights.push_back( 10022 );	// PitOut
	vecStraights.push_back( 10021 );	// PitIn

	vecStraights.push_back( 1010 );		// CheckLane

	// StandardStraights
	vecStraights.push_back( 1 );		// S1
	vecStraights.push_back( 2 );		// S2
	vecStraights.push_back( 3 );		// S3
	iItemTypeListIndex_0 = iDefaultItemTypeListIndex_0 = vecStraights.size()-1;
	
	// SingleStraights
	vecStraights.push_back( 4 );		// create_singleStraight_L
	vecStraights.push_back( 6 );		// create_singleStraight_Sensor_L
	vecStraights.push_back( 5 );		// create_singleStraight_R
	vecStraights.push_back( 7 );		// create_singleStraight_Sensor_R
	
	// DoubleStraights
	vecStraights.push_back( 4546 );		// DoubleLaneChangeSection
	vecStraights.push_back( 5012 );		// LaneChangeSectionLeft
	vecStraights.push_back( 5022 );		// LaneChangeSectionRight

	vecStraights.push_back( 1222 );		// create_1auf2rechts2
	vecStraights.push_back( 1212 );		// create_1auf2links2

	vecStraights.push_back( 2112 );		// create_2auf1links2
	vecStraights.push_back( 2122 );		// create_2auf1rechts2

	vecStraights.push_back( 502 );		// CrossingNew_Up
	vecStraights.push_back( 504 );		// CrossingNew_Down

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//vecCurves15L
	vecCurves15L.push_back( 0 );		// Index0
	vecCurves15L.push_back( 304151 );	// HBC
	vecCurves15L.push_back( 4151 );
	iItemTypeListIndex_15L = iDefaultItemTypeListIndex_15L = vecCurves15L.size()-1;

	//vecCurves15R
	vecCurves15R.push_back( 0 );		// Index0
	vecCurves15R.push_back( 304152 );	// HBC
	vecCurves15R.push_back( 4152 );
	iItemTypeListIndex_15R = iDefaultItemTypeListIndex_15R = vecCurves15R.size()-1;
	
	//vecCurves30L_R1
	vecCurves30L_R1.push_back( 0 );			// Index0
	vecCurves30L_R1.push_back( 301301 );	// HBC
	vecCurves30L_R1.push_back( 1301 );
	iItemTypeListIndex_30L_R1 = iDefaultItemTypeListIndex_30L_R1 = vecCurves30L_R1.size()-1;

	//vecCurves30L_R2
	vecCurves30L_R2.push_back( 0 );			// Index0
	vecCurves30L_R2.push_back( 302301 );	// HBC
	vecCurves30L_R2.push_back( 2301 );
	iItemTypeListIndex_30L_R2 = iDefaultItemTypeListIndex_30L_R2 = vecCurves30L_R2.size()-1;

	//vecCurves30L_R3
	vecCurves30L_R3.push_back( 0 );			// Index0
	vecCurves30L_R3.push_back( 303301 );	// HBC
	vecCurves30L_R3.push_back( 3301 );
	iItemTypeListIndex_30L_R3 = iDefaultItemTypeListIndex_30L_R3 = vecCurves30L_R3.size()-1;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	//vecCurves30R_R1
	vecCurves30R_R1.push_back( 0 );			// Index0
	vecCurves30R_R1.push_back( 301302 );	// HBC
	vecCurves30R_R1.push_back( 1302 );
	iItemTypeListIndex_30R_R1 = iDefaultItemTypeListIndex_30R_R1 = vecCurves30R_R1.size()-1;

	//vecCurves30R_R2
	vecCurves30R_R2.push_back( 0 );			// Index0
	vecCurves30R_R2.push_back( 302302 );	// HBC
	vecCurves30R_R2.push_back( 2302 );
	iItemTypeListIndex_30R_R2 = iDefaultItemTypeListIndex_30R_R2 = vecCurves30R_R2.size()-1;

	//vecCurves30R_R3
	vecCurves30R_R3.push_back( 0 );			// Index0
	vecCurves30R_R3.push_back( 303302 );	// HBC
	vecCurves30R_R3.push_back( 3302 );
	iItemTypeListIndex_30R_R3 = iDefaultItemTypeListIndex_30R_R3 = vecCurves30R_R3.size()-1;
	
	////vecCurves30L
	//vecCurves30L.push_back( 0 );		// Index0
	//vecCurves30L.push_back( 303301 );	// HBC
	//vecCurves30L.push_back( 302301 );	// HBC
	//vecCurves30L.push_back( 301301 );	// HBC
	//
	//vecCurves30L.push_back( 1301 );
	//iItemTypeListIndex_30L = iDefaultItemTypeListIndex_30L = vecCurves30L.size()-1;
	//vecCurves30L.push_back( 2301 );
	//vecCurves30L.push_back( 3301 );

	////vecCurves30R
	//vecCurves30R.push_back( 0 );		// Index0
	//vecCurves30R.push_back( 303302 );	// HBC
	//vecCurves30R.push_back( 302302 );	// HBC
	//vecCurves30R.push_back( 301302 );	// HBC

	//vecCurves30R.push_back( 1302 );
	//iItemTypeListIndex_30R = iDefaultItemTypeListIndex_30R = vecCurves30R.size()-1;
	//vecCurves30R.push_back( 2302 );
	//vecCurves30R.push_back( 3302 );

	//vecCurves60L
	vecCurves60L.push_back( 0 );		// Index0
	vecCurves60L.push_back( 1601 );
	iItemTypeListIndex_60L = iDefaultItemTypeListIndex_60L = vecCurves60L.size()-1;
	vecCurves60L.push_back( 11601 );	// LaneChangeCurve AI
	vecCurves60L.push_back( 31601 );	// LaneChangeCurve IA

	vecCurves60L.push_back( 3012 );		// Hairpin_L
	
	//vecCurves60R
	vecCurves60R.push_back( 0 );		// Index0
	vecCurves60R.push_back( 1602 );
	iItemTypeListIndex_60R = iDefaultItemTypeListIndex_60R = vecCurves60R.size()-1;
	vecCurves60R.push_back( 21602 );	// LaneChangeCurve AI
	vecCurves60R.push_back( 41602 );	// LaneChangeCurve IA
	
	vecCurves60R.push_back( 3022 );		// Hairpin_R
	
	
	
	
	
	// HighBankedCurves
	//vecCurvesL.push_back( 304151 );
	//vecCurvesL.push_back( 303301 );
	//vecCurvesL.push_back( 302301 );
	//vecCurvesL.push_back( 301301 );
	
	// LaneChangeCurves
	//vecCurvesL.push_back( 11601 );		// Kurventeil - aussen nach innen
	//vecCurvesL.push_back( 31601 );		// Kurventeil - innen nach aussen
	
	// flat Curves
	//vecCurvesL.push_back( 1301 );
	//vecCurvesL.push_back( 1601 );
	//iItemTypeListIndex_1 = iDefaultItemTypeListIndex_1 = vecCurvesL.size()-1;
	//vecCurvesL.push_back( 2301 );
	//vecCurvesL.push_back( 3301 );
	//vecCurvesL.push_back( 4151 );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//vecCurvesR
	//vecCurvesR.push_back( 0 );		// Index0
	
	// HighBankedCurves
	//vecCurvesR.push_back( 304152 );
	//vecCurvesR.push_back( 301302 );
	//vecCurvesR.push_back( 302302 );
	//vecCurvesR.push_back( 303302 );

	// LaneChangeCurves
	//vecCurvesR.push_back( 21602 );		// Kurventeil - aussen nach innen
	//vecCurvesR.push_back( 41602 );		// Kurventeil - innen nach aussen
			
	// flat Curves
	//vecCurvesR.push_back( 1302 );
	//vecCurvesR.push_back( 1602 );
	//iItemTypeListIndex_2 = iDefaultItemTypeListIndex_2 = vecCurvesR.size()-1;
	//vecCurvesR.push_back( 2302 );
	//vecCurvesR.push_back( 3302 );
	//vecCurvesR.push_back( 4152 );
				
	return;
}

