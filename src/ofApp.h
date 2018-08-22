#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxGPIO.h"
//Define num of ports to listen
#define PORTS 16
#define LEDS 50

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void palette();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();
		//OSC ping ESP
		void pingSender();

		int height;
		int width;
		float size;

		//Scene control variable
		int scene;

		//XML object - edit settings
		ofxXmlSettings XML;
		//OSC ---------------------------
		ofxOscReceiver receiver;
		ofxOscSender sender[PORTS];
		int out_ports[PORTS];
		int in_port;
		string ipESP[PORTS];
		string inTags[PORTS];

		bool esp_state[PORTS];
		int esp_state_control[PORTS];
		string esp_value[PORTS];

		float startTimer;
		float timer;

		//Sound
		ofSoundPlayer  audio[PORTS];
		bool inlet[PORTS];
		bool outlet[PORTS];
		int connection[PORTS][2];
		int numActConnections;
		int toConnect;
		bool pressed;

		//Video
		ofVideoPlayer video;

		LED strip;
		int bright;
		vector<ofVec3f> colors;
};
