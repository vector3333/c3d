#include "cTwistContourDummy_TP.h"
#include "C3D.h"


cTwistContourDummy_TP::cTwistContourDummy_TP(int track, int strand, int tp, Ogre::String name, int iSegments)
{
	tpName = name + "twistDummy";
	iTrack = track;
	iStrand = strand;
	iTP = tp;

	fDummyTwist = 0;

	radiusEnt = NULL;
	centerEnt = NULL;

	radiusNodeTP = NULL;
	centerNodeTP = NULL;
	
	parentNodeTP = T[iTrack]->S[iStrand]->TP[iTP]->getPitchNodePtr();
	twistDummyStartNodeTP = parentNodeTP->createChildSceneNode();
	tpID = T[iTrack]->S[iStrand]->TP[iTP]->get_TPID();
	nSegs = iSegments;
	
	createDummy();

	#ifdef _DEBUG
		//createAndShowPoints();
	#endif

	allignSegmentDummys();
}

cTwistContourDummy_TP::~cTwistContourDummy_TP(void)
{
	if ( gl_bLogMessages_Level02 ) Ogre::LogManager::getSingleton().logMessage("~TCD_TP");
	
	while ( !vPoints.empty() )
	{
		appPtr->mSceneMgrWorld->destroyEntity( vPoints[vPoints.size()-1] );
		vPoints.pop_back();
	}

	if ( radiusEnt != NULL ) // radiusEnt wurde nur erstellt, wenn TP eine Kurve ist
	{
		appPtr->mSceneMgrWorld->getEntity( radiusEnt->getName() )->detachFromParent();
		appPtr->mSceneMgrWorld->destroyEntity( radiusEnt );
		appPtr->mSceneMgrWorld->getEntity( centerEnt->getName() )->detachFromParent();
		appPtr->mSceneMgrWorld->destroyEntity( centerEnt );
	}

	//twistDummyStartNodeTP->removeAndDestroyAllChildren();
	//parentNodeTP->removeAndDestroyChild( twistDummyStartNodeTP->getName() );
	
	if ( radiusNodeTP != NULL && appPtr->mSceneMgrWorld->hasSceneNode( radiusNodeTP->getName() ) )
		appPtr->mSceneMgrWorld->getSceneNode( radiusNodeTP->getName() )->getParentSceneNode()->removeAndDestroyChild( radiusNodeTP->getName() );
	
	/*if ( radiusNodeTP != NULL )
	{
		radiusNodeTP->removeAndDestroyChild( centerNodeTP->getName() );
		parentNodeTP->removeAndDestroyChild( radiusNodeTP->getName() );
	}*/

	while ( !vSegmentNodes_TP.empty() )
	{
		vSegmentNodes_TP[vSegmentNodes_TP.size()-1]->at(0)->removeAndDestroyAllChildren();
		vSegmentNodes_TP[vSegmentNodes_TP.size()-1]->at(0)->getParentSceneNode()->removeAndDestroyChild(vSegmentNodes_TP[vSegmentNodes_TP.size()-1]->at(0)->getName());
		
		vSegmentNodes_TP[vSegmentNodes_TP.size()-1]->at(0) = NULL;
		vSegmentNodes_TP[vSegmentNodes_TP.size()-1]->at(1) = NULL;
		vSegmentNodes_TP[vSegmentNodes_TP.size()-1]->at(2) = NULL;
		vSegmentNodes_TP[vSegmentNodes_TP.size()-1]->at(3) = NULL;
		vSegmentNodes_TP[vSegmentNodes_TP.size()-1]->at(4) = NULL;

		delete vSegmentNodes_TP[vSegmentNodes_TP.size()-1];
		vSegmentNodes_TP[vSegmentNodes_TP.size()-1] = NULL;
		
		vSegmentNodes_TP.pop_back();
	}
	
	parentNodeTP = NULL;
	twistDummyStartNodeTP = NULL;
	
	//appPtr->mSceneMgrWorld->getRootSceneNode()->removeAndDestroyChild( parentNodeTP->getName() );
}

