#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

namespace AudioManager {

	class Audio {
	private:
		AEAudio bgMusic;
		AEAudio soundEffect;
		AEAudioGroup bgm;
		AEAudioGroup sfx;
		float bgVolume;
		float sfxVolume;
		bool bgMuted;
		bool sfxMuted;

	public:

		void init();

		//bgm
		void loadBGM(const std::string& path);
		void playBGM(float volume, bool loop);
		void setBGMVolume(float volume);
		void toggleBGMMuted();
		//sfx
		void loadSFX(const std::string& path);
		void playSFX(float volume);
		void setSFXVolume(float volume);
		void toggleSFXMuted();

		void exit();
	};
}

#endif