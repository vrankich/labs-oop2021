#pragma once

#include "packages.h" 

class MainPackage;

class EmptyPackage {
private:
	MainPackage *m_package;
public:
	EmptyPackage(MainPackage *mp)
		: m_package(mp) {}
	EmptyPackage(const EmptyPackage&) noexcept;
	EmptyPackage(EmptyPackage&&) noexcept;
	~EmptyPackage() = default;
	
	EmptyPackage& operator =(const EmptyPackage&);
	EmptyPackage& operator =(EmptyPackage&&) noexcept;
	
	MainPackage *get_package() const noexcept { return m_package; }
};
