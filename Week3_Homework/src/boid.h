/*

 *
 */

#ifndef _BOID
#define _BOID
#include <vector>
#include "ofMain.h"

class Boid
{
// all the methods and variables after the
// private keyword can only be used inside
// the class
public:
	ofVec3f position;
	ofVec3f velocity;
	
	float separationWeight;
	float cohesionWeight;
	float alignmentWeight;
    float separationWeightTwo;
    float cohesionWeightTwo;
    float alignmentWeightTwo;
	
	float separationThreshold;
	float neighbourhoodSize;
    float separationThresholdTwo;
    float neighbourhoodSizeTwo;
	
	ofVec3f separation(std::vector<Boid *> &otherBoids);
	ofVec3f cohesion(std::vector<Boid *> &otherBoids);
	ofVec3f alignment(std::vector<Boid *> &otherBoids);
    
    ofVec3f separationTwo(std::vector<Boid *> &otherBoids);
    ofVec3f cohesionTwo(std::vector<Boid *> &otherBoids);
	
	Boid();
	Boid(ofVec3f &pos, ofVec3f &vel);
	
	~Boid();
	
	ofVec3f getPosition();
	ofVec3f getVelocity();
	
	
	float getSeparationWeight();
	float getCohesionWeight();
	float getAlignmentWeight();
	
	float getSeparationThreshold();
	float getNeighbourhoodSize();
	
	void setSeparationWeight(float f);
	void setCohesionWeight(float f);
	void setAlignmentWeight(float f);
	
	void setSeparationThreshold(float f);
	void setNeighbourhoodSize(float f);
	
	void update(std::vector<Boid *> &otherBoids, std::vector<Boid *> &otherBoidsTwo, ofVec3f &min, ofVec3f &max);
	
	void walls(ofVec3f &min, ofVec3f &max);
	
	void draw();
    void myDraw();
};

//New boids group
class myBoid: public Boid{
private:
    ofVec3f myPosition;
    ofVec3f myPvelocity;
    
    float mySeparationWeight;
    float myCohesionWeight;
    float myAlignmentWeight;
    
    float mySeparationThreshold;
    float myNeighbourhoodSize;
    
    ofVec3f mySeparation(std::vector<myBoid *> &otherBoids);
    ofVec3f myCohesion(std::vector<myBoid *> &otherBoids);
    ofVec3f myAlignment(std::vector<myBoid *> &otherBoids);
};

#endif
