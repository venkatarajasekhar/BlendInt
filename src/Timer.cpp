/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <iostream>
#include <errno.h>
#include <string.h>

#include <BlendInt/Timer.hpp>

namespace BlendInt {

	Timer::Timer()
	: m_id (0), m_interval(40), m_enabled(false)
	{
		Create();
	}

	Timer::~Timer()
	{
#ifdef __UNIX__
#ifndef __APPLE__
		timer_delete(m_id);
#else
		// TODO: find implementation in Mac OS
#endif	// __APPLE__
#endif	// __UNIX__
	}

	void Timer::Start ()
	{
#ifdef __UNIX__
#ifndef __APPLE__
		int ret = -1;
		struct itimerspec ts;

		unsigned int sec = m_interval / 1000;
		long nsec = (m_interval % 1000) * 1000 * 1000;

		ts.it_interval.tv_sec = sec;
		ts.it_interval.tv_nsec = nsec;
		ts.it_value.tv_sec = sec;
		ts.it_value.tv_nsec = nsec;

		ret = timer_settime(m_id, 0, &ts, 0);

		if(ret < 0) {
			std::cout << "Fail to start timer in " << __func__ << std::endl;
			m_enabled = false;
		} else {
			m_enabled = true;
		}
#else
		// TODO: code for Mac OS X
#endif
#endif
	}

	void Timer::Stop ()
	{
#ifdef __UNIX__
#ifndef __APPLE__
		if(! m_enabled) return;

		int ret = -1;
		struct itimerspec ts;
		memset (&ts, 0, sizeof(ts));

		ret = timer_settime(m_id, 0, &ts, 0);

		if(ret < 0) {
			std::cerr << "Fail to stop timer in " << __func__ << std::endl;
		}

		m_enabled = false;
#else
		// TODO: code for Mac OS X
#endif	// __APPLE__
#endif	// __UNIX__
	}

	void Timer::SetInterval(unsigned int interval)
	{
		if(m_interval == interval) return;

		m_interval = interval;

		if(m_enabled)	Start ();
	}

	unsigned int Timer::GetTimeLeft()
	{
#ifdef __UNIX__

#ifdef __LINUX__
		if(!m_enabled)
			return 0;

		struct itimerspec ts;

		int ret = -1;

		ret = timer_gettime (m_id, &ts);

		if(ret < 0) {
			std::cerr << "Fail to get timer in " << __func__ << std::endl;
		}

		return ts.it_value.tv_nsec / 1000 / 1000;
#endif	// __LINUX__
		
#ifdef __APPLE__
		return 0;
#endif	// __APPLE__

#endif	// __UNIX__
	}

	void Timer::Create()
	{
#ifdef __UNIX__

#ifdef __LINUX__
		int ret = -1;
		struct sigevent sev;
		sev.sigev_notify = SIGEV_THREAD;
		sev.sigev_value.sival_ptr = this;
		sev.sigev_notify_function = ThreadCallback;
		sev.sigev_notify_attributes = 0;

		ret = timer_create(CLOCK_REALTIME, &sev, &m_id);
		if(ret < 0) {
			// do error
			std::cerr << "Fail to create timer in " << __func__ << std::endl;
			if(ret == EAGAIN) {
				std::cerr << "The calling process has already created all of the timers it is allowed by this implementation" << std::endl;
			}
		}
#endif	// __LINUX__
		
#ifdef __APPLE__
		// TODO: code for Mac OS X
#endif	// __APPLE__

#endif	// __UNIX__
	}

	void Timer::ThreadCallback(union sigval sigev_value)
	{
		Timer* timer = static_cast<Timer*>(sigev_value.sival_ptr);
		timer->m_timeout.fire();
	}

}
