#include "Audio.h"

void Audio::Initialize()
{
	System_Create(&system);

	system->init(maxCount, FMOD_INIT_NORMAL, NULL);

	sound = new Sound * [maxCount];
	channel = new Channel * [maxCount];

	ZeroMemory(sound, sizeof(Sound*) * maxCount);
	ZeroMemory(channel, sizeof(Channel*) * maxCount);
}

void Audio::Destroy()
{
	if (channel != NULL)
	{
		for (UINT i = 0; i < maxCount; i++)
			if (channel[i]) channel[i]->stop();
	}

	if (sound != NULL)
	{
		for (UINT i = 0; i < maxCount; i++)
			if (sound[i]) sound[i]->release();
	}

	SafeDeleteArray(channel);
	SafeDeleteArray(sound);

	if (system != NULL)
	{
		system->release();
		system->close();
	}

	sounds.clear();
}

void Audio::Update()
{
	system->update();
}

void Audio::AddSound(string name, string file, bool bLoop)
{
	if (soundString.size() >= maxCount)
		return;

	if (bLoop == true)
		system->createStream(file.c_str(), FMOD_LOOP_NORMAL, NULL, &sound[sounds.size()]);
	else
		system->createStream(file.c_str(), FMOD_DEFAULT, NULL, &sound[sounds.size()]);

	sounds[name] = &sound[sounds.size()];
	soundString.push_back(name);
}

void Audio::Play(string name, float volume)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &channel[count]);
			channel[count]->setVolume(volume);
			lastPlayed = name;
		}
	}
}

void Audio::Stop(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->stop();
			break;
		}
	}
}

void Audio::Pause(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(true);
			break;
		}
	}
}

void Audio::Resume(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(false);
			break;
		}
	}
}

bool Audio::Playing(string name)
{
	bool bPlay = false;
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->isPlaying(&bPlay);
			break;
		}
	}

	return bPlay;
}

bool Audio::Paused(string name)
{
	bool bPaused = false;
	int count = 0;
	iter = sounds.begin();

	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->getPaused(&bPaused);
			break;
		}
	}

	return bPaused;
}

vector<string> Audio::NowPlaying()
{
	vector<string> ret;
	int count = 0;
	iter = sounds.begin();

	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (Playing(iter->first))
		{
			ret.push_back(iter->first);
		}
	}

	return ret;
}

void Audio::Volume(string name, float volume)
{
	int count = 0;
	iter = sounds.begin();

	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setVolume(volume);
			break;
		}
	}
}