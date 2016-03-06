#include "ofApp.h"
#include "const.h"

//--------------------------------------------------------------
void ofApp::setup(){
	myFont.load("frabk.ttf", kNormalFontSize);
    ofSetBackgroundColor(0);
    ofEnableDepthTest();
    ofSetVerticalSync(true);
    ofSetCircleResolution(50);
    for(int v = 0; v < kNumTimeSlices; v++){
        for(int u = 0; u < kNumBins;u++){
            float x = static_cast<float>(u)/static_cast<float>(kNumBins);
            spectrogram.push_back(ofVec3f(x, 0, v * 0.05));
        }
    }

    for(int u = 0; u < kNumBins;u++){
        float x = static_cast<float>(u)/static_cast<float>(kNumBins);
        eqLine3D.push_back(ofVec3f(x, 0, 0));
    }

    spectrumVbo.setVertexData(&spectrogram[0], kNumVertices, GL_DYNAMIC_DRAW);
    eqLineVbo.setVertexData(&eqLine3D[0], kNumBins, GL_DYNAMIC_DRAW);
    ofSoundStreamSetup(kNumInput, kNumOutput, this, kSampleRate, ofxPd::blockSize()*8, 3);
    pd.init(kNumInput, kNumOutput, kSampleRate);
    pd.subscribe("toOF");
    pd.addReceiver(*this);
    pd.openPatch("spectrum.pd");
    pd.start();

    ofSetLineWidth(0.1);

    for(int i = 0; i < kNumBins;i++){
        gains.push_back(ofRandom(0, 0.2));
    }
    ofEnableAlphaBlending();
    for(int i =0;i < 5;i++){
    	touches.push_back(Touch(i,ofToString(i+1), "eq", "freq", "magnitude", "", "", ofColor::white, myFont));
    }
}


void ofApp::print(const std::string& message) {
	ofLog() << message;
}

//--------------------------------------------------------------
void ofApp::update(){
    recordHead++;
    recordHead %= kNumTimeSlices;

	ofVec3f accel = ofxAccelerometer.getForce().getNormalized();
	accel.y *= -1.0;
	std::fill(gains.begin(), gains.end(), 0.0 );
	for(int i = 0 ;i< kMaxTouch;i++ ){
		touches[i].process(touches[0].getPoint(), pd, accel);
		const ofPoint point = touches[i].getInterpolatedPoint();
	    int bin = static_cast<int>(static_cast<float>(point.x) /  ofGetWidth() * static_cast<float>(kNumBins));
	    float gain = 1.0-static_cast<float>(point.y)/1024.0;
	    gains[bin] = gain;
	}
    pd.writeArray("gain", gains);

    std::vector<float> spectrum;
    pd.readArray("spectrum", spectrum);
    spectrum[0] = 0 ;
    int offset = recordHead * kNumBins;

    float yOffset = ofGetHeight() /4 * 3;

    for (int i = 0; i < kNumBins; i++) {
        float x = static_cast<float>(i)/static_cast<float>(kNumBins)*ofGetWidth();
        spectrogram[offset+i] = ofVec3f(x, -spectrum[i] * yOffset + yOffset, 0);
        eqLine3D[i] = ofVec3f(x, gains[i],(kNumTimeSlices-1) * 0.05);
    }

    spectrumVbo.updateVertexData(&spectrogram[0], kNumVertices);
    eqLineVbo.updateVertexData(&eqLine3D[0], kNumBins);

}




//--------------------------------------------------------------
void ofApp::draw(){
	drawTouches();

    for (int i = kNumTimeSlices; i >= 0; i--) {
        int offset = (recordHead + i) % kNumTimeSlices;
        float scale = (static_cast<float>(i) / kNumTimeSlices) / 2.0 + 0.5;
        float alpha = static_cast<float>(i) / static_cast<float>(kNumTimeSlices);
        ofSetColor(ofColor::orange, alpha * alpha * 255);

        ofPushMatrix();
        ofTranslate(ofPoint((1.0-scale)*ofGetWidth()*0.5, i*1.0-ofGetHeight()/5,0));
        ofScale(scale,1.0 );

        spectrumVbo.draw(GL_LINE_STRIP, offset * kNumBins, kNumBins);
        ofPopMatrix();
    }

    ofSetColor(ofColor::white);
    eqLineVbo.draw(GL_LINE_STRIP, 0, kNumBins);
    ofSetColor(255);
}

void ofApp::drawTouches(){
	for(int i = 0;i < kMaxTouch; i++){
		touches[i].draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
	if(id >= kMaxTouch) return;
	if(id == 0){
		touches[id].setPoint(ofPoint(x,y));
		touches[id].setInterpolatedPoint(ofPoint(x,y));
	}else{
		touches[id].setPoint(ofPoint(x,y));
		touches[id].setInterpolatedPoint(touches[0].getInterpolatedPoint());
	}
	touches[id].setStatus(TouchStatus::INTERPOLATED);
	touches[id].rippleIn();
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	if(id >= kMaxTouch) return;
	touches[id].setPoint(ofPoint(x,y));

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
	if(id >=kMaxTouch ) return;
	touches[id].setStatus(TouchStatus::RELEASE);
	touches[id].sendMessage(pd);
	touches[id].rippleOut();
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){


}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	pd.audioIn(input, bufferSize, nChannels);
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels){
	pd.audioOut(output, bufferSize, nChannels);
}
