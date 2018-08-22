#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

  //Init window settings
  ofSetWindowTitle("Oasis Control");
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
  ofSetBackgroundColor(0);

  height = ofGetHeight();
  width = ofGetWidth();
  size = (height / PORTS)-20;

  if( XML.loadFile("data/settings.xml") ){
    ofLog() << "settings.xml loaded.";
  }else{
    ofLog() << "unable to load settings.xml check data/ folder";
  }

  in_port  = XML.getValue("OSCPORTS:START:IN", 4000);
  int portTempOut  = XML.getValue("OSCPORTS:START:OUT", 5000);

  receiver.setup(in_port);

  // Init tag addresses for OSC
  string inTagGen = "/ESP_";

  //audio - Temporal
  //audio[0].load("data/audios/a.wav");
  audio[1].load("data/audios/b.wav");
  audio[2].load("data/audios/c.wav");
  audio[3].load("data/audios/d.wav");
  audio[4].load("data/audios/e.wav");
  audio[5].load("data/audios/f.wav");
  audio[6].load("data/audios/g.wav");
  audio[7].load("data/audios/h.wav");
  audio[8].load("data/audios/i.wav");
  audio[9].load("data/audios/j.wav");
  audio[10].load("data/audios/k.wav");
  audio[11].load("data/audios/l.wav");
  audio[12].load("data/audios/m.wav");
  audio[13].load("data/audios/n.wav");
  audio[14].load("data/audios/o.wav");
  audio[15].load("data/audios/p.wav");

  //video - Temporal
  video.load("data/videos/fingers.mov");

  //Init all variables
  for(int i = 0; i < PORTS; i++){
    //tags for ESP-OSC
    inTags[i] = inTagGen + ofToString(i);
    //init all ESP communication state to false
    esp_state[i] = false;
    //Different out ports to control ESP-state individually (5000, 5001 ...)
    out_ports[i] = portTempOut + i;

    ipESP[i] = XML.getValue("OSCPORTS:START:IP" + ofToString(i), "127.0.0.1");
    sender[i].setup(ipESP[i], out_ports[i]);

    //to check esp osc response
    esp_state_control[i] = 0;
    esp_value[i] = "nill";

    inlet[i] = false;
    outlet[i] = false;
    connection[i][0] = -1;
    connection[i][1] = -1;
    toConnect = -1;

    audio[i].stop();
  }
  startTimer = ofGetElapsedTimeMillis();

  pressed = false;
  numActConnections = 0;
  toConnect = false;

  scene = 1;
}

//--------------------------------------------------------------
void ofApp::update(){

  ofSoundUpdate();
  if(scene == 2){
    video.update();
  }

  timer = ofGetElapsedTimeMillis() - startTimer;
  if(timer >= 1000){
    pingSender();
    startTimer = ofGetElapsedTimeMillis();
  }
  for(int i = 0; i < PORTS; i++){
    if(esp_state_control[i] >= 5){
      esp_state[i] = false;
      esp_value[i] = "nill";
    }
  }

  while(receiver.hasWaitingMessages()){

		ofxOscMessage m;
		receiver.getNextMessage(m);

    for(int i = 0; i < PORTS; i++){
      if(m.getAddress() == inTags[i]){
        esp_state[i] = true;
        esp_state_control[i] = 0;
        esp_value[i] = ofToString(m.getArgAsInt(0));
        if(connection[i][0] != -1 && connection[i][1] != -1){
          if(esp_value[i] == "1" && audio[connection[i][1]].isLoaded()){
            audio[connection[i][1]].play();
          }
          if(esp_value[i] == "1" && video.isLoaded()){
            video.play();
          }
        }
      }
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  if(scene == 1){
    ofFill();
    ofTranslate(50,50);
    for(int i = 0; i < PORTS; i++){
      if(esp_state[i]){
        ofSetColor(ofColor::green);
        ofDrawRectangle(0, (40 * i), size, size);
      } else {
        ofSetColor(ofColor::red);
        ofDrawRectangle(0, (40 * i), size, size);
      }
      ofSetColor(ofColor::white);
      ofDrawBitmapString(esp_value[i],40,(40 * i) + 17);
      //OSC outlets
      ofDrawCircle(85,(40 * i) + 14,3);
      //trigger inlet
      ofDrawCircle((width/4)*3,(40 * i) + 14,3);


      if(audio[i].isLoaded()){
        ofSetColor(ofColor::green);
        ofDrawRectangle(((width/4)*3)+10, (40 * i), size, size);
        ofDrawBitmapString("Audio file",(width/4)*3 + 45,(40 * i) + 17);
      } else {
        /*ofSetColor(ofColor::red);
        ofDrawRectangle(((width/4)*3)+10, (40 * i), size, size);*/
        ofSetColor(ofColor::green);
        ofDrawRectangle(((width/4)*3)+10, (40 * i), size, size);
        ofDrawBitmapString("Video file",(width/4)*3 + 45,(40 * i) + 17);
      }

      //drawing connections btw inlet and outlet
      if(inlet[i] == true && pressed &&  connection[i][1] == -1 && connection[i][0] == toConnect){
        ofSetColor(ofColor::yellow);
        ofDrawLine(85, (40 * i) + 14,ofGetMouseX()-50,ofGetMouseY()-50);
      }
      if(connection[i][0] != -1 && connection[i][1] != -1){
        if(esp_value[i] == "nill"){
          ofSetColor(ofColor::red);
        } else{
          ofSetColor(ofColor::green);
        }
        ofDrawLine(85, (40 * connection[i][0]) + 14,(width/4)*3,(40 * connection[i][1]) + 14);
      }
    }
  }
  if(scene == 2){
    ofSetColor(ofColor::white);
    video.draw(0,0,width,height);
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  switch (key){
    case 49:
      scene = 1;
      break;
    case 50:
      scene = 2;
      break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  pressed = true;
  if(x > 130 && x < 138 && button == 0){
    for(int i = 0; i < PORTS; i++){
      if(y > (40 * i) + 60 && y < (40 * i) + 68){
        toConnect = i;
        inlet[i] = true;
        connection[i][0] = i;
      }
    }
  }
  if(x > 130 && x < 138 && button == 2){
    for(int i = 0; i < PORTS; i++){
      if(y > (40 * i) + 60 && y < (40 * i) + 68){
        inlet[i] = false;
        connection[i][0] = -1;
        connection[i][1] = -1;
      }
    }
  }
  if(x > ((width/4)*3)+45 && x < ((width/4)*3)+55 && button == 2){
    for(int i = 0; i < PORTS; i++){
      if(y > (40 * i) + 60 && y < (40 * i) + 68){
        outlet[i] = false;
        connection[i][1] = -1;
      }
    }
  }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
  pressed = false;
  if(x > ((width/4)*3)+45 && x < ((width/4)*3)+55 && button == 0 && toConnect != -1){
    for(int i = 0; i < PORTS; i++){
      if(y > (40 * i) + 60 && y < (40 * i) + 68){
        outlet[i] = true;
        connection[toConnect][1] = i;
      }
    }
  }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
  height = h;
  width = w;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void ofApp::pingSender(){
  for(int i = 0; i < PORTS; i++){
    ofxOscMessage m;
    m.setAddress("/ping");
    m.addIntArg(1);
    sender[i].sendMessage(m);
    esp_state_control[i]++;
  }
}
