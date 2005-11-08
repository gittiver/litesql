#include "../library/compatibility.hpp"
#include "xmlobjects.hpp"
namespace xml {
string safe(const char * s) {
	if (s)
		return s;
	return "";
}
}