void cTwistContourDummy_TP::setTwist(float fNewTwist)
{
	// + = rollen nach rechts ( im Uhrzeigersinn )
	// - = rollen nach links ( gegen Uhrzeigersinn )
	
	float twistPerSeg = -(fDummyTwist)/nSegs;
	
	// dummy auf twist 0 zurueck twisten
	if ( fDummyTwist != 0 )
	{
		float fTwistBack = 0;
		
		for ( unsigned int i=1; i<=vSegmentNodes_TP.size()-1; i++ )
		{
			fTwistBack += twistPerSeg;
			vSegmentNodes_TP[i]->at(0)->roll(Ogre::Degree(fTwistBack));
		}
	}
	
	//neuen Wert fuer twistDummy speichern
	fDummyTwist = fNewTwist;
	
	if ( fDummyTwist != 0 )
	{	
		// dummy auf neuen Wert twisten
		twistPerSeg = fDummyTwist/nSegs;

		// wenn TP eine Rechtskurve ist
		//if ( iSLR == 2 )
			//twistPerSeg *= -1;
	
		float fTwistNew = 0;
		
		for ( unsigned int i=1; i<=vSegmentNodes_TP.size()-1; i++ )
		{
			fTwistNew += twistPerSeg;
			vSegmentNodes_TP[i]->at(0)->roll(Ogre::Degree(fTwistNew));
		}
	}

	if ( gl_bLogMessages_Level02 )
		Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 01)");
		
	appPtr->mSceneMgrWorld->getRootSceneNode()->_update(true, false);

	return;
}

// getVertexPositions -->
//			SH_L		PL_L				TP				PL_R		SH_R
//		8-----------6-----------2-----------------------4-----------10----------12
//		|			|			|			x			|			|			|
//		7-----------5-----------1-----------------------3-----------9-----------11
//

Ogre::Vector3 cTwistContourDummy_TP::getVertexPosition(int seg, int vertx)
{
	if ( gl_bLogMessages_Level02 )	Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 02) 0");
		
	appPtr->mSceneMgrWorld->getRootSceneNode()->_update(true, false);
	
	if ( seg <= vSegmentNodes_TP.size()-1 )
	{
		if ( gl_bLogMessages_Level02 )	Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 02) 1");
		return vSegmentNodes_TP[seg]->at(vertx)->_getDerivedPosition() - parentNodeTP->_getDerivedPosition();
	}

	if ( gl_bLogMessages_Level02 )	Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 02) 2");
	
	return Ogre::Vector3(0, 0, 0);
}
Ogre::Vector3 cTwistContourDummy_TP::getVertexPosition_PitLaneLeft(int seg, int vertx)
{
	if ( gl_bLogMessages_Level02 )
		Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 02)");
		
	appPtr->mSceneMgrWorld->getRootSceneNode()->_update(true, false);
	
	int iVertexID = 0;

	switch ( vertx )
	{
		case 1:		iVertexID = 5;		break;
		case 2:		iVertexID = 6;		break;
		case 3:		iVertexID = 1;		break;
		case 4:		iVertexID = 2;		break;
	}
	
	if ( seg <= vSegmentNodes_TP.size()-1 )
	{
		return vSegmentNodes_TP[seg]->at(iVertexID)->_getDerivedPosition() - parentNodeTP->_getDerivedPosition();
	}
	
	return Ogre::Vector3(0, 0, 0);
}
Ogre::Vector3 cTwistContourDummy_TP::getVertexPosition_PitLaneRight(int seg, int vertx)
{
	if ( gl_bLogMessages_Level02 )
		Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 02)");
		
	appPtr->mSceneMgrWorld->getRootSceneNode()->_update(true, false);
	
	int iVertexID = 0;

	switch ( vertx )
	{
		case 1:		iVertexID = 3;		break;
		case 2:		iVertexID = 4;		break;
		case 3:		iVertexID = 9;		break;
		case 4:		iVertexID = 10;		break;
	}
	
	if ( seg <= vSegmentNodes_TP.size()-1 )
	{
		return vSegmentNodes_TP[seg]->at(iVertexID)->_getDerivedPosition() - parentNodeTP->_getDerivedPosition();
	}
	
	return Ogre::Vector3(0, 0, 0);
}
	
