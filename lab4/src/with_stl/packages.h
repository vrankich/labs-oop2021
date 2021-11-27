#pragma once

#include "pm.h" 
#include "structs.h" 

class ProPro;

class MainPackage {
private:
	void delete_dependencies();
protected:
	bool m_installed;
	std::string m_name;
	Version m_version;
	std::string m_publisher;
	std::vector<std::string> m_source;
	std::list<MainPackage*> m_dependencies;
public:
	MainPackage() : m_installed(false) {}
	MainPackage(const std::string &name, const Version &v, const std::string &publisher, 
		const std::vector<std::string> &source, const std::list<MainPackage*> &dep)
		: m_installed(false)
		, m_name(name)
		, m_version(v)
		, m_publisher(publisher)
		, m_source(source)
		, m_dependencies(dep) {}
	MainPackage(const MainPackage&);
	MainPackage(MainPackage&&) noexcept;
	virtual ~MainPackage() noexcept {} 
	
	virtual MainPackage& operator =(const MainPackage&);
	virtual MainPackage& operator =(MainPackage&&) noexcept;
	bool operator ==(const MainPackage&) noexcept;
	bool operator !=(const MainPackage&) noexcept;
	friend std::ostream& operator <<(std::ostream&, const MainPackage&) noexcept;

//	/* add to repository */
//	MainPackage& operator ++() noexcept;
//	MainPackage operator ++(int) noexcept;
//	/* remove from repository */
//	MainPackage& operator --() noexcept;
//	MainPackage operator --(int) noexcept;
	
	/* getters */
	bool is_installed() const noexcept { return m_installed; }
	std::string get_name() const noexcept { return m_name; }
	Version get_version() const noexcept { return m_version; }
	std::string get_publisher() const noexcept { return m_publisher; }
	std::vector<std::string> get_source() const noexcept { return m_source; }
	std::list<MainPackage*> get_dependencies() const noexcept { return m_dependencies; }
	
	/* setters */
	void set_name(const std::string &name) noexcept { m_name = name; }
	void set_version(const Version &version) noexcept { m_version = version; }
	void set_publisher(const std::string &publ) noexcept { m_publisher = publ; }
	void set_source(const std::vector<std::string> &s) noexcept { m_source = s; }
	void set_dependencies(const std::list<MainPackage*> &d) noexcept
		{ delete_dependencies(); m_dependencies = d; }

	MainPackage& install() noexcept;
	MainPackage& add_to_repository(ProPro&);
	package_operations remove_from_repository(ProPro&) noexcept;
	MainPackage& update_version() noexcept;
	virtual MainPackage *unite_packages(std::istream&, std::list<MainPackage*>&);
};

class AuxiliaryPackage: public MainPackage {
private:
	std::vector<std::string> m_libs;
public:
	AuxiliaryPackage() : MainPackage() {}
	AuxiliaryPackage(const std::string &name, const Version &v, const std::string &publisher, 
		const std::vector<std::string> &source, const std::list<MainPackage*> &dep, const std::vector<std::string> &libs)
		: MainPackage(name, v, publisher, source, dep)
		, m_libs(libs) {}
	AuxiliaryPackage(const AuxiliaryPackage &ap) 
		: MainPackage(ap) { m_libs = ap.m_libs; }
	AuxiliaryPackage(AuxiliaryPackage &&ap) noexcept
		: MainPackage(ap) { m_libs = std::move(ap.m_libs); }
	~AuxiliaryPackage() = default;
	
	AuxiliaryPackage& operator =(const AuxiliaryPackage&);
	AuxiliaryPackage& operator =(AuxiliaryPackage&&) noexcept;
	bool operator ==(const AuxiliaryPackage&) noexcept;
	bool operator !=(const AuxiliaryPackage&) noexcept;
	//friend std::ostream& operator <<(std::ostream&, const AuxiliaryPackage&) noexcept;

	std::vector<std::string> get_libs() { return m_libs; }
	void set_libs(const std::vector<std::string> &libs) { m_libs = libs; }
	MainPackage *unite_packages(std::istream&, std::list<MainPackage*>&) override;
	std::list<MainPackage*> separate(unsigned, const std::vector<std::vector<std::string>>&);
};

