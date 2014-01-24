#pragma once

#include <SFML\Graphics\Rect.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>

class Animation
{
	public:

						Animation(sf::Vector2i frameSize, int maxFrames, float frameTime = (0.1f));
						Animation();
		virtual			~Animation();

		sf::Rect<int>	getFrame() const;
		
		void			play(int animation);
		void			loop(int animation);
		void			stop();

		void			update();

		bool			isPlaying() const;

	private:
		sf::Clock		mFrameTimer;
		float		mFrameTime;
		sf::Rect<int>	mFrame;

		int				mMaxFrames;
		int				mCurrentAnimation;
		int				mCurrentFrame;

		bool			mLoop;
		bool			mPlaying;
};