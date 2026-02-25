#include "AudioManager.hpp"
#include <AEEngine.h>

namespace AudioManager {

	void Audio::init() {
		//create audio groups for bgm and sfx
		bgm = AEAudioCreateGroup();
		sfx = AEAudioCreateGroup();
	}

	//bgm
	void Audio::loadBGM(const std::string &path) {
		bgMusic = AEAudioLoadMusic(path.c_str());
	}

	void Audio::playBGM(float volume = 1.0f, bool loop = true) {
		bgVolume = volume;
		AEAudioPlay(bgMusic, bgm, bgVolume, 1.f, loop ? -1 : 0);
	}

	void Audio::setBGMVolume(float volume) {
		bgVolume = volume;
		AEAudioSetGroupVolume(bgm, bgVolume);
	}

	//sfx
	void Audio::loadSFX(const std::string& path) {
		soundEffect = AEAudioLoadSound(path.c_str());
	}

	void Audio::playSFX(float volume = 1.0f) {
		sfxVolume = volume;
		AEAudioPlay(soundEffect, sfx, sfxVolume, 1.f, 0);
	}

	void Audio::setSFXVolume(float volume) {
		sfxVolume = volume;
		AEAudioSetGroupVolume(sfx, sfxVolume);
	}

	void Audio::exit() {
		AEAudioUnloadAudio(bgMusic);
		AEAudioUnloadAudio(soundEffect);
		AEAudioUnloadAudioGroup(bgm);
		AEAudioUnloadAudioGroup(sfx);
	}

}