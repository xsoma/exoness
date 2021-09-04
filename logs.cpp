#include "logs.h"
#include "draw_manager.h"
#include <chrono>

int epoch_time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void logs_put::run() {
	if (logs.empty())
		return;

	auto last_y = 6;
	auto last_x = 4;

	for (unsigned int i = 0; i < logs.size(); i++) {
		auto& log = logs.at(i);

		if (epoch_time() - log.log_time > 3000) {
			float factor = (log.log_time + 3400) - epoch_time();
			factor /= 1000;

			auto opacity = int(255 * factor);

			if (opacity < 2) {
				logs.erase(logs.begin() + i);
				continue;
			}

			log.alpha = opacity;

			log.x += 1 * (factor * 1.25);
			log.y += 1 * (factor * 1.25);
		}

		const auto text = log.message.c_str();

		render::get().draw_text(log.x + 3, last_y + log.y, log.colors, render::get().console_font, false, text);

		last_y += 14;
	}
}

void logs_put::add(std::string text, Color colors) {
	logs.push_front(loginfo_t(epoch_time(), text, colors));
}