#pragma once

#include <SFML\Graphics\Rect.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>

class Animation
{
	public:

						Animation(sf::Vector2i frameSize, int maxFrames, sf::Time frameTime = sf::Time(sf::seconds(0.1f)));
		virtual			~Animation();

		sf::Rect<int>	getFrame() const;
		
		void			play(int animation);
		void			loop(int animation);
		void			stop();

		void			update();

		bool			isPlaying() const;

	private:
		sf::Clock		mFrameTimer;
		sf::Time		mFrameTime;
		sf::Rect<int>	mFrame;

		int				mMaxFrames;
		int				mCurrentAnimation;
		int				mCurrentFrame;

		bool			mLoop;
		bool			mPlaying;
};