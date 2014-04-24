#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxUGen.h"
#include "ofxBox2d.h"
#include "Leap.h"
#include "Sinewave.h"
#include "rotateSinewave.h"
#include "whiteNoise.h"
#include "ofxContourAnalysis.h"
#include "CustomCircle.h"
#include "CustomRect.h"
//#include "CustomPolygon.h"
#include "rotateBass.h"
#include "myLFSaw.h"
#include "myLFPulse.h"
#include "myHarmonicOsc.h"
#include "bass.h"

using namespace Leap;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

    
        ofVideoGrabber 		vidGrabber;
		
        ofxCvColorImage			colorImg;

        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

        ofxCvContourFinder 	contourFinder;

		int 				threshold;
		bool				bLearnBakground;
    
    ofxBox2d						box2d;			  //	the box2d world
	vector		<CustomCircle*>	circles;		  //	default box2d circles
	vector		<CustomRect*>	boxes;//	defalut box2d rects
    
    deque <Sinewave*>       sinewaves;
    deque <rotateSinewave*> rotateSinewaves;
    deque <rotateBass*>     bassSounds;
    deque <whiteNoise*>     whiteNoises;
    deque <myLFSaw*>        myLFSaws;
    deque <myLFPulse*>      myLFPulses;
    deque <myHarmonicOsc*>  myHarmonicOsces;
    deque <bass*>           basses;
    
    int circlePositionX,circlePositionY,boxPositionX,boxPositionY,cPforNoizeX,cPforNoizeY,boxPforPulseX,boxPforPulseY;
    int freq,bassFreq;
    float pan,bassPan;
    
    // Leap Motionのコントローラー
    Leap::Controller controller;
    Leap::Finger finger;
    Leap::Hand hand;
    // カメラ
    
    float camdistance;
    float camdegree;
    
    void drawFinger(Leap::Finger finger);
    void drawPoint(ofPoint point);
    void drawFingerBox(Leap::Finger finger, ofPoint tip, ofPoint base);
    void drawPalm(Leap::Hand hand);
    void Sound();
    void durationSound();
    void drawLineStrip();
    void lineDraw();
    void Sound7();
    
    ofxContourAnalysis contourAnalysis;
    
    vector <ofPoint>   simpleCountour;
    
    float              simpleAmount;
    bool               bReversePoints,bSoundOn,bDurationSound,blineStrip,b0Gravity,bDurationSound2,bTextHide,b7Sound;
    
    ofxBox2dLine       lineStrip;
    
    float              gravity,sideGravity,radius;
    
    ofVec2f enMove2_2,enMove3_2,enMove4,enMove1_1;
    
    float count2;
};

