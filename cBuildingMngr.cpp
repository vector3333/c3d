#include "cBuildingMngr.h"


cBuildingMngr::cBuildingMngr(void)
{
	eBuildingMngrMode = BMM_OFF;

	iActvStackID = 0;
	ptrActvBldNode = NULL;
}

cBuildingMngr::~cBuildingMngr(void)
{
}

void cBuildingMngr::mouseMoved( const OIS::MouseEvent &arg )
{
	if ( eBuildingMngrMode == BMM_CREATE_NEW_BLD )
	{
		// neuen Strang positionieren und Startrichtung festlegen (drehen)
		_moveAndRotateBuildingStack( arg );
	}
	else if ( eBuildingMngrMode == BMM_ADD_FLOOR )
	{
		// Bld-Type mit Mausrad waehlen
		_toggleFloorBuildingType( arg.state.Z.rel );
	}
	else if ( eBuildingMngrMode == BMM_ROTATE_BUILDING )
	{
		_rotateBuilding( arg.state.Z.rel );
	}
	else if ( eBuildingMngrMode == BMM_MOVE_AND_ROTATE_BUILDING )
	{
		_moveAndRotateBuildingStack( arg );
	}
	else if ( eBuildingMngrMode == BMM_CHANGE_BLD_HEIGHT )
	{
		_changeBuildingHeight( arg );
	}
	
	return;
}
void cBuildingMngr::mousePressed( OIS::MouseButtonID id )
{
	return;
}
void cBuildingMngr::mouseReleased( OIS::MouseButtonID mbReleased )
{
	if ( eBuildingMngrMode == BMM_CREATE_NEW_BLD )
	{
		if ( mbReleased == OIS::MB_Left )
		{
			// Neues Gebaeude uebernehmen und Modus beenden
			_stopCreateNewBuilding(true);
		}
		else if ( mbReleased == OIS::MB_Right )
		{
			// Neues Gebaeude loeschen und Modus beenden
			_stopCreateNewBuilding(false);
		}
	}
	else if ( eBuildingMngrMode == BMM_ADD_FLOOR )
	{
		if ( mbReleased == OIS::MB_Left )
		{
			// Neue Etage uebernehmen und Modus beenden
			_stopAddFloor(true);
		}
		else if ( mbReleased == OIS::MB_Right )
		{
			// Neue Etage loeschen und Modus beenden
			_stopAddFloor(false);
		}
	}
	else if ( eBuildingMngrMode == BMM_ROTATE_BUILDING )
	{
		if ( mbReleased == OIS::MB_Left )
		{
			// Neues Gebaeude uebernehmen und Modus beenden
			_stopRotateBuilding(true);
		}
		else if ( mbReleased == OIS::MB_Right )
		{
			// Neues Gebaeude loeschen und Modus beenden
			_stopRotateBuilding(false);
		}
	}
	else if ( eBuildingMngrMode == BMM_MOVE_AND_ROTATE_BUILDING )
	{
		if ( mbReleased == OIS::MB_Left )
		{
			// Neues Gebaeude uebernehmen und Modus beenden
			_stopMoveAndRotateBuilding(true);
		}
		else if ( mbReleased == OIS::MB_Right )
		{
			// Neues Gebaeude loeschen und Modus beenden
			_stopMoveAndRotateBuilding(false);
		}
	}
	else if ( eBuildingMngrMode == BMM_CHANGE_BLD_HEIGHT )
	{
		if ( mbReleased == OIS::MB_Left )
		{
			// Modus beenden und Aenderungen uebernehmen
			_stopChangeBuildingHeight(true);
		}
		else if ( mbReleased == OIS::MB_Right )
		{
			// Modus beenden und Aenderungen verwerfen
			_stopChangeBuildingHeight(false);
		}
	}
	
	return;
}

