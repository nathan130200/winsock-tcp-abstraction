#include <string>
#include <cstdarg>
#include <exception>

#ifndef HAVE_STRFMT_H
#define HAVE_STRFMT_H

struct StrFmt {
private:
	std::string result;

public:

	StrFmt(const char* fmt, ...)
	{
		va_list ap;
		va_start(ap, fmt);

		size_t len = vsnprintf(NULL, 0, fmt, ap);
		char* temp = new char[len];

		if (temp == NULL)
			throw std::exception("Cannot allocate string formatter block.");

		len = vsprintf(temp, fmt, ap);
		result = std::string(temp, len);
	}

	~StrFmt() = default;

	inline const char* Get() const {
		return result.c_str();
	}

	inline operator std::string() {
		return result;
	}
};

#endif