Ogre::Vector3 cTwistContourDummy_TP::getVertexPosition_ShoulderLeft(int seg, int vertx)
{
	if ( gl_bLogMessages_Level02 )
		Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 02)");
		
	appPtr->mSceneMgrWorld->getRootSceneNode()->_update(true, false);
	
	int iVertexID = 0;

	switch ( vertx )
	{
		case 1:		iVertexID = 7;		break;
		case 2:		iVertexID = 8;		break;
		case 3:		iVertexID = 5;		break;
		case 4:		iVertexID = 6;		break;
	}
	
	if ( seg <= vSegmentNodes_TP.size()-1 )
	{
		return vSegmentNodes_TP[seg]->at(iVertexID)->_getDerivedPosition() - parentNodeTP->_getDerivedPosition();
	}
	
	return Ogre::Vector3(0, 0, 0);
}
Ogre::Vector3 cTwistContourDummy_TP::getVertexPosition_ShoulderRight(int seg, int vertx)
{
	if ( gl_bLogMessages_Level02 )
		Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 02)");
		
	appPtr->mSceneMgrWorld->getRootSceneNode()->_update(true, false);
	
	int iVertexID = 0;

	switch ( vertx )
	{
		case 1:		iVertexID = 9;		break;
		case 2:		iVertexID = 10;		break;
		case 3:		iVertexID = 11;		break;
		case 4:		iVertexID = 12;		break;
	}
	
	if ( seg <= vSegmentNodes_TP.size()-1 )
	{
		return vSegmentNodes_TP[seg]->at(iVertexID)->_getDerivedPosition() - parentNodeTP->_getDerivedPosition();
	}
	
	return Ogre::Vector3(0, 0, 0);
}
// getVertexPositions <--

