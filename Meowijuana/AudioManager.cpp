#include "pch.h"
#include "AudioManager.hpp"

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
		if (!bgMuted) {
			AEAudioSetGroupVolume(bgm, bgVolume);
		}
	}

	void Audio::toggleBGMMuted() {
		if (!bgMuted) {
			AEAudioSetGroupVolume(bgm, 0.0f);
			bgMuted = true;
		}
		else {
			AEAudioSetGroupVolume(bgm, bgVolume);
			bgMuted = false;
		}
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
		if (!sfxMuted) {
			AEAudioSetGroupVolume(sfx, sfxVolume);
		}
	}

	void Audio::toggleSFXMuted() {
		if (!sfxMuted) {
			AEAudioSetGroupVolume(sfx, 0.0f);
			sfxMuted = true;
		}
		else {
			AEAudioSetGroupVolume(sfx, sfxVolume);
			sfxMuted = false;
		}
	}

	void Audio::exit() {
		AEAudioUnloadAudio(bgMusic);
		AEAudioUnloadAudio(soundEffect);
		AEAudioUnloadAudioGroup(bgm);
		AEAudioUnloadAudioGroup(sfx);
	}

}

