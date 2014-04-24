#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
   // vidGrabber.setDeviceID(1);
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
    
    controller.setPolicyFlags(Controller::POLICY_BACKGROUND_FRAMES );
    
    box2d.init();
	box2d.setGravity(0, 0);
	//box2d.setFPS(30.0);
	//box2d.registerGrabbing();
    
    ofxUGen::s().setup();
    
    ofBackground(0);
    
    simpleAmount=4.0;
    bReversePoints=false;
    bSoundOn = false;
    bDurationSound = false;
    blineStrip = false;
    b0Gravity = false;
    bDurationSound2 = false;
    bTextHide = false;
    b7Sound = false;
    
    ofHideCursor();
    
}

//--------------------------------------------------------------
void ofApp::update(){

    bool bNewFrame = false;

       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    

	if (bNewFrame){
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}
    
    box2d.update();
    
    if (circles.size() > 0) {
        circlePositionX = circles.back()->getPosition().x;
        circlePositionY = circles.back()->getPosition().y;
        freq = ofMap(circlePositionY,0,ofGetHeight(),12000,10000);
        pan = ofMap(circlePositionX, ofGetWidth()/5,(ofGetWidth()/5)*4, 1, -1);
    }
    if (circles.size() > 1) {
        cPforNoizeX = circles[circles.size()-2]->getPosition().x;
        cPforNoizeY = circles[circles.size()-2]->getPosition().y;
    }
    
    if(boxes.size() >0){
        boxPositionX = boxes.back()->getPosition().x;
        boxPositionY = boxes.back()->getPosition().y;
        bassFreq = ofMap(boxPositionY,0,ofGetHeight(),50,150);
        bassPan = ofMap(boxPositionX, ofGetWidth()/5,(ofGetWidth()/5)*4, 1, -1);
        
    }
    if(boxes.size() > 1){
        boxPforPulseX = boxes[boxes.size()-2]->getPosition().x;
        boxPforPulseY = boxes[boxes.size()-2]->getPosition().y;
    }
        gravity = ofMap(finger.tipPosition().z,-150,150,-20,20);
        sideGravity = ofMap(finger.tipPosition().x,-300,300,20,-20);
        box2d.setGravity(sideGravity, gravity);
    if (hand.fingers().count() == 0 &&  b0Gravity) {
        box2d.setGravity(0, 0);
    }
    
    if(contourFinder.nBlobs > 0){
        radius =ofDist(contourFinder.blobs[0].boundingRect.x*ofGetWidth()/320,contourFinder.blobs[0].boundingRect.y*ofGetHeight()/240,
                       contourFinder.blobs[0].boundingRect.x*ofGetWidth()/320+contourFinder.blobs[0].boundingRect.width*ofGetWidth()/320,contourFinder.blobs[0].boundingRect.y*ofGetHeight()/240+contourFinder.blobs[0].boundingRect.height)/2*ofGetHeight()/240;
        radius = radius/1.5;
        
        count2 = count2 + 0.3f;
        
        enMove1_1.x = std::cos(count2)*radius;
        enMove1_1.y = std::sin(count2)*radius;
        
        enMove2_2.x = std::cos(count2+300)*radius;
        enMove2_2.y = std::sin(count2+300)*radius;
        
        enMove3_2.x = std::cos(count2+600)*radius;
        enMove3_2.y = std::sin(count2+600)*radius;
        
        enMove4.x = std::cos(count2+900)*radius;
        enMove4.y = std::sin(count2+900)*radius;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableAlphaBlending();
    
    if (b7Sound && (hand.fingers().count() == 2 || hand.fingers().count() == 1)) {
        ofBackground(ofRandom(0,50), ofRandom(0,50), ofRandom(0,50));
    }
    
    ofSetColor(255,150);
	vidGrabber.draw(0, 0,ofGetWidth(),ofGetHeight());
    
    Sound7();
    
   
    Frame frame = controller.frame();
    
    for(int i=0; i<frame.hands().count(); i++) {
        hand = frame.hands()[i];
        // Hand内のFingerをあるだけ描画
        for(int j=0; j<hand.fingers().count(); j++) {
            finger = frame.fingers()[j];
            drawFinger(finger);
        }
    }
    // Handを描画
    drawPalm(hand);
    
    deque<Sinewave*>::iterator it1 = sinewaves.begin();
    while (it1 != sinewaves.end())
    {
        Sinewave *s = *it1;
        s->draw(circlePositionX,circlePositionY);
        
        if (!s->isAlive())
        {
            it1 = sinewaves.erase(it1);
            delete s;
        }
        else
            it1++;
    }
    deque<bass*>::iterator it5 = basses.begin();
    while (it5 != basses.end())
    {
        bass *s = *it5;
        s->draw(boxPositionX,boxPositionY);
        
        if (!s->isAlive())
        {
            it5 = basses.erase(it5);
            delete s;
        }
        else
            it5++;
    }


    
    if (blineStrip) drawLineStrip();
    
    for(int i=0; i<circles.size(); i++) {
        circles[i]->draw();
	}
    for(int i=0; i<boxes.size(); i++) {
		boxes[i]->draw();
	}
    
    if(bDurationSound && circles.size() > 0) {
        durationSound();
    }
    
    if(bSoundOn) Sound();
    
    lineDraw();

    if (bTextHide) {
        ofSetHexColor(0xffffff);
        stringstream reportStr;
        reportStr << "bg subtraction and blob detection" << endl
        << "press ' ' to capture bg" << endl
        << "threshold " << threshold << " (press: +/-)" << endl
        << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
        ofDrawBitmapString(reportStr.str(), 20, 600);
    }
	
    ofLine(mouseX-5,mouseY-5, mouseX+5, mouseY+5);
    ofLine(mouseX-5,mouseY+5, mouseX+5, mouseY-5);
}
//--------------------------------------------------------------
void ofApp::drawLineStrip()
{
    if(contourFinder.nBlobs>0){
        contourAnalysis.simplify(contourFinder.blobs[0].pts,simpleCountour,simpleAmount);
		ofPoint p;
		lineStrip.setWorld(box2d.getWorld());
		lineStrip.clear();
        if(bReversePoints){
			for(int i=simpleCountour.size()-1;i>0;i--){
				p.x=simpleCountour[i].x*ofGetWidth()/320;
				p.y=simpleCountour[i].y*ofGetHeight()/240;
				lineStrip.addPoint(p.x, p.y);
			}
		}
		else{
			for(int i=0;i<simpleCountour.size();i++){
				p.x=simpleCountour[i].x*ofGetWidth()/320;
				p.y=simpleCountour[i].y*ofGetHeight()/240;
				lineStrip.addPoint(p.x, p.y);
            }
        }
        
		lineStrip.createShape();
    }
    ofSetColor(204,153,255);
    lineStrip.draw();
}
//--------------------------------------------------------------
void ofApp::durationSound(){
    ofSetColor(125,155,255);
    ofSetLineWidth(2);
    ofLine(circlePositionX,circlePositionY,boxPositionX,boxPositionY);
    ofLine(boxPositionX, boxPositionY, cPforNoizeX, cPforNoizeY);
    ofLine(cPforNoizeX, cPforNoizeY, circlePositionX, circlePositionY);
}
//--------------------------------------------------------------
void ofApp::Sound(){
         if ((hand.fingers().count() == 1 || bDurationSound) && circles.size() >0){
             deque<rotateSinewave*>::iterator it1 = rotateSinewaves.begin();
             while (it1 != rotateSinewaves.end())
             {
                 rotateSinewave *s = *it1;
                 s->rotation = 2000;
                 s->color1=(ofVec3f(253,217,217));
                 s->color2=(ofVec3f(220,214,217));
                 s->color3=(ofVec3f(253,217,217));
                 s->radius =200;
                 s->draw(0,0);
                 
                 if (!s->isAlive())
                 {
                     it1 = rotateSinewaves.erase(it1);
                     delete s;
                 }
                 else
                     it1++;
             }
             if(circles.size() > 0){
             rotateSinewaves.push_back(new rotateSinewave(circlePositionX,circlePositionY,
                                              finger.tipPosition().y,70,4000,10000));
             }
             while (rotateSinewaves.size() > 8) {
                 rotateSinewaves.pop_front();
             }
             rotateSinewaves.back()->play();
         }
    
    if ((hand.fingers().count() == 2 || bDurationSound) && boxes.size() > 0 ){
        deque<rotateBass*>::iterator it2 = bassSounds.begin();
        while (it2 != bassSounds.end())
        {
            rotateBass *s = *it2;
            s->draw();
            
            if (!s->isAlive())
            {
                it2 = bassSounds.erase(it2);
                delete s;
            }
            else
                it2++;
        }
        bassSounds.push_back(new rotateBass(boxPositionX,boxPositionY,finger.tipPosition().y,50,150,true,130));
        
        while (bassSounds.size() > 15) {
            bassSounds.pop_front();
        }
        bassSounds.back()->play();
    }
    if ((hand.fingers().count() == 3 || bDurationSound) && (circles.size() > 1)){
        deque<whiteNoise*>::iterator it3 = whiteNoises.begin();
        while (it3 != whiteNoises.end())
        {
            whiteNoise *s = *it3;
            s->draw();
            
            if (!s->isAlive())
            {
                it3 = whiteNoises.erase(it3);
                delete s;
            }
            else
                it3++;
        }
        whiteNoises.push_back(new whiteNoise(cPforNoizeX,cPforNoizeY,finger.tipPosition().y));
        while (whiteNoises.size() > 7) {
            whiteNoises.pop_front();
        }
        whiteNoises.back()->play();
    }
}
//--------------------------------------------------------------
void ofApp::lineDraw(){
    if ((hand.fingers().count() == 4 &&  bSoundOn ) || bDurationSound2){
        deque<myLFSaw*>::iterator it7 = myLFSaws.begin();
        while (it7 != myLFSaws.end())
        {
            myLFSaw *s = *it7;
            if(contourFinder.nBlobs>0){
                contourAnalysis.simplify(contourFinder.blobs[0].pts,simpleCountour,simpleAmount);
                ofPoint p;
                for(int i=0;i<simpleCountour.size();i++){
                    p.x=simpleCountour[i].x*ofGetWidth()/320;
                    p.y=simpleCountour[i].y*ofGetHeight()/240;
                    
                    for (int j = 1; j < circles.size()/2; j++) {
                        
                        if (circles[j]->getPosition().x < p.x) {
                            s->draw(p.x,p.y,circles[j]->getPosition().x,circles[j]->getPosition().y);
                        }
                        if (circles[j]->getPosition().x > p.x) {
                                s->draw(p.x,p.y,circles[j]->getPosition().x,circles[j]->getPosition().y);
                        }
                    }
                }
            }
            
            if (!s->isAlive())
            {
                it7 = myLFSaws.erase(it7);
                delete s;
            }
            else
                it7++;
        }
        myLFSaws.push_back(new myLFSaw(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()),0));
        while (myLFSaws.size() >5) {
            myLFSaws.pop_front();
        }
        myLFSaws.back()->play();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case OF_KEY_UP:
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case OF_KEY_DOWN:
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'o':
            bSoundOn = !bSoundOn;
            break;
        case 'd':
            bDurationSound = !bDurationSound;
            break;
        case 'l':
            blineStrip = !blineStrip;
            break;
        case '1':
            sinewaves.push_back(new Sinewave(&freq,&pan));
            sinewaves.back()->play();
            break;
        case '3':
            basses.push_back(new bass(&bassFreq,&bassPan));
            basses.back()->play();
            break;
        case 'b':
            box2d.createBounds();
            break;
        case'0':
            b0Gravity = !b0Gravity;
            break;
        case 's':
            bDurationSound2 = !bDurationSound2;
            break;
        case 't':
            bTextHide = !bTextHide;
            break;
        case '7':
            b7Sound = !b7Sound;
            break;
	}
    
    for (int i = 0; i < sinewaves.size(); i++)
        sinewaves[i]->release();
    for (int i = 0; i < rotateSinewaves.size(); i++)
        rotateSinewaves[i]->release();
    for (int i = 0; i < bassSounds.size(); i++) {
        bassSounds[i]->release();
    }
    for (int i = 0; i < whiteNoises.size(); i++) {
        whiteNoises[i]->release();
    }
    
    
    if (key == 'c') {
        float r = ofRandom(4, 20);		// a random radius 4px - 20px
        CustomCircle * c =new CustomCircle(circles.size());
        c->setPhysics(3.0, 0.53, 0.1);
        c->setup(box2d.getWorld(), mouseX, mouseY, r);
        circles.push_back(c);
    }
    if (key == 'x') {
        CustomRect * r =new CustomRect(boxes.size());
        r->setPhysics(3.0, 0.53, 0.1);
        r->setup(box2d.getWorld(), mouseX, mouseY, 15,15);
        boxes.push_back(r);
    }
    if (key == 'v') {
        if (circles.size() > 0) {
            circles.back()->destroyShape();
            circles.pop_back();
        }
    }
    if (key == 'z') {
        if (boxes.size() > 0){
            boxes.back()->destroyShape();
            boxes.pop_back();
        }
    }
    if(key == '2'){
        if(sinewaves.size() > 0) {
            sinewaves.back()->stop();
            sinewaves.pop_back();
        }
    }
    if (key == '3') {
        if (basses.size() > 0) {
            basses.back()->stop();
            basses.pop_back();
        }
    }
    if(key == 's'){
        vidGrabber.videoSettings();
    }
}
//--------------------------------------------------------------
void ofApp::Sound7(){
    if (b7Sound){
        if ( hand.fingers().count() == 1 || bDurationSound2) {
            deque<myLFPulse*>::iterator it3 = myLFPulses.begin();
            while (it3 != myLFPulses.end())
            {
                myLFPulse *s = *it3;
                s->draw();
                
                if (!s->isAlive())
                {
                    it3 = myLFPulses.erase(it3);
                    delete s;
                }
                else
                    it3++;
            }
            myLFPulses.push_back(new myLFPulse(boxPforPulseX,boxPforPulseY,finger.tipPosition().y));
            while (myLFPulses.size() > 6) {
                myLFPulses.pop_front();
            }
            myLFPulses.back()->play();
        }
        
        if (hand.fingers().count() == 2 || bDurationSound2) {
            deque<myHarmonicOsc*>::iterator it4 = myHarmonicOsces.begin();   //*ofGetWidth()/320  *ofGetHeight()/240
            while (it4 != myHarmonicOsces.end())
            {
                myHarmonicOsc *s = *it4;
                for( int i=0; i<contourFinder.blobs[0].nPts; i= i+20 ){
                    ofSetColor(151,202,153,200);
                    s->draw(contourFinder.blobs[0].centroid.x*ofGetWidth()/320+enMove1_1.x,contourFinder.blobs[0].centroid.y*ofGetHeight()/240+enMove1_1.y,contourFinder.blobs[0].pts[i].x*ofGetWidth()/320,contourFinder.blobs[0].pts[i].y*ofGetHeight()/240);
                    ofSetColor(151,202,202,200);
                    s->draw(contourFinder.blobs[0].centroid.x*ofGetWidth()/320+enMove2_2.x,contourFinder.blobs[0].centroid.y*ofGetHeight()/240+enMove2_2.y,contourFinder.blobs[0].pts[i].x*ofGetWidth()/320,contourFinder.blobs[0].pts[i].y*ofGetHeight()/240);
                    ofSetColor(151,202,151,200);
                     s->draw(contourFinder.blobs[0].centroid.x*ofGetWidth()/320+enMove3_2.x,contourFinder.blobs[0].centroid.y*ofGetHeight()/240+enMove3_2.y,contourFinder.blobs[0].pts[i].x*ofGetWidth()/320,contourFinder.blobs[0].pts[i].y*ofGetHeight()/240);
                     ofSetColor(151,202,255,200);
                    s->draw(contourFinder.blobs[0].centroid.x*ofGetWidth()/320+enMove4.x,contourFinder.blobs[0].centroid.y*ofGetHeight()/240+enMove4.y,contourFinder.blobs[0].pts[i].x*ofGetWidth()/320,contourFinder.blobs[0].pts[i].y*ofGetHeight()/240);
                }
                
                
                if (!s->isAlive())
                {
                    it4 = myHarmonicOsces.erase(it4);
                    delete s;
                }
                else
                    it4++;
            }
            myHarmonicOsces.push_back(new myHarmonicOsc(contourFinder.blobs[0].centroid.x*ofGetWidth()/320,contourFinder.blobs[0].centroid.y*ofGetHeight()/240,finger.tipPosition().y,true));
            while (myHarmonicOsces.size() > 8) {
                myHarmonicOsces.pop_front();
            }
            myHarmonicOsces.back()->play();
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFinger(Finger finger) {
    
    // 指先の点を描画
    ofPoint tip(finger.tipPosition().x, finger.tipPosition().y, finger.tipPosition().z);
    drawPoint(tip);
    
    // 指の付け根の座標を計算
    ofPoint base = ofPoint(tip.x + finger.direction().x * finger.length() * -1,
                           tip.y + finger.direction().y * finger.length() * -1,
                           tip.z + finger.direction().z * finger.length() * -1);
    /*    // 指の付け根を描画
     drawPoint(base);
     
     // 指先から付け根に線を描く
     ofLine(tip.x, tip.y, tip.z, base.x, base.y, base.z);
     
     // 付け根から掌に線を描く
     ofLine(base.x, base.y, base.z,
     finger.hand().palmPosition().x,
     finger.hand().palmPosition().y,
     finger.hand().palmPosition().z);
     
     // 指の箱を描画
     drawFingerBox(finger, tip, base);
     */
}
//--------------------------------------------------------------点を描画
void ofApp::drawPoint(ofPoint point) {
    
    ofPushMatrix();
    ofTranslate(point);
    ofNoFill();
    ofSetColor(0xFF, 0xFF, 0xFF, 255);
   // ofDrawSphere(3);
    ofPopMatrix();
}
//--------------------------------------------------------------指の箱を描画
void ofApp::drawFingerBox(Finger finger, ofPoint tip, ofPoint base) {
    
    // 指の中間の座標
    ofPoint middle = base.middle(tip);
    ofPushMatrix();
    ofTranslate(middle);
    
    // 指の方向に従い回転
    ofQuaternion quat;
    quat.makeRotate(ofPoint(0, -1, 0), ofPoint(finger.direction().x, finger.direction().y, finger.direction().z));
    ofMatrix4x4 matrix;
    quat.get(matrix);
    glMultMatrixf(matrix.getPtr());
    
    ofNoFill();
    ofSetColor(0xCC,0,0,255);
    ofScale(1, finger.length()/10, 1);
  //  ofDrawBox(10);
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::drawPalm(Hand hand) {
    // 掌の描画処理
    
    ofPoint point = ofPoint(hand.palmPosition().x, hand.palmPosition().y, hand.palmPosition().z);
    drawPoint(point);
    
    ofPushMatrix();
    ofTranslate(point);
    
    // 掌を回転
    ofQuaternion quat;
    quat.makeRotate(ofPoint(0, -1, 0), ofPoint(hand.palmNormal().x, hand.palmNormal().y, hand.palmNormal().z));
    ofMatrix4x4 matrix;
    quat.get(matrix);
    glMultMatrixf(matrix.getPtr());
    
    ofNoFill();
    ofSetColor(0xCC,0x0,0x0,255);
    ofScale(1, 0.25, 1.0);
  //  ofDrawBox(0, 0, 0, 60);
    
    ofPopMatrix();
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
