#include "structs.h" 

Version::Version(const Version &v)
{
	major = v.major;
	minor = v.minor;
	patch = v.patch;
}

Version& Version::set(unsigned _major, unsigned _minor, unsigned _patch)
{
	major = _major;
	minor = _minor;
	patch = _patch;
	return *this;
}

Version& Version::operator =(const Version &v)
{
	major = v.major;
	minor = v.minor;
	patch = v.patch;
	return *this;
}

Version& Version::operator ++() noexcept
{
	if (patch < 9) { 
		patch++; 
		return *this;
	}
	patch = 0;
	if (minor < 9) {
		minor++;
		return *this;
	}
	minor = 0;
	major++;
	return *this;
}

Version Version::operator ++(int) noexcept
{
	Version temp(*this);
	++(*this);
	return temp;
}

std::ostream& operator <<(std::ostream &out, const Version &v)
{
	out << v.major << "." << v.minor << "." << v.patch;
	return out;
}

std::istream& operator >>(std::istream &in, Version &v)
{
	unsigned n;
	in >> n;
	if (n >= 10) {
		throw invalid_version();
	}
	v.patch = n;
	in >> n;
	if (n >= 10) {
		throw invalid_version();
	}
	v.minor = n;
	in >> n;
	v.major = n;
	return in;
}
