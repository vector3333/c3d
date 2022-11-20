#include "c2Dto3DMousePositionMngr.h"
#include "C3D.h"


c2Dto3DMousePositionMngr::c2Dto3DMousePositionMngr()
{
	pMouseRayCamera = NULL;
	pNewCreatedObjectNode = NULL;

	sLastCreatedObject = "";

	iClickMousePosX = 0;
	iClickMousePosY = 0;

	fHeightOffsetY = 0;

	p2Dto3DPlane = new Ogre::MovablePlane(Vector3::UNIT_Y, 0);

	p2Dto3DPlaneNode = appPtr->mSceneMgrWorld->getRootSceneNode()->createChildSceneNode( "p2Dto3DPlaneNode" );
	p2Dto3DPlaneNode->attachObject( p2Dto3DPlane );

	p3DCoordinateEntity = appPtr->mSceneMgrWorld->createEntity("p3DCoordinateEntity", Ogre::SceneManager::PT_SPHERE);
	p3DCoordinateNode = appPtr->mSceneMgrWorld->getRootSceneNode()->createChildSceneNode( "p3DCoordinateNode" );
	
	Ogre::Real radius = p3DCoordinateEntity->getBoundingRadius(); 
	p3DCoordinateNode->scale( 0.01f, 0.01f, 0.01f );//50/radius,50/radius,50/radius);

	fLastBuildingCreatedWithRotation = 0;
}


c2Dto3DMousePositionMngr::~c2Dto3DMousePositionMngr(void)
{
	delete p2Dto3DPlane;
	pMouseRayCamera = NULL;
}


Ogre::Vector3 c2Dto3DMousePositionMngr::get3DMouseCoordinate( int iMousePosX, int iMousePosY )
{
	if ( gl_bLogMessages_Level02 ) Ogre::LogManager::getSingleton().logMessage("MSG -> c2DT3DMPM -> G3MC 0");
	
	pMouseRayCamera = appPtr->mainViewport->getCamera();
	
	if ( pMouseRayCamera == NULL )
	{
		if ( gl_bLogMessages_Level01 ) Ogre::LogManager::getSingleton().logMessage("MSG -> c2DT3DMPM -> PMRC == NULL -> RETURN");

		return Ogre::Vector3(0,0,0);
	}
	
	// get window height and width
	Ogre::Real screenWidth = appPtr->mainViewport->getActualWidth();
	Ogre::Real screenHeight = appPtr->mainViewport->getActualHeight();
 
	// Camera to 0-1 offset
	Ogre::Real offsetX = iMousePosX / screenWidth;
	Ogre::Real offsetY = iMousePosY / screenHeight;
 
	// set up the ray
	Ray mouseRay = pMouseRayCamera->getCameraToViewportRay(offsetX, offsetY);
 
	// check if the ray intersects our plane
	// intersects() will return whether it intersects or not (the bool value) and
	// what distance (the Real value) along the ray the intersection is
	std::pair<bool, Real> result = mouseRay.intersects( p2Dto3DPlane->_getDerivedPlane() );
	
	if(result.first)
	{
		// if the ray intersect the plane, we have a distance value
		// telling us how far from the ray origin the intersection occurred.
		// the last thing we need is the point of the intersection.
		// Ray provides us getPoint() function which returns a point
		// along the ray, supplying it with a distance value.
 
		// get the point where the intersection is
		Vector3 point = mouseRay.getPoint(result.second);

		if ( pMouseRayCamera->getName() != "topViewCam" )
		{
			float fGridSize = appPtr->vpGrid->getPerspectiveSize()/2;
			Ogre::Vector3 fGridPos = appPtr->vpGrid->getGridNodePtr()->_getDerivedPosition();

			if ( point.x < -(fGridSize + 300) + fGridPos.x )
			{
				if ( point.x <= -(3000) + fGridPos.x )
					point.x = -(3000) + fGridPos.x;
				else
				{
					point.x = -(fGridSize + 300) + fGridPos.x;

					if ( point.x <= -(3000) + fGridPos.x )
						point.x = -(3000) + fGridPos.x;
				}
			}
			else if ( point.x > fGridPos.x + fGridSize + 300 )
			{
				if ( point.x >= 3000  + fGridPos.x )
					point.x = 3000 + fGridPos.x;
				else
				{
					point.x = fGridPos.x + fGridSize + 300;

					if ( point.x >= 3000  + fGridPos.x )
						point.x = 3000 + fGridPos.x;
				}
			}

			if ( point.z < -(fGridSize + 300) + fGridPos.z )
			{
				if ( point.z <= -(3000) + fGridPos.z )
					point.z = -(3000) + fGridPos.z;
				else
				{
					point.z = -(fGridSize + 300) + fGridPos.z;

					if ( point.z <= -(3000) + fGridPos.z )
						point.z = -(3000) + fGridPos.z;
				}			
			}
			else if ( point.z > fGridPos.z + fGridSize + 300 )
			{
				if ( point.z >= 3000 + fGridPos.z )
					point.z = 3000 + fGridPos.z;
				else
				{
					point.z = fGridPos.z + fGridSize + 300;

					if ( point.z >= 3000 + fGridPos.z )
						point.z = 3000 + fGridPos.z;
				}			
			}
		}
		
		// set Entity position to that point  
		p3DCoordinateNode->setPosition(point);
	}	
	
	Ogre::Vector3 v3MousePos3D = p3DCoordinateNode->getPosition();
	gl_bUpdateObjectPositionAndRotationInfoPanel = true;
	pMouseRayCamera = NULL;

	if ( gl_bLogMessages_Level02 ) Ogre::LogManager::getSingleton().logMessage("MSG -> c2DT3DMPM -> G3MC 1");
	
	return v3MousePos3D;
}


