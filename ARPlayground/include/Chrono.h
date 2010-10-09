#ifndef Chrono_h__
#define Chrono_h__

#pragma once

class Chrono
{	
	public:
		Chrono(bool _autostart = false);

		void start();		
		unsigned int getTimeElapsed(); //in ms

	private:
		__int64 mFreq;
		__int64 mStart;
};
#endif // Chrono_h__