void cTwistContourDummy_TP::createDummy()
{
	// create 1 Profile/Segment
	
	//			SH_L		PL_L				TP				PL_R		SH_R
	//		8-----------6-----------2-----------------------4-----------10----------12
	//		|			|			|			x			|			|			|
	//		7-----------5-----------1-----------------------3-----------9-----------11
	//
	
	float fX = fCarreraTPWidth/2;
	float fY = fCarreraTPHeight/2;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SegmentNodes erstellen
	
	// vSegmentNodes_TP[Index]->at(0) = segCenterNode
	// vSegmentNodes_TP[Index]->at(1) = NodeVertex1
	// vSegmentNodes_TP[Index]->at(2) = NodeVertex2
	// vSegmentNodes_TP[Index]->at(3) = NodeVertex3
	// vSegmentNodes_TP[Index]->at(4) = NodeVertex4
		
	float fPL_offsetLeft = 0.0f;
	// wenn PitLaneLeft vorhanden ist
	if ( T[iTrack]->S[iStrand]->TP[iTP]->getIs_PLL() != "" )
		fPL_offsetLeft = 9.9f;

	float fPL_offsetRight = 0.0f;
	// wenn PitLaneRight vorhanden ist
	if ( T[iTrack]->S[iStrand]->TP[iTP]->getIs_PLR() != "" )
		fPL_offsetRight = -9.9f;

	// nur bei Geraden mit konischen Randstreifen Links
	float fSH_offsetLeft = 9.9f;
	float fSH_offsetLeftPerSeg = 0.0f;
	
	if ( T[iTrack]->S[iStrand]->TP[iTP]->get_Modul_L1() == "RS_breit_schmal" )
		fSH_offsetLeftPerSeg = -(9.9f/2/nSegs);
	else if ( T[iTrack]->S[iStrand]->TP[iTP]->get_Modul_L1() == "RS_schmal_breit" )
	{
		fSH_offsetLeft = 4.95f;
		fSH_offsetLeftPerSeg = 9.9f/2/nSegs;
	}

	// nur bei Geraden mit konischen Randstreifen Rechts
	float fSH_offsetRight = -9.9f;
	float fSH_offsetRightPerSeg = 0.0f;
	
	if ( T[iTrack]->S[iStrand]->TP[iTP]->get_Modul_R1() == "RS_breit_schmal" )
		fSH_offsetRightPerSeg = 9.9f/2/nSegs;
	else if ( T[iTrack]->S[iStrand]->TP[iTP]->get_Modul_R1() == "RS_schmal_breit" )
	{
		fSH_offsetRight = -4.95f;
		fSH_offsetRightPerSeg = -(9.9f/2/nSegs);
	}

	for ( unsigned int i=0; i<=nSegs; i++ )
	{
		// neues Segment erstellen
		vSegmentNodes_TP.push_back( new std::vector<Ogre::SceneNode*> );
		// Segment CenterNode
		vSegmentNodes_TP[i]->push_back( twistDummyStartNodeTP->createChildSceneNode() );
		// SegmentVertices 1-4
		vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(0)->createChildSceneNode(Ogre::Vector3( fX, -fY, 0.0f )) );
		vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(0)->createChildSceneNode(Ogre::Vector3( fX,  fY, 0.0f )) );
		vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(0)->createChildSceneNode(Ogre::Vector3(-fX, -fY, 0.0f )) );
		vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(0)->createChildSceneNode(Ogre::Vector3(-fX,  fY, 0.0f )) );

		if ( gl_sSystemName == "D124" )
		{
			// SegmentVertices PitLaneLeft
			vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(1)->createChildSceneNode(Ogre::Vector3( fPL_offsetLeft, 0.0f, 0.0f )) );
			vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(5)->createChildSceneNode(Ogre::Vector3( 0.0f,  fCarreraTPHeight, 0.0f )) );
				
			// SegmentVertices ShoulderLeft
			vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(5)->createChildSceneNode(Ogre::Vector3( fSH_offsetLeft, 0.0f, 0.0f )) );
			vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(7)->createChildSceneNode(Ogre::Vector3( 0.0f,  fCarreraTPHeight, 0.0f )) );

			fSH_offsetLeft += fSH_offsetLeftPerSeg;
			
			// SegmentVertices PitLaneRight
			vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(3)->createChildSceneNode(Ogre::Vector3( fPL_offsetRight, 0.0f, 0.0f )) );
			vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(9)->createChildSceneNode(Ogre::Vector3( 0.0f,  fCarreraTPHeight, 0.0f )) );

			// SegmentVertices ShoulderRight
			vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(9)->createChildSceneNode(Ogre::Vector3( fSH_offsetRight, 0.0f, 0.0f )) );
			vSegmentNodes_TP[i]->push_back( vSegmentNodes_TP[i]->at(11)->createChildSceneNode(Ogre::Vector3( 0.0f,  fCarreraTPHeight, 0.0f )) );

			fSH_offsetRight += fSH_offsetRightPerSeg;
		}
	}
	
	return;
}
void cTwistContourDummy_TP::allignSegmentDummys(void)
{
	float radius = T[iTrack]->S[iStrand]->TP[iTP]->getTPAttribute_radius();
	int iSLR = 0;

	// ist das TP eine Gerade, Links(+)- oder Rechtskurve(-) ? Erkennbar am TP-Radius
	if ( radius > 0 )
		iSLR = 1;
	else if ( radius < 0 )
		iSLR = 2;
	
	// wenn TP eine Gerade
	if ( iSLR == 0 )
	{
		float tpLength = T[iTrack]->S[iStrand]->TP[iTP]->getTPAttribute_length();
		float segDistance = tpLength/nSegs;
		float fMoveSegment = 0;
		
		for ( unsigned int i=1; i<=vSegmentNodes_TP.size()-1; i++ )
		{
			fMoveSegment += segDistance;
			vSegmentNodes_TP[i]->at(0)->translate( 0.0f, 0.0f, fMoveSegment, Ogre::Node::TS_LOCAL );
		}
	}
	// wenn TP eine Kurve
	else // if ( SLR != 0 )
	{
		// get TP Radius
		float degree = T[iTrack]->S[iStrand]->TP[iTP]->getTPAttribute_bend();
		float rotatePerSeg = degree/nSegs;

		//// create helperNodeTPs
		radiusNodeTP = parentNodeTP->createChildSceneNode( tpName + "_radiusNodeTP" );
		centerNodeTP = radiusNodeTP->createChildSceneNode( tpName + "_centerNodeTP" );
		
		radiusNodeTP->translate( radius, 0.0f, 0.0f );
		centerNodeTP->translate( -radius, 0.0f, 0.0f );

		// Points erstellen
		/*radiusEnt = appPtr->mSceneMgrWorld->createEntity( tpName + "radiusEnt", "point.mesh" );
		radiusNodeTP->attachObject( radiusEnt );
		radiusNodeTP->scale( 1,1,1 );
		centerEnt = appPtr->mSceneMgrWorld->createEntity( tpName + "centerEnt", "point.mesh" );
		centerNodeTP->attachObject( centerEnt );
		centerNodeTP->scale( 4,4,4 );*/
		
		if ( gl_bLogMessages_Level02 )
			Ogre::LogManager::getSingleton().logMessage("TCD->_update (TCTP 01)");
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		for ( unsigned int i=1; i<= vSegmentNodes_TP.size()-1; i++ )
		{
			radiusNodeTP->yaw(Ogre::Degree(rotatePerSeg));
			appPtr->mSceneMgrWorld->getRootSceneNode()->_update(true, false);

			vSegmentNodes_TP[i]->at(0)->_setDerivedPosition( centerNodeTP->_getDerivedPosition() );
			vSegmentNodes_TP[i]->at(0)->yaw(Ogre::Degree(rotatePerSeg*i));
		}
	}
	
	//appPtr->mSceneMgrWorld->getRootSceneNode()->_update(true, false);
	
	return;
}

