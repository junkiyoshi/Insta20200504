#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	for (int i = 0; i < 3; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofColor random_color;
	for (auto& noise_seed : this->noise_seed_list) {

		auto location = glm::vec2(ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.003), 0, 1, 100, ofGetWidth() - 100), ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.003), 0, 1, 100, ofGetHeight() - 100));
		auto next = glm::vec2(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.003), 0, 1, 100, ofGetWidth() - 100), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.003), 0, 1, 100, ofGetHeight() - 100));
		auto distance = location - next;
		distance *= 2;

		auto future = location + distance * 30;
		auto random_deg = ofRandom(360);
		future += glm::vec2(30 * cos(random_deg * DEG_TO_RAD), 30 * sin(random_deg * DEG_TO_RAD));
		auto future_distance = future - location;

		this->location_list.push_back(location);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));

		random_color.setHsb(ofRandom(255), 255, 255, 255);
		this->color_list.push_back(random_color);
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;
		if (this->color_list[i].a > 4) { this->color_list[i].a -= 4; }

		if (glm::distance(glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5), this->location_list[i]) > 720) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->location_list.size(); i++) {

		auto& location = this->location_list[i];
		ofSetColor(this->color_list[i]);
		ofDrawCircle(location, 3);
		for (int k = 0; k < this->location_list.size(); k++) {

			if (i == k)continue;
			auto other = this->location_list[k];
			if (glm::distance(location, other) < 50) {

				auto alpha = this->color_list[i].a > this->color_list[k].a ? this->color_list[k].a : this->color_list[i].a;

				ofSetColor(this->color_list[i], alpha);
				ofDrawLine(location, other);
				ofSetColor(this->color_list[k], alpha);
				ofDrawLine(location, other);
			}
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}