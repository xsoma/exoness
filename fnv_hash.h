#include <Windows.h>

using fnv_t = unsigned;

class fnv_hash {
private:
	template < unsigned int len >
	static constexpr fnv_t fnv_hash_const(const char(&str)[len], unsigned int I = len) {
		return (fnv_t)(1ULL * (I == 1 ? (2166136261 ^ str[0]) : (fnv_hash_const(str, I - 1) ^ str[I - 1])) * 16777619);
	}

	static fnv_t get_fnv_hash(const char* str) {
		const auto length = strlen(str) + 1;
		uintptr_t hash = 2166136261;

		for (size_t i = 0; i < length; ++i) {
			hash ^= *str++;
			hash *= 16777619;
		}

		return hash;
	}

	struct wrapper {
		wrapper(const char* str) : str(str) { }
		const char* str = nullptr;
	};

	fnv_t hash_value;
public:
	fnv_hash(wrapper wrapper) : hash_value(get_fnv_hash(wrapper.str)) {

	}

	template < unsigned int len >
	constexpr fnv_hash(const char(&str)[len]) : hash_value(fnv_hash_const(str)) {

	}

	constexpr operator fnv_t() const {
		return this->hash_value;
	}
};