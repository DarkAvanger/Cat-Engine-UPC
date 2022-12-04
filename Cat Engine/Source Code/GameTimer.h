#pragma once

#include "JsonParse.h"

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	void Start();
	void FinishUpdate();

	inline void SetDesiredCappedMs(int miliseconds) { cappedMs = miliseconds; }
	inline void SetDesiredDeltaTime(float dt) { deltaTime = dt; }

	int GetEngineTimeStartup() const;
	int GetTime() const;
	inline int GetFramesSinceStartup() const { return frameCounter; }
	inline float GetDeltaTime() const { return deltaTime; }
	inline int GetFps() const { return (1.0f / ((float)cappedMs) * 1000.0f); }

	void ReadConfig(JsonParsing& node);
	void SaveConfig(JsonParsing& node);
	void ResetTimer();
private:
	int timer;
	float deltaTime;
	int frameCounter;
	int lastFrameMs;
	int cappedMs;

	int gameTimer;
	bool gameStarted;

	JsonParsing jsonFile;
};