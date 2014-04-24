//
//  CustomPolygon.cpp
//  box2d_UGen_Leap
//
//  Created by 諸岡 光男 on 2013/11/02.
//
//

#include "CustomPolygon.h"

CustomPolygon::CustomPolygon(int _num){
    num = _num;
}

void CustomPolygon::draw(){
    float count;
    ofNoFill();
    
    ofSetColor(102,102,102);
    ofTriangle(getPosition().x,getPosition().y,getPosition().x-15,getPosition().y+30,getPosition().x+15,getPosition().y+30);
    
}
