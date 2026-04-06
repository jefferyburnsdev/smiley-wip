#pragma once
#include "SFML/Audio.hpp"
#include <unordered_map>
#include <string>

using namespace std;

class MusicManager {
	public:
		static MusicManager& getInstance() {
			static MusicManager instance;
			return instance;
		}
		bool playMusic(const string& filename, float volume, bool loop);
		void pauseMusic();
		void startFadeOut(float durationInS);
		void updateFadeOut(float deltaTime);
		void setMasterVol(float vol);
		void setMusicVol(float vol);
		float getMasterVol() const;
		float getMusicVol() const;
	private:
		MusicManager() = default;
		MusicManager(const MusicManager&) = delete;
		MusicManager& operator=(const MusicManager&) = delete;
		sf::Music music;
		float masterVol = 100.f;
		float musicVol = 100.f;
		float initVol = 100.f;
		float fadeTimer = 0.f;
		float fadeDuration = 0.f;;
		bool isFading = false;
		string currentFile;
		string queuedFile;
		float queuedVol = 100.f;
		bool queuedLoop = true;
		bool hasQueuedTrack = false;
};

class SFXManager {
	public:
		bool playSound(const string& filename, float volume = 50.f);
		bool makeLoop(sf::Music& music);
	private:
		unordered_map<string, sf::SoundBuffer> soundBuffers;
		vector<sf::Sound> activeSounds;
};
