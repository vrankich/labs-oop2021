#include "cringe.h" 

namespace cringe {
	Version& Version::set(unsigned _major, unsigned _minor, unsigned _patch)
	{
		major = _major;
		minor = _minor;
		patch = _patch;
		is_set = true;
		return *this;
	}

	std::ostream& Version::operator <<(std::ostream &out)
	{
		/* TODO: think about this */
		if (!is_set) { return out; }
		out << major << "." << minor << "." << patch;
		return out;
	}
}