	#include "packages.h" 

	void MainPackage::delete_dependencies()
	{
		while(!m_dependencies.empty()) {
			delete m_dependencies.front();
			m_dependencies.pop_front();
		}
	}

	MainPackage::MainPackage(const MainPackage &mp)
	{
		m_installed = mp.m_installed;
		m_name = mp.m_name;
		m_publisher = mp.m_publisher;
		m_version = mp.m_version;
		m_source = mp.m_source;
		m_dependencies = mp.m_dependencies;
	}

	MainPackage::MainPackage(MainPackage &&mp) noexcept
	{
		m_installed = mp.m_installed;
		m_name = std::move(mp.m_name);
		m_version = mp.m_version;
		m_publisher = std::move(mp.m_publisher);
		m_source = std::move(mp.m_source);
		m_dependencies = std::move(mp.m_dependencies);
		mp.delete_dependencies();
	}

	MainPackage& MainPackage::operator =(const MainPackage &mp)
	{
		if (this == &mp) { return *this;}

		delete_dependencies();
		m_installed = mp.m_installed;
		m_name = mp.m_name;
		m_version = mp.m_version;
		m_source = mp.m_source;
		m_dependencies = mp.m_dependencies;
		return *this;
	}

	MainPackage& MainPackage::operator =(MainPackage &&mp) noexcept
	{
		m_installed = mp.m_installed;
		m_name = std::move(mp.m_name);
		m_version = mp.m_version;
		m_publisher = std::move(mp.m_publisher);
		m_source = std::move(mp.m_source);
		m_dependencies = std::move(mp.m_dependencies);
		mp.delete_dependencies(); // do we need it???
		return *this;
	}

	bool MainPackage::operator ==(const MainPackage &mp) noexcept
	{
		if (m_name.empty()) { return false; }
		if (mp.m_name.empty()) { return false; }
		if (m_name != mp.m_name) { return false; }
		return true;
	}

	bool MainPackage::operator !=(const MainPackage &mp) noexcept
	{
		return !(*this == mp);
	}

std::ostream& operator <<(std::ostream &out, const MainPackage &p) noexcept
{
	out << "Installed: ";
	if (p.m_installed) {
		out  << "yes";
	} else {
		out << "no";
	}
	out << std::endl;

	out << "Name: " << p.m_name << std::endl;
	out << "Version: " << p.m_version << std::endl;
	out << "Publisher: " << p.m_publisher << std::endl;

	out << "Source code: " << std::endl; 
	for (size_t i = 0; i < p.m_source.size(); i++) {
		out << p.m_source[i] << std::endl;
	}

	return out;
}

///* add to repository */
//MainPackage& MainPackage::operator ++() noexcept
//{
//
//	return *this;
//}
//
//MainPackage MainPackage::operator ++(int) noexcept
//{
//	MainPackage p_cp(*this);
//
//
//	return p_cp;
//}
//
///* remove from repository */
//MainPackage& MainPackage::operator --() noexcept
//{
//
//	return *this;
//}
//
//MainPackage MainPackage::operator --(int) noexcept
//{
//	MainPackage p_cp(*this);
//
//
//	return p_cp;
//}


	MainPackage& MainPackage::install() noexcept
	{
		if (m_installed) {
			update_version();
			return *this;
		}
		m_installed = true;
		return *this;
	}

	package_operations MainPackage::add_to_repository(ProPro &p)
	{
		return p.add_package(this);
	}

	package_operations MainPackage::remove_from_repository(ProPro &p) noexcept
	{
		return p.remove_package(this);
	}

	MainPackage& MainPackage::update_version() noexcept
	{
		++m_version;
		return *this;
	}

	bool is_in_list(const std::list<MainPackage*> &list, MainPackage *p)
	{
		std::list<MainPackage*>::const_iterator it;
		for (it = list.begin(); it != list.end(); it++) {
			if (*p == **it) { return true; }
		}
		return false;
	}

	MainPackage* MainPackage::unite_packages(std::istream &in, std::list<MainPackage*> &list)
	{
		MainPackage *new_package = new MainPackage;
		
		std::string name;
		in >> new_package->m_name;
		in >> new_package->m_version;
		in >> new_package->m_publisher;
		
		/* input number of source */
		size_t n_source;
		in >> n_source;
		/* input all source one by one */
		std::vector<std::string> temp_source;
		std::string one_source;
		for (size_t i = 0; i < n_source; i++) {
			in >> one_source;
			temp_source.push_back(one_source);
		}
		new_package->m_source = std::move(temp_source);

		std::list<MainPackage*> temp_dep;
		std::list<MainPackage*>::const_iterator it;
		for (it = list.begin(); it != list.end(); it++) {
			if (!is_in_list(temp_dep, *it)) {
				temp_dep.push_back(*it);
			}
		}
		new_package->m_dependencies = std::move(temp_dep);

		return new_package;
	}

	AuxiliaryPackage& AuxiliaryPackage::operator =(const AuxiliaryPackage &ap)
	{
		if (this == &ap) { return *this; }
		
		MainPackage::operator =(ap);
		m_libs = ap.m_libs;
		return *this;
	}

	AuxiliaryPackage& AuxiliaryPackage::operator =(AuxiliaryPackage &&ap) noexcept
	{
		MainPackage::operator =(std::move(ap));
		m_libs = std::move(ap.m_libs);
		return *this;
	}

	bool AuxiliaryPackage::operator ==(const AuxiliaryPackage &ap) noexcept
	{
		if (ap.get_name().empty()) { return false; }
		if (MainPackage::operator !=(ap)) { return false; }
		if (m_libs.size() != ap.m_libs.size()) { return false; }
		for (size_t i = 0; i < ap.m_libs.size(); i++) {
			if (m_libs[i] != ap.m_libs[i]) { return false; }
		}
		return true;
	}

	bool AuxiliaryPackage::operator !=(const AuxiliaryPackage &ap) noexcept
	{
		return !(*this == ap);
	}

	MainPackage *AuxiliaryPackage::unite_packages(std::istream &in, std::list<MainPackage*> &list)
	{
		MainPackage *new_package = MainPackage::unite_packages(in, list);
		
		size_t n_libs;
		in >> n_libs;
		std::vector<std::string> temp_libs;
		std::string lib;
		for (size_t i = 0; i < n_libs; i++) {
			in >> lib;
			if (std::find(temp_libs.begin(), temp_libs.end(), lib) == temp_libs.end()) {
				temp_libs.push_back(lib);
			}
		}
		dynamic_cast<AuxiliaryPackage*>(new_package)->m_libs = temp_libs;
		
		return new_package;
	}

	std::list<MainPackage*> AuxiliaryPackage::separate(unsigned n, const std::vector<std::vector<std::string>> &data)
	{
		if (n != data.size()) {
			throw invalid_vector_size();
		}

		std::list<MainPackage*> parts;
		MainPackage *temp = nullptr;
		for (size_t i = 0; i < data.size(); i++) {
			temp = new AuxiliaryPackage(*this);
			//*temp = *this;
			dynamic_cast<AuxiliaryPackage*>(temp)->set_libs(data[i]);
			parts.push_back(temp);
		}
		return parts;
	}

