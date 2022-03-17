/*
 *  boid.cpp
 *  boids
 *
 *  Created by Marco Gillies on 05/10/2010.
 *  Copyright 2010 Goldsmiths, University of London. All rights reserved.
 *
 */

#include "boid.h"
#include "ofMain.h"

Boid::Boid()
{
	separationWeight = 0.8f;
	cohesionWeight = 0.2f;
	alignmentWeight = 0.1f;
    
    separationWeightTwo = 0.2f;
    cohesionWeightTwo = 0.2f;
    alignmentWeightTwo = 0.1f;
    
	separationThreshold = 15;
	neighbourhoodSize = 100;
    
    separationThresholdTwo = 10;
    neighbourhoodSizeTwo = 150;
	
	position = ofVec3f(ofRandom(0, 200), ofRandom(0, 200));
	velocity = ofVec3f(ofRandom(-2, 2), ofRandom(-2, 2));
}

Boid::Boid(ofVec3f &pos, ofVec3f &vel)
{
	separationWeight = 1.0f;
	cohesionWeight = 0.2f;
	alignmentWeight = 0.1f;
    
    separationWeightTwo = 1.0f;
    cohesionWeightTwo = 0.2f;
    alignmentWeightTwo = 0.1f;
	
	separationThreshold = 15;
	neighbourhoodSize = 100;
    
    separationThresholdTwo = 15;
    neighbourhoodSizeTwo = 150;
	
	position = pos;
	velocity = vel;
}

Boid::~Boid()
{
	
}

float Boid::getSeparationWeight()
{
	return separationWeight;
}
float Boid::getCohesionWeight()
{
	return cohesionWeight;
}

float Boid::getAlignmentWeight()
{
	return alignmentWeight;
}


float Boid::getSeparationThreshold()
{
	return separationThreshold;
}

float Boid::getNeighbourhoodSize()
{
	return neighbourhoodSize;
}


void Boid::setSeparationWeight(float f)
{
	separationWeight = f;
}
void Boid::setCohesionWeight(float f)
{
	cohesionWeight = f;
}

void Boid::setAlignmentWeight(float f)
{
	alignmentWeight = f;
}


void Boid::setSeparationThreshold(float f)
{
	separationThreshold = f;
}

void Boid::setNeighbourhoodSize(float f)
{
	neighbourhoodSize = f;
}


ofVec3f Boid::getPosition()
{
	return position;
}

ofVec3f Boid::getVelocity()
{
	return velocity;
}

ofVec3f Boid::separation(std::vector<Boid *> &otherBoids)
{
	for (int i = 0; i < otherBoids.size(); i++)
	{	
		if(position.distance(otherBoids[i]->getPosition()) < separationThreshold)
		{
			ofVec3f v = position -  otherBoids[i]->getPosition();
			v.normalize();
			return v;
		}
	}
}

ofVec3f Boid::separationTwo(std::vector<Boid *> &otherBoidsTwo)
{
    for (int i = 0; i < otherBoidsTwo.size(); i++)
    {
        if(position.distance(otherBoidsTwo[i]->getPosition()) < separationThresholdTwo)
        {
            ofVec3f v = position -  otherBoidsTwo[i]->getPosition();
            v.normalize();
            return v;
        }
    }
}

ofVec3f Boid::cohesion(std::vector<Boid *> &otherBoids)
{
	ofVec3f average(0,0,0);
	int count = 0;
	for (int i = 0; i < otherBoids.size(); i++)
	{
		if (position.distance(otherBoids[i]->getPosition()) < neighbourhoodSize)
		{
			average += otherBoids[i]->getPosition();
			count += 1;
		}
	}
	average /= count;
	ofVec3f v =  average - position;
	v.normalize();
	return v;
}

ofVec3f Boid::cohesionTwo(std::vector<Boid *> &otherBoidsTwo)
{
    ofVec3f average(0,0,0);
    int count = 0;
    for (int i = 0; i < otherBoidsTwo.size(); i++)
    {
        if (position.distance(otherBoidsTwo[i]->getPosition()) < neighbourhoodSizeTwo)
        {
            average += otherBoidsTwo[i]->getPosition();
            count += 1;
        }
    }
    average /= count;
    ofVec3f v =  average - position;
    v.normalize();
    return v;
}

ofVec3f Boid::alignment(std::vector<Boid *> &otherBoids)
{
	ofVec3f average(0,0,0);
	int count = 0;
	for (int i = 0; i < otherBoids.size(); i++)
	{
		if (position.distance(otherBoids[i]->getPosition()) < neighbourhoodSize)
		{
			average += otherBoids[i]->getVelocity();
			count += 1;
		}
	}
	average /= count;
	ofVec3f v =  average - velocity;
	v.normalize();
	return v;
}

void Boid::update(std::vector<Boid *> &otherBoids, std::vector<Boid *> &otherBoidsTwo, ofVec3f &min, ofVec3f &max)
{
	velocity += separationWeight*separation(otherBoids) + separationWeightTwo*separationTwo(otherBoidsTwo);
	velocity += cohesionWeight*cohesion(otherBoids) + cohesionWeightTwo*cohesionTwo(otherBoidsTwo);
	velocity += alignmentWeight*alignment(otherBoids);
	
	walls(min, max);
	position += velocity;
}

void Boid::walls(ofVec3f &min, ofVec3f &max)
{
	if (position.x < min.x){
		position.x = min.x;
		velocity.x *= -1;
	} else if (position.x > max.x){
		position.x = max.x;
		velocity.x *= -1;
	}
	
	if (position.y < min.y){
		position.y = min.y;
		velocity.y *= -1;
	} else if (position.y > max.y){
		position.y = max.y;
		velocity.y *= -1;
	}
	
	
}

void Boid::draw()
{
	ofSetColor(0, 255, 255);
	ofCircle(position.x+400, position.y+400, 5);
}

void Boid::myDraw()
{
    ofSetColor(230, 120, 180);
    ofCircle(position.x, position.y, 3);
}
