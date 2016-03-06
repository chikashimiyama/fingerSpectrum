#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#include "ofxAccelerometer.h"
#include "const.h"
#include "Touch.h"
#include "ofxPd.h"

using namespace pd;

class ofApp : public ofxAndroidApp,  public PdReceiver{


	public:
		
		void setup();

		void update();
		void draw();
		

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);

		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();
		void audioIn(float * input, int bufferSize, int nChannels);
		void audioOut(float * output, int bufferSize, int nChannel);
	protected:

		std::vector<Touch> touches;
		ofxPd pd;
		std::vector<ofVec3f> spectrogram;
	    std::vector<ofVec3f> eqLine3D;

	    ofVbo spectrumVbo;
	    ofVbo eqLineVbo;
	    std::vector<float> gains;
	    int recordHead;
		void print(const std::string& message);

		void glSetup();
		void audioSetup();
		void storageSetup();

		void drawTouches();
		void drawSpectrogram();
		ofTrueTypeFont myFont;

};
