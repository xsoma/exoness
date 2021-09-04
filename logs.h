#pragma once
#include "main_entity.h"
#include <deque>

class logs_put : public singleton <logs_put>
{
public:

	void run();
	void add(std::string text, Color colors);

private:

	struct loginfo_t {
		loginfo_t(const float log_time, std::string message, const Color colors) {
			this->log_time = log_time;
			this->message = message;
			this->colors = colors;

			this->x = 2.f;
			this->y = 0.f;
			this->change_alpha = false;
		}

		float log_time;
		std::string message;
		Color colors;
		float x, y;
		bool change_alpha;
		int alpha = -1;
	};

	std::deque< loginfo_t > logs;

};