void cTwistContourDummy_TP::createAndShowPoints(void)
{
	// dummy Points sichtbar machen

	// HelperNodeTPs
	/*if ( radiusNodeTP != NULL )
	{
		radiusEnt = appPtr->mSceneMgrWorld->createEntity( tpName + "radiusEnt", "point.mesh" );
		radiusNodeTP->attachObject( radiusEnt );
		radiusNodeTP->scale( 1,1,1 );
		centerEnt = appPtr->mSceneMgrWorld->createEntity( tpName + "centerEnt", "point.mesh" );
		centerNodeTP->attachObject( centerEnt );
		centerNodeTP->scale( 4,4,4 );
	}*/
	
	for ( unsigned int i=0; i<=vSegmentNodes_TP.size()-1; i++ )
	{
		for ( unsigned int j=1; j<=vSegmentNodes_TP[i]->size()-1; j++ )
		{
			vPoints.push_back(appPtr->mSceneMgrWorld->createEntity( "twistDummy_" + tpName + "_Seg_" + Ogre::StringConverter::toString(i) + "_Point_" + Ogre::StringConverter::toString(j), "point.mesh"));
			vSegmentNodes_TP[i]->at(j)->attachObject( vPoints[vPoints.size()-1] );
		}
	}
	
	return;
}
