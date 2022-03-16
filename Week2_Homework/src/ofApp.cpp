#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    image.load("blackGalaxy.jpg");
//    image.load("galaxy.jpg");
//    image.load("nebula.png");
    image.load("planet.jpeg");
//    image.load("stars.png");
    
    image.resize(200, 200);
    gui.setup();
    
    mesh.setMode(OF_PRIMITIVE_LINES);
//    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
//    mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
//    mesh.enableColors();
    mesh.enableIndices();
    
    // Add this line to explicitly set the framerate to 60 frames per second:
    ofSetFrameRate(60);
    
    //gui.add(intensityThreshold.setup("intensityThreshold", 150, 50,300));
//        float intensityThreshold = 50.0;   //blackGalaxy
//        float intensityThreshold = 50.0;   //galaxy
//        float intensityThreshold = 100.0;   //nebula
        float intensityThreshold = 60.0;   //planet
//        float intensityThreshold = 150.0;   //stars

    int w = image.getWidth();
    int h = image.getHeight();
    for(int x=0; x<w; ++x)
        for(int y=0; y<h; ++y)
        {
            ofColor c = image.getColor(x, y);
            float intensity = c.getLightness();
            if(intensity >= intensityThreshold){
                // We shrunk our image by a factor of 4, so we need to multiply our pixel
                // locations by 4 in order to have our mesh cover the openFrameworks window
                float saturation = c.getSaturation();
                float z = ofMap(saturation, 0, 255, -100, 100);
                ofVec3f pos(x*4, y*4, z);
                mesh.addVertex(pos);
                mesh.addColor(c);
                
                // And add this line to your existing for loop:
                // It will create a ofVec3f with 3 random values, which
                // will allow us to move the x, y and z coordinates of
                // each vertex independently
                offsets.push_back(ofVec3f(ofRandom(0, 100000), ofRandom(0, 100000), ofRandom(0, 100000)));
            }
        }
    //gui.add(connectionDistance.setup("connectionDistance", 30, 1,100));
    float connectionDistance = 20;    //blackGalaxy
//    /float connectionDistance = 15;    //galaxy
//    float connectionDistance = 20;    //nebula
//    float connectionDistance = 20;    //planet
//    float connectionDistance = 30;    //stars
    
    int numVerts = mesh.getNumVertices();
    for(int a=0; a<numVerts; ++a){
        ofVec3f verta = mesh.getVertex(a);
        for(int b=a+1; b<numVerts; ++b){
            ofVec3f vertb = mesh.getVertex(b);
            float distance = verta.distance(vertb);
            if(distance <= connectionDistance){
                // In OF_PRIMITIVE_LINES, every pair of vertices or indices will be connected to form a line
                mesh.addIndex(a);
                mesh.addIndex(b);
            }
        }
    }
    
    
    // We need to calculate our center point for the mesh
    // ofMesh has a method called getCentroid() that will
    // find the average location over all of our vertices
    //    http://en.wikipedia.org/wiki/Centroid
    meshCentroid = mesh.getCentroid();
    
    // Now that we know our centroid, we need to know the polar coordinates
    // (distance and angle) of each vertex relative to that center point.
    // We've found the distance between points before, but what about the angle?
    // This is where atan2 comes in.  atan2(y, x) takes an x and y value and returns
    // the angle relative to the origin (0,0).  If we want the angle between two
    // points (x1, y1) and (x2, y2) then we just need to use atan2(y2-y1, x2-x1).
    for(int i=0; i<numVerts; ++i){
        ofVec3f vert = mesh.getVertex(i);
        float distance = vert.distance(meshCentroid);
        float angle = atan2(vert.y-meshCentroid.y, vert.x-meshCentroid.x);
        distances.push_back(distance);
        angles.push_back(angle);
    }
    // These variables will allow us to toggle orbiting on and off
    orbiting = false;
    startOrbitTime = 0.0;
    meshCopy = mesh;    // Store a copy of the mesh, so that we can reload the original state
}

//--------------------------------------------------------------
void ofApp::update(){
    int numVerts = mesh.getNumVertices();
    for(int i=0; i<numVerts; ++i){
        ofVec3f vert = mesh.getVertex(i);
        
        float time = ofGetElapsedTimef();
        float timeScale = 5.0;
        float displacementScale = 0.75;
        ofVec3f timeOffsets = offsets[i];
        
        // A typical design pattern for using Perlin noise uses a couple parameters:
        // ofSignedNoise(time*timeScale+timeOffset)*displacementScale
        //     ofSignedNoise(time) gives us noise values that change smoothly over
        //         time
        //     ofSignedNoise(time*timeScale) allows us to control the smoothness of
        //         our noise (smaller timeScale, smoother values)
        //     ofSignedNoise(time+timeOffset) allows us to use the same Perlin noise
        //         function to control multiple things and have them look as if they
        //         are moving independently
        //     ofSignedNoise(time)*displacementScale allows us to change the bounds
        //         of the noise from [-1, 1] to whatever we want
        // Combine all of those parameters together, and you've got some nice
        // control over your noise
        vert.x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
        vert.y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
        vert.z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
        mesh.setVertex(i, vert);
    }
    
    if(orbiting){
        int numVerts = mesh.getNumVertices();
        for(int i=0; i<numVerts; ++i){
            ofVec3f vert = mesh.getVertex(i);
            float distance = distances[i];
            float angle = angles[i];
            float elapsedTime = ofGetElapsedTimef() - startOrbitTime;
            
            // Lets adjust the speed of the orbits such that things that are closer to
            // the center rotate faster than things that are more distant
            float speed = ofMap(distance, 0, 200, 1, 0.25, true);
            
            // To find the angular rotation of our vertex, we use the current time and
            // the starting angular rotation
            float rotatedAngle = elapsedTime * speed + angle;
            
            // Remember that our distances are calculated relative to the centroid
            // of the mesh, so we need to shift everything back to screen
            // coordinates by adding the x and y of the centroid
            vert.x = distance * cos(rotatedAngle) + meshCentroid.x;
            vert.y = distance * sin(rotatedAngle) + meshCentroid.y;
            
            mesh.setVertex(i, vert);
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofColor centerColor = ofColor(11, 16, 33);    //blue
    ofColor centerColor = ofColor(33, 20, 11);
    ofColor edgeColor(0, 0, 0);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);
    //ofBackground(0, 0, 255);
        
    easyCam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    mesh.draw();
    ofPopMatrix();
    easyCam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'o'){
        orbiting = !orbiting; //This inverts the boolean
        startOrbitTime = ofGetElapsedTimef();
        mesh = meshCopy;    // This restores the mesh to its original values
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
