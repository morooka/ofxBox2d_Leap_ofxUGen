//
//  myLFPulse.h
//  box2d_UGen_Leap
//
//  Created by 諸岡 光男 on 2013/11/02.
//
//
#pragma onse

#include "ofxUGen.h"

class myLFPulse : public ofxUGen::SynthDef
{
public:
	
	ofVec3f pos;
	
	UGen envgen;
	UGen amp;
    
    int count;
    
	myLFPulse(int x, int y,int z)
	{
		pos.x = x;
		pos.y = y;
        pos.z = z;
		
		float freq = ofMap(pos.y, 0, ofGetHeight(), 100, 2000);
		float pan = ofMap(pos.x, 0, ofGetWidth(), -1, 1);
        float volume = ofMap(pos.z,50, 700, 500, 1);
        
		Env env = Env::perc(0.6, 1.0, 0.5, EnvCurve::Welch);  //sine // Numerical :Welch
		envgen = EnvGen::AR(env);
		envgen.addDoneActionReceiver(this);
		
		amp = LFPulse::AR() * envgen;   //SinOsc::AR(ofRandom(4,0), 0, 0.5, 0.5) FSinOsc SinOsc LFPulse
        // amp = SinOsc::AR(0, 0, 0.5, 0.5);
		
        Out(
            Pan2::AR(SinOsc::AR(freq)*amp*volume/700, pan)
            );
    }
	void draw()
	{
		ofFill();
        // glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
		ofSetColor(50,233,50,255*amp.getValue()*ofRandom(0.5,1.5));
		ofCircle(pos.x, pos.y, amp.getValue() * 20*ofRandom(-20,20));
	}
    
    bool isAlive()
	{
		return !Out().isNull();
	}
};

