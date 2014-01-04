#pragma once
#include "Math\GeneralMath.h"

int math::random(int low, int high){
	if (low > high){
		int temp = low;
		low = high;
		high = temp;
	}
	return low + (std::rand() % (high - low + 1));
}

int math::random(int low, int high, int exclude){
	if (low > high){
		int temp = low;
		low = high;
		high = temp;
	}
	int i = low + (std::rand() % (high - low + 1));
	if (i == exclude)
	{
		random(low, high, exclude);
	}
	return i;
}

float math::toDegrees(float x)
{
	return x * 180/std::_Pi;
}