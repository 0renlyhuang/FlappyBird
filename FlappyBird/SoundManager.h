#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <memory>
#include <vector>
#include <iostream>
#include "freealut\alut.h"
#include "OpenAL\al.h"
#include "OpenAL\alc.h"

// 音频管理类
class SoundManager {
public:
	class SoundException {};

	static bool setUp(int argc, char *argv[]) {
		if (isSetUp)
			return false;
		command.first = argc;
		command.second = argv;
		isSetUp = true;
		return true;
	}

	// Singleton
	// 单例模式
	static std::shared_ptr<SoundManager> instance() {
		if (isSetUp) {
			if (!pManager)
				pManager = std::shared_ptr<SoundManager>(new SoundManager(command.first, command.second));

			return pManager;
		}

		else {
			std::cerr << "ERROR: in " << __FILE__
				<< " line " << __LINE__
				<< ": SoundManager is not set up yet." << std::endl;
			throw SoundException();
		}
	}

	// retrun index of the file
	// 返回音频的索引
	std::size_t load(const char *fileName) {
		ALuint buffer = alutCreateBufferFromFile(fileName);

		ALuint source;
		alGenSources(1, &source);
		ALfloat sourcePos[] = { 0.0, 0.0, 0.0 }; // 源声音的位置 	
		ALfloat sourceVel[] = { 0.0, 0.0, 0.0 }; // 源声音的速度  
		alSourcei(source, AL_BUFFER, buffer);
		alSourcef(source, AL_PITCH, 1.0f);
		alSourcef(source, AL_GAIN, 1.0f);
		alSourcefv(source, AL_POSITION, sourcePos);
		alSourcefv(source, AL_VELOCITY, sourceVel);

		sounds.push_back({ source, buffer });
		return sounds.size() - 1;
	}


	void play(std::size_t index) {
		alSourcePlay(sounds[index].first);
	}


	SoundManager(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager &operator=(const SoundManager&) = delete;
	SoundManager &operator=(SoundManager&&) = delete;

	~SoundManager() {
		for (auto &p : sounds) {
			alDeleteBuffers(1, &p.second);
			alDeleteSources(1, &p.first);
		}
	}
private:
	SoundManager(int argc, char *argv[]) {
		if (!alutInit(&argc, argv)) {
			fprintf(stderr, "ALUT error: %s\n",
				alutGetErrorString(alutGetError()));
			exit(EXIT_FAILURE);
		}

		ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
		ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
		ALfloat listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 }; // (first 3 elements are "at", second 3 are "up")  
		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);
	}
	static std::shared_ptr<SoundManager> pManager;
	static std::vector<std::pair<ALuint, ALuint>> sounds;  // source & buffer 
	static std::pair<int, char**> command;
	static bool isSetUp;
};

std::shared_ptr<SoundManager> SoundManager::pManager = nullptr;
std::vector<std::pair<ALuint, ALuint>> SoundManager::sounds;
std::pair<int, char**> SoundManager::command;
bool SoundManager::isSetUp = false;


#endif // !SOUNDMANAGER_H

