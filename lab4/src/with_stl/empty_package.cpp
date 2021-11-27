#include "empty_package.h" 

EmptyPackage::EmptyPackage(const EmptyPackage &ep) noexcept
{
	m_package = ep.m_package;
}

EmptyPackage::EmptyPackage(EmptyPackage &&ep) noexcept
{
	m_package = ep.m_package;
	ep.m_package = nullptr;
}

EmptyPackage& EmptyPackage::operator =(const EmptyPackage &ep)
{
	if (this == &ep) { return *this; }
	
	delete m_package;
	m_package = ep.m_package;
	return *this;
}

EmptyPackage& EmptyPackage::operator =(EmptyPackage &&ep) noexcept
{
	m_package = ep.m_package;
	ep.m_package = nullptr;
	return *this;
}
