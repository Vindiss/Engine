#include "Clock.h"
#include "Engine.h"

bool Module::Time::Clock::Init()
{
	ComputeDeltaTime();
	return true;
}

bool Module::Time::Clock::Start()
{
	ComputeDeltaTime();
	return true;
}

bool Module::Time::Clock::Update()
{
	ComputeDeltaTime();
	return true;
}

bool Module::Time::Clock::Destruct()
{
	return true;
}

float Module::Time::Clock::GetDeltaTime() const
{
	return static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(currentDeltaTime).count() / 1000.f);
}

void Module::Time::Clock::ComputeDeltaTime()
{
	currentTimePoint = clock.now();
	currentDeltaTime = currentTimePoint - previousTimePoint;
	previousTimePoint = currentTimePoint;
}