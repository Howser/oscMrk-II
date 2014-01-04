#include "Animation\Animation.h"
#include <iostream>

Animation::Animation(sf::Vector2i frameSize, int maxFrames, sf::Time frameTime)
	:
	mFrame(sf::Vector2i(0, 0), frameSize),
	mFrameTime(frameTime),	
	mFrameTimer(),
	mMaxFrames(maxFrames),
	mCurrentAnimation(0),
	mLoop(false),
	mPlaying(false),
	mCurrentFrame(0)
{
}

Animation::~Animation()
{
}

sf::Rect<int> Animation::getFrame() const
{
	return mFrame;
}

void Animation::play(int animation)
{
	mPlaying = true;
	mLoop = false;

	mFrame.top = animation * mFrame.height;
	mFrame.left = 0;
	mCurrentFrame = 0;
}

void Animation::loop(int animation)
{
	play(animation);
	mLoop = true;
}

void Animation::stop()
{
	mPlaying = false;
	mLoop = false;
	mCurrentFrame = 0;
	mFrame.left = 0;
}

void Animation::update()
{
	if (mPlaying)
	{
		if (mFrameTimer.getElapsedTime() >= mFrameTime)
		{
			if (++mCurrentFrame >= mMaxFrames)
			{
				(mLoop) ? mCurrentFrame = 0 : stop();
			}
			mFrameTimer.restart();
			mFrame.left = mCurrentFrame * mFrame.width;
		}
	}
}

bool Animation::isPlaying() const
{
	return mPlaying;
}