void c2Dto3DMousePositionMngr::activate2Dto3DMoveAndRotateSelectedObject(bool bActivate, std::string sObjectType, Ogre::Camera *pActvCamera)
{
	pMouseRayCamera = pActvCamera;

	if ( bActivate )
	{
		if ( sObjectType == "driver" )
		{
			gl_moveAndRotateObjectMode = true;
						
			appPtr->eSelectedEntityType = appPtr->SELECTED_DRIVER;
			
			pNewCreatedObjectNode = T[iLastObjectClicked_trackID]->Driver[iLastObjectClicked_ID]->getDriverBaseNodePtr();

			sLastCreatedObject = "driver";
			fHeightOffsetY = -(gl_fTrackHeight);
			p2Dto3DPlaneNode-> setPosition( 0, fHeightOffsetY, 0 );
		}
		else if ( sObjectType == "building" )
		{
			pNewCreatedObjectNode = T[iActvTrack]->cBldStackMngr->getActiveStackBaseNodePtr();
			
			sLastCreatedObject = "building";
			fHeightOffsetY = T[iActvTrack]->cBldStackMngr->getActiveStackBaseNodePtr()->getPosition().y;
			p2Dto3DPlaneNode-> setPosition( 0, fHeightOffsetY, 0 );

			appPtr->eSelectedEntityType = appPtr->SELECTED_BLD;

			gl_moveAndRotateObjectMode = true;
		}
		
		p3DCoordinateNode->attachObject( p3DCoordinateEntity );
		get3DMouseCoordinate(iClickMousePosX, iClickMousePosY);
	}
	else // if ( !bActivate )
	{
		appPtr->camTargetNode->setPosition( p3DCoordinateNode->getPosition().x, 0, p3DCoordinateNode->getPosition().z );
		if ( appPtr->mainViewport->getCamera() == appPtr->mainCamera )
			appPtr->mainCameraMan->lookAtTarget();

		fHeightOffsetY = 0;
		p2Dto3DPlaneNode-> setPosition( 0, fHeightOffsetY, 0 );

		p3DCoordinateNode->detachObject( p3DCoordinateEntity );
				
		pNewCreatedObjectNode = NULL;

		sLastCreatedObject = "";
	}
	
	return;
}


void c2Dto3DMousePositionMngr::setMouseClickPosition( int iMousePosX, int iMousePosY )
{
	iClickMousePosX = iMousePosX;
	iClickMousePosY = iMousePosY;
	
	return;
}


void c2Dto3DMousePositionMngr::deleteLastCreatedObject(void)
{
	if ( sLastCreatedObject == "driver" )
	{
		// letzten driver loeschen
		delete T[0]->Driver[T[0]->Driver.size()-1];
		T[0]->Driver.pop_back();
	}
	else if ( sLastCreatedObject == "building" )
	{
		// letzten Building Stack loeschen
		T[0]->cBldStackMngr->deleteCompleteStack( T[0]->cBldStackMngr->deqBuildingsPtr.size()-1 );
	}

	return;
}


Ogre::SceneNode* c2Dto3DMousePositionMngr::get3DCoordinateNodePtr(void)
{
	return p3DCoordinateNode;
}

void c2Dto3DMousePositionMngr::reset_fLastBuildingCreatedWithRotation(void)
{
	fLastBuildingCreatedWithRotation = 0;

	return;
}