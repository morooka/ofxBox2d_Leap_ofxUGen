//
//  myLFSaw.h
//  box2d_UGen_Leap
//
//  Created by 諸岡 光男 on 2013/11/02.
//
//
#pragma once

#include "ofxUGen.h"

class myLFSaw : public ofxUGen::SynthDef
{
public:
	
	ofVec3f pos;
	
	UGen envgen;
	UGen amp;
    
    float volume;
    //int lineLeftX,lineLeftY,lineRightX,lineRightY;
    
    myLFSaw(int x,int y,int z)
	{
        pos.x = x;
        pos.y = y;
        pos.z = z;
        
		float freq = ofMap(pos.y, 0, ofGetWidth(), 12000,14000);
		float pan = ofMap(pos.x, 200, ofGetHeight()-200, -1, 1);
        float volume = ofMap(pos.z,50, 700, 200, 1);
        
        
		Env env = Env::perc(0.1,0.5,1.5, EnvCurve::Sine);
		envgen = EnvGen::AR(env);
		envgen.addDoneActionReceiver(this);
		
        amp =SinOsc::AR(ofRandom(4.0), 0, 0.5, 0.5) *  envgen;
        
        Out(
            Pan2::AR(LFSaw::AR(freq,1.0,1.0) * amp*volume/1000, pan)
            );
    }
	void draw(int width,int height,int x, int y)
	{
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        ofSetColor(255,248,144,amp.getValue()*20);
        glLineWidth(2);
        ofLine(width,height,x,y);
    }
    
	bool isAlive()
	{
		return !Out().isNull();
	}
};

