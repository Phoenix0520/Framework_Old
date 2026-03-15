#pragma once

#include "Framework.h"

using namespace FMOD;

class Audio : public Singleton<Audio>
{
private:
	Audio() {}
	Audio(const Audio& other) {}
	~Audio() {}

public:
	friend Singleton;

	virtual void Initialize()	override;
	virtual void Destroy()		override;
	virtual void Update()		override;

public:
	void AddSound(string name, string file, bool bLoop = false);
	void Play(string name, float volume = 1.0f);
	void Stop(string name);
	void Pause(string name);
	void Resume(string name);

	bool Playing(string name);
	bool Paused(string name);

	string LastPlayed() { return lastPlayed; };
	vector<string> NowPlaying();
	vector<string> GetSoundString() { return soundString; }

	void Volume(string name, float volume);
	float Volume() { return volume; }

private:
	System* system;
	Sound** sound;
	Channel** channel;

	UINT maxCount = 50;

	map<string, Sound**> sounds;
	map<string, Sound**>::iterator iter;

	float volume = 1.0f;

	vector<string> soundString;
	string lastPlayed = "Not Found";
};