void cBuildingMngr::createNewBuilding( std::string sBldToCreate )
{
	int iBldID = 0;
	
	if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_atPit_box" )
	{
		iBldID = 1;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_atPit_vipLounge" )
	{
		iBldID = 2;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_atPit_presstower" )
	{
		iBldID = 33;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_onTrack_CtrlTower")
	{
		iBldID = 14;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_onTrack_grandstand" )
	{
		iBldID = 5;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_onTrack_footbridge1" )
	{
		iBldID = 7;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_onTrack_footbridge2" )
	{
		iBldID = 8;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_onTrack_footbridge3" )
	{
		iBldID = 9;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_onTrack_footbridge4" )
	{
		iBldID = 10;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_equipment_positionTower" )
	{
		iBldID = 11;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_equipment_mobilTower" )
	{
		iBldID = 12121;
	}
	else if ( sBldToCreate == "createNewObjectPopUp_itmCreateNewBuilding_equipment_lapCounter" )
	{
		iBldID = 13;
	}
	else
	{
		// Unknown _senderName
		return;
	}
		
	eBuildingMngrMode = BMM_CREATE_NEW_BLD;
	appPtr->eSelectedEntityType = appPtr->SELECTED_BLD;
	appPtr->textBoxSelectedEnt->setCaption( "SELECTED_BLD" );

	// Neuen Bld-Stack erzeugen
	iActvStackID = T[0]->cBldStackMngr->createNewBuildingStack(0, 0, 0, 0 );
	// Building erstellen
	T[0]->cBldStackMngr->addFloorToLastBuildingStack( iBldID, 0, 0, 0, 0 );

	ptrActvBldNode = T[0]->cBldStackMngr->getActiveStackBaseNodePtr();

	const OIS::MouseState mouseState = appPtr->mMouse->getMouseState();
	ptrActvBldNode->_setDerivedPosition( appPtr->c2Dto3DMousePositionMngr1->get3DMouseCoordinate( mouseState.X.abs, mouseState.Y.abs ));

	appPtr->show_toolTipInfoPanel( "createNewObject_building" );

	return;
}

void cBuildingMngr::_moveAndRotateBuildingStack( const OIS::MouseEvent &arg )
{
	ptrActvBldNode->_setDerivedPosition( appPtr->c2Dto3DMousePositionMngr1->get3DMouseCoordinate( arg.state.X.abs, arg.state.Y.abs ));

	if ( arg.state.Z.rel != 0 )
	{
		float fRotation = ptrActvBldNode->getOrientation().getYaw().valueDegrees();
		if ( fRotation <= 0 )
			fRotation += 360;
	
		ptrActvBldNode->setOrientation( 1, 0, 0, 0 );

		if ( arg.state.Z.rel < 0 )
		{
			fRotation += 15.2f;
			int iFullRotation = fRotation/15;

			fRotation = 15*iFullRotation;
		
			// um x Grad im Uhrzeigersinn drehen
			ptrActvBldNode->yaw( Ogre::Degree( fRotation ), Ogre::Node::TS_LOCAL );
		}
		else // if ( arg.state.Z.abs > 0 )
		{
			int iFullRotation = (fRotation-0.2)/15;

			fRotation = 15*iFullRotation;
		
			// um x Grad im Uhrzeigersinn drehen
			ptrActvBldNode->yaw( Ogre::Degree( fRotation ), Ogre::Node::TS_LOCAL );
		}
	}
	
	return;	
}

void cBuildingMngr::_stopCreateNewBuilding( bool bTakeChanges )
{
	if ( !bTakeChanges )
	{
		// Bld-Stack loeschen
		_removeAndDestroyLastCreatedBuildingStack();
	}
	else
	{
		gl_bAskForSaving = true;
		gl_bUpdateCamera = true;
		gl_bUpdateShadowTex = true;
		gl_bUpdateGridSize = true;
	}
	
	appPtr->hide_toolTipInfoPanel();
	eBuildingMngrMode = BMM_OFF;
	appPtr->eSelectedEntityType = appPtr->SELECTED_NONE;
	appPtr->eMouseState_Mode = appPtr->MMODE_OFF;

	return;
}


void cBuildingMngr::addFloor(void)
{
	appPtr->NEW_NEW_highlightMouseHoverEntityOFF();
	appPtr->NEW_NEW_deselectSelectedEntitys();

	eBuildingMngrMode = BMM_ADD_FLOOR;
	//gl_bMouseMode_toggleBuildingType = true;
	appPtr->eMouseState_Mode = appPtr->MMODE_BLD_MGR;
	appPtr->eSelectedEntityType = appPtr->SELECTED_BLD;
	appPtr->textBoxSelectedEnt->setCaption( "SELECTED_BLD" );
	
	if ( T[iLastObjectClicked_trackID]->cBldStackMngr->getBuildingTypeIDAsString() != "tribuneBase" )
		appPtr->show_toolTipInfoPanel( "editBldPopUpMenu_createNewFloor" );
	
	T[iLastObjectClicked_trackID]->cBldStackMngr->edit_createNewFloorAbove();
	
	return;
}


void cBuildingMngr::_toggleFloorBuildingType( int iMouseMoveZ )
{
	if ( iMouseMoveZ != 0 )
	{
		if ( iMouseMoveZ > 0 )
			T[0]->cBldStackMngr->toggleFloorBuildingType( false );
		else
			T[0]->cBldStackMngr->toggleFloorBuildingType( true );
	}
	
	return;
}


void cBuildingMngr::_stopAddFloor( bool bTakeChanges )
{
	if ( !bTakeChanges )
	{
		// Bld-Stack loeschen
		removeSelectedFloor();
	}
	else
	{
		gl_bAskForSaving = true;
		gl_bUpdateCamera = true;
		gl_bUpdateShadowTex = true;
		gl_bUpdateGridSize = true;
	}

	appPtr->NEW_NEW_deselectSelectedEntitys();
	appPtr->hide_toolTipInfoPanel();
	
	eBuildingMngrMode = BMM_OFF;
	appPtr->eSelectedEntityType = appPtr->SELECTED_NONE;
	appPtr->eMouseState_Mode = appPtr->MMODE_OFF;

	//gl_bUpdateCamera = true;
	//gl_bUpdateShadowTex = true;
	//gl_bUpdateGridSize = true;
	
	return;
}


void cBuildingMngr::removeSelectedFloor(void)
{
	if ( !T[iHoverTrack]->cBldStackMngr->getIsLastOfStack() )
	{
		int iActvFloorID = T[iHoverTrack]->cBldStackMngr->getActvFloor();
		float fRotateUpperFloor = 0.0f;
		fRotateUpperFloor += T[iHoverTrack]->cBldStackMngr->deqBuildingsPtr[iHoverStrand]->at(iActvFloorID)->getBaseNodePtr()->getOrientation().getYaw().valueDegrees();
		T[iHoverTrack]->cBldStackMngr->deqBuildingsPtr[iHoverStrand]->at(iActvFloorID + 1)->getBaseNodePtr()->yaw(Ogre::Degree(fRotateUpperFloor));
	}
	
	appPtr->NEW_NEW_deselectSelectedEntitys();
	
	T[0]->cBldStackMngr->edit_removeSelectedFloor();

	gl_bAskForSaving = true;
	gl_bUpdateCamera = true;
	gl_bUpdateShadowTex = true;
	gl_bUpdateGridSize = true;

	return;
}


void cBuildingMngr::_removeAndDestroyLastCreatedBuildingStack(void)
{
	T[0]->cBldStackMngr->deleteCompleteStack( iActvStackID );
	
	return;
}


void cBuildingMngr::startMoveAndRotateBuilding(void)
{
	eBuildingMngrMode = BMM_MOVE_AND_ROTATE_BUILDING;
	
	appPtr->eSelectedEntityType = appPtr->SELECTED_BLD;
	appPtr->textBoxSelectedEnt->setCaption( "SELECTED_BLD" );
	
	ptrActvBldNode = T[0]->cBldStackMngr->getActiveStackBaseNodePtr();
	//ptrActvBldNode->showBoundingBox(true);
	
	// Aktuelle Position und Rotation des ausgewaehlten Buildings merken
	v3RememberLastBuildingStackPosition = ptrActvBldNode->_getDerivedPosition();
	quatRememberLastBuildingRotation = ptrActvBldNode->_getDerivedOrientation();

	appPtr->show_toolTipInfoPanel( "editBldPopUpMenu_moveAndRotateExistingObject" );
	
	return;
}


void cBuildingMngr::_moveAndRotateBuilding( const OIS::MouseEvent &arg )
{
	ptrActvBldNode->_setDerivedPosition( appPtr->c2Dto3DMousePositionMngr1->get3DMouseCoordinate( arg.state.X.abs, arg.state.Y.abs ));
	ptrActvBldNode->translate( 0, v3RememberLastBuildingStackPosition.y , 0, Ogre::Node::TS_LOCAL );

	_rotateBuilding( arg.state.Z.rel );
	
	return;
}


void cBuildingMngr::_stopMoveAndRotateBuilding( bool bTakeChanges )
{
	if ( !bTakeChanges )
	{
		// Position/Rotation verwerfen
		ptrActvBldNode->_setDerivedPosition( v3RememberLastBuildingStackPosition );
		ptrActvBldNode->_setDerivedOrientation( quatRememberLastBuildingRotation );
	}
	else
	{
		gl_bAskForSaving = true;
		gl_bUpdateCamera = true;
		gl_bUpdateShadowTex = true;
		gl_bUpdateGridSize = true;
	}

	//ptrActvBldNode->showBoundingBox(false);
	
	appPtr->NEW_NEW_deselectSelectedEntitys();
	appPtr->hide_toolTipInfoPanel();
	
	eBuildingMngrMode = BMM_OFF;
	appPtr->eSelectedEntityType = appPtr->SELECTED_NONE;
	appPtr->eMouseState_Mode = appPtr->MMODE_OFF;
	
	return;
}


void cBuildingMngr::startRotateBuilding(void)
{
	eBuildingMngrMode = BMM_ROTATE_BUILDING;
	
	appPtr->NEW_NEW_highlightMouseHoverEntityOFF();
	appPtr->NEW_NEW_deselectSelectedEntitys();
	
	T[iHoverTrack]->cBldStackMngr->setStateSelected(true);
	appPtr->show_toolTipInfoPanel( "editBldPopUpMenu_rotate" );
	
	int iFloorID = T[iHoverTrack]->cBldStackMngr->getFloorID_searchBldCounterID(iHoverStrand, iHoverID);

	if (iFloorID > 0 )
	{
		std::string sBldType = T[iHoverTrack]->cBldStackMngr->deqBuildingsPtr[iHoverStrand]->at(iFloorID-1)->getBuildingTypeID_asString();

		// wenn das Bld ein Oval ist, pruefen, ob eine Erweiterung (Balkon) darunter ist
		if (sBldType == "presstowerExt")
			iFloorID--;
		else if (sBldType == "mTowerBase")
			iFloorID--; 
		else if (sBldType == "tribuneBase")
		{
			while (T[iHoverTrack]->cBldStackMngr->deqBuildingsPtr[iHoverStrand]->at(iFloorID-1)->getBuildingTypeID_asString() == "tribuneBase")
			{
				iFloorID--;

				if (iFloorID == 0)
					break;
			}
		}
	}
	ptrActvBldNode = T[iHoverTrack]->cBldStackMngr->deqBuildingsPtr[iHoverStrand]->at(iFloorID)->getBaseNodePtr();
	
	// Aktuelle Rotation des ausgewaehlten Buildings merken
	quatRememberLastBuildingRotation = ptrActvBldNode->_getDerivedOrientation();
		
	return;
}


void cBuildingMngr::_rotateBuilding( int iMouseMoveZ )
{
	if ( iMouseMoveZ != 0 )
	{
		float fRotation = ptrActvBldNode->getOrientation().getYaw().valueDegrees();
		if ( fRotation <= 0 )
			fRotation += 360;
	
		ptrActvBldNode->setOrientation( 1, 0, 0, 0 );

		if ( iMouseMoveZ < 0 )
		{
			fRotation += 15.2f;
			int iFullRotation = fRotation/15;

			fRotation = 15*iFullRotation;
		
			// um x Grad im Uhrzeigersinn drehen
			ptrActvBldNode->yaw( Ogre::Degree( fRotation ), Ogre::Node::TS_LOCAL );
		}
		else // if ( iMouseMoveZ > 0 )
		{
			int iFullRotation = (fRotation-0.2)/15;

			fRotation = 15*iFullRotation;
		
			// um x Grad im Uhrzeigersinn drehen
			ptrActvBldNode->yaw( Ogre::Degree( fRotation ), Ogre::Node::TS_LOCAL );
		}
	}

	return;
}


void cBuildingMngr::_stopRotateBuilding( bool bTakeChanges )
{
	if ( !bTakeChanges )
	{
		// Rotation verwerfen
		ptrActvBldNode->_setDerivedOrientation( quatRememberLastBuildingRotation );
	}
	else
	{
		gl_bAskForSaving = true;
		gl_bUpdateCamera = true;
		gl_bUpdateShadowTex = true;
		gl_bUpdateGridSize = true;
	}

	appPtr->NEW_NEW_deselectSelectedEntitys();
	appPtr->hide_toolTipInfoPanel();
	
	eBuildingMngrMode = BMM_OFF;
	appPtr->eSelectedEntityType = appPtr->SELECTED_NONE;
	appPtr->eMouseState_Mode = appPtr->MMODE_OFF;
	
	return;
}


void cBuildingMngr::startChangeBuildingHeight(void)
{
	eBuildingMngrMode = BMM_CHANGE_BLD_HEIGHT;
	
	appPtr->NEW_NEW_highlightMouseHoverEntityOFF();
	appPtr->NEW_NEW_deselectSelectedEntitys();
	
	T[0]->cBldStackMngr->setStateSelected(true);

	ptrActvBldNode = T[0]->cBldStackMngr->getActiveStackBaseNodePtr();

	// Aktuelle Position merken
	v3RememberLastBuildingStackPosition = ptrActvBldNode->_getDerivedPosition();
	
	appPtr->show_toolTipInfoPanel( "editBldPopUpMenu_height" );
	
	return;
}


void cBuildingMngr::_changeBuildingHeight( const OIS::MouseEvent &arg )
{
	if ( arg.state.Z.rel != 0 )
	{
		// Mausrad wurde bewegt
		float fCurrentHeight = ptrActvBldNode->_getDerivedPosition().y;
		int iFullLevel = 0;
		
		if ( arg.state.Z.rel > 0 )
		{
			// Gebaeude auf naechste Ebene positionieren
			iFullLevel = (fCurrentHeight +0.1f) / gl_fLevelHeight;
			iFullLevel++;
		}
		else
		{
			// Gebaeude auf naechste Ebene positionieren
			iFullLevel = (fCurrentHeight -0.1f) / gl_fLevelHeight;

			if ( iFullLevel < 0 )
				iFullLevel = 0;
		}

		float fNewHeight = iFullLevel * gl_fLevelHeight;
		if ( fNewHeight > 200 )
			fNewHeight = 200;

		ptrActvBldNode->_setDerivedPosition( Ogre::Vector3( v3RememberLastBuildingStackPosition.x, fNewHeight, v3RememberLastBuildingStackPosition.z ));
	}
	
	return;
}


void cBuildingMngr::_stopChangeBuildingHeight( bool bTakeChanges )
{
	if ( !bTakeChanges )
	{
		// Rotation verwerfen
		ptrActvBldNode->_setDerivedPosition( v3RememberLastBuildingStackPosition );
	}
	else
	{
		gl_bAskForSaving = true;
		gl_bUpdateCamera = true;
		gl_bUpdateShadowTex = true;
		gl_bUpdateGridSize = true;
	}

	appPtr->NEW_NEW_deselectSelectedEntitys();
	appPtr->hide_toolTipInfoPanel();
	
	eBuildingMngrMode = BMM_OFF;
	appPtr->eSelectedEntityType = appPtr->SELECTED_NONE;
	appPtr->eMouseState_Mode = appPtr->MMODE_OFF;
	
	return;
}


void cBuildingMngr::cancelCurrentAction(void)
{
	// Diese Funktion wird aufgerufen, wenn die Maus ausserhalb des mainWindow geklickt wurde.
	
	if ( eBuildingMngrMode == BMM_CREATE_NEW_BLD )
	{
		_stopCreateNewBuilding(false);
	}
	else if ( eBuildingMngrMode == BMM_ADD_FLOOR )
	{
		_stopAddFloor(false);
	}
			
	return;
}