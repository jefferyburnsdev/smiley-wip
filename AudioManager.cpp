#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
#include "AudioManager.h"

using namespace std;

bool MusicManager::playMusic(const string& filename, float volume, bool loop) {
    if (isFading) {
        queuedFile = filename;
        queuedVol = volume;
        queuedLoop = loop;
        hasQueuedTrack = true;
        return false;
    }

    if (music.getStatus() == sf::Music::Playing && filename == currentFile)
        return true;

	if (!music.openFromFile(filename))
		return false;

	music.setVolume(volume);
	music.setLoop(loop);
	music.play();
    currentFile = filename;
	return true;
}

void MusicManager::pauseMusic() {
	music.stop();
}

void MusicManager::startFadeOut(float durationInS) {
    fadeDuration = durationInS;
    fadeTimer = 0.f;
    initVol = music.getVolume();
    isFading = true;
}

void MusicManager::updateFadeOut(float deltaTime) {
    if (!isFading) return;

    fadeTimer += deltaTime;
    float t = fadeTimer / fadeDuration;
    if (t >= 1.f) {
        music.setVolume(0.f);
        music.stop();
        isFading = false;

        if (hasQueuedTrack) {
            playMusic(queuedFile, queuedVol, queuedLoop);
            hasQueuedTrack = false;
        }
        return;
    }
    
    float newVolume = initVol * (1.f - t);
    music.setVolume(newVolume);
}

void MusicManager::setMasterVol(float vol) {
    masterVol = std::clamp(vol, 0.f, 100.f);
    music.setVolume(masterVol * (musicVol / 100.f));
}

void MusicManager::setMusicVol(float vol) {
    musicVol = std::clamp(vol, 0.f, 100.f);
    music.setVolume(masterVol * (musicVol / 100.f));
}

float MusicManager::getMasterVol() const {
    return masterVol;
}

float MusicManager::getMusicVol() const {
    return musicVol;
}