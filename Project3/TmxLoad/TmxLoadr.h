#pragma once

#include <string>
#include <vector>

#define TMX_VERSION "1.4"

namespace Loader
{
	class TmxLoadr
	{
	public:
		TmxLoadr();
		TmxLoadr(const char* filename);
		~TmxLoadr();
	private:
		bool Load(const char* filename);
	};
}

