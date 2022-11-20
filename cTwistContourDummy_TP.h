#include "C3D.h"

#ifndef __CCONTOURDUMMY_TPTWIST__
#define __CCONTOURDUMMY_TPTWIST__


class cTwistContourDummy_TP
{
public:
	cTwistContourDummy_TP(int, int, int, Ogre::String, int);
	~cTwistContourDummy_TP(void);

	void setTwist(float);
	// int Segment, int Vertex
	Ogre::Vector3 getVertexPosition(int, int);

	Ogre::Vector3 getVertexPosition_PitLaneLeft(int, int);
	Ogre::Vector3 getVertexPosition_PitLaneRight(int, int);
	
	Ogre::Vector3 getVertexPosition_ShoulderLeft(int, int);
	Ogre::Vector3 getVertexPosition_ShoulderRight(int, int);

private:
		
	Ogre::String tpName;
	
	int iTrack;
	int iStrand;
	int iTP;
	
	float fDummyTwist;	// "ist"-twistValue von twistDummy
	
	int tpID;
	int nSegs;
	
	void createDummy(void);
	void allignSegmentDummys(void);
	
	Ogre::SceneNode *parentNodeTP;			// der parentNodeTP ist der Pointer auf den TP-RollNodeTP
	Ogre::SceneNode *twistDummyStartNodeTP;	// der twistDummyStartNodeTP ist der startNodeTP des twistDummy

	Ogre::SceneNode *centerNodeTP;
	Ogre::SceneNode *radiusNodeTP;
	
	Ogre::Entity *radiusEnt;
	Ogre::Entity *centerEnt;
		
	std::vector< std::vector<Ogre::SceneNode*>* > vSegmentNodes_TP;
	
	// fuer dev -> Dummy-Points sichtbar machen
	std::vector<Ogre::Entity*> vPoints;
	void createAndShowPoints(void);
	
};

#endif
