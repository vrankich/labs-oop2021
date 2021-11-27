#include "pm.h" 

namespace PackageManager {
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
		major++;
		return *this;
	}

	Version Version::operator ++(int) noexcept
	{
		Version temp(*this);
		(*this)++;
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
		if (n >= 100) {
			throw invalid_version();
		}
		v.minor = n;
		in >> n;
		if (n >= 100) {
			throw invalid_version();
		}
		v.major = n;
		in >> n;
		v.patch = n;
		return in;
	}

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
		if (mp.m_name.empty()) { return false; }
		if (m_name != mp.m_name) { return false; }
		return true;
	}

	bool MainPackage::operator !=(const MainPackage &mp) noexcept
	{
		return !(*this == mp);
	}

	MainPackage& MainPackage::install() noexcept
	{
		if (m_installed) {
			update();
			return *this;
		}
		m_installed = true;
		return *this;
	}

	MainPackage& MainPackage::add_to_repository(ProPro &p)
	{
		p.add_package(this);
		return *this;
	}

	bool MainPackage::remove_from_repository(ProPro &p) noexcept
	{
		if (p.remove_package(this)) { return true; }
		return false;
	}

	MainPackage& MainPackage::update() noexcept
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
			*temp = *this;
			dynamic_cast<AuxiliaryPackage*>(temp)->set_libs(data[i]);
			parts.push_back(temp);
		}
		return parts;
	}

	EmptyPackage::EmptyPackage(const EmptyPackage &ep) noexcept
	{
		m_name = ep.m_name;
		m_package = ep.m_package;
	}

	EmptyPackage::EmptyPackage(EmptyPackage &&ep) noexcept
	{
		m_name = std::move(ep.m_name);
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
		m_name = std::move(ep.m_name);
		m_package = ep.m_package;
		ep.m_package = nullptr;
		return *this;
	}

	package_operations ProPro::remove_package(MainPackage *mp) noexcept
	{
		return remove_package(mp);
	}
	
	package_operations ProPro::install_package_auto(const EmptyPackage &ep)
	{
		MainPackage *package = ep.get_package();
		
		if (!package) { return NULL_P; }
		if (!m_graph.is_package_in_graph(package)) { return NOT_FOUND; }

		if (package->is_installed()) {
			package->update();
			return EXISTS_IN_PM;
		}

		package->install();
		std::list<MainPackage*> dep = package->get_dependencies();
		std::list<MainPackage*>::iterator it;
		/* install all dependencies needed for the package */
		for(it = dep.begin(); it != dep.end(); it++) {
			(*it)->install();
		}
		return INSTALLED;
	}

	void Graph::delete_graph()
	{
		std::list<MainPackage*> temp;
		for (size_t i = 0; i < m_graph.size(); i++) {	
			temp = m_graph.front();
			while (!temp.empty()) {
				delete temp.front();
				temp.pop_front();
			}
		}
	}
	
	Graph::Graph(std::list<MainPackage*> &list)
	{
		if (list.empty()) { return; }
		
		std::list<MainPackage*>::const_iterator it;
		for (it = list.begin(); it != list.end(); it++) {
			add_package(*it);
		}
	}
	
	/* get index in the vector of lists of the package */ 
	int Graph::index_in_graph(const MainPackage *p) const noexcept
	{
		if (!p) { return -1; }
		for (size_t i = 0; i < m_graph.size(); i++) {
			if (*(m_graph[i].front()) == *p) { return i; }
		}
		return -1;
	}
	
	/* check whether adding a package would form a cycle */
	bool Graph::is_reachable(const MainPackage *p1, const MainPackage *p2) const noexcept
	{
		if (p1 == p2) { return true; }
		
		bool *is_visited = new bool[m_graph.size()];
		for (size_t i = 0; i < m_graph.size(); i++) {
			is_visited[i] = false;
		}
	
		std::list<int> q;
		int p1_index = index_in_graph(p1);
		int p2_index = index_in_graph(p2);
		is_visited[p1_index] = true;
		q.push_back(p1_index);
		std::list<MainPackage*>::const_iterator it;
		while (!q.empty()) {
			p1_index = q.front();
			q.pop_front();
			for(it = m_graph[p1_index].begin(); it != m_graph[p1_index].end(); it++) {
				int i = index_in_graph(*it);
				if (i == p2_index) { 
					delete [] is_visited;
					return true; 
				}
				if (!is_visited[i]) {
					is_visited[i] = true;
					q.push_back(i);
				}
			}
		}
		delete [] is_visited;
	
		return false;
	}
	
	package_operations Graph::add_package(MainPackage *p)
	{
		if (!p) { return NULL_P; }
		if (is_package_in_graph(p)) { return EXISTS_IN_PM; } // maybe throw an exception
		
		std::list<MainPackage*>::const_iterator it;
		std::list<MainPackage*> dependencies = p->get_dependencies();
		/* graph should be without cycles */
		for (it = dependencies.begin(); it != dependencies.end(); it++) {
			/* check whether adding a package would form a cycle */
			if(is_reachable(p, *it)) { return CONFLICT; }
		}
		/* add new vertex with edges to graph */
		dependencies.push_front(p);
		m_graph.push_back(dependencies);
		
		return ADDED;
	}
	
	std::ostream& Graph::update_packages(std::ostream &out) noexcept
	{
		if (m_graph.empty()) { return out; }
		
		bool *is_visited = new bool[m_graph.size()];
		for (size_t i = 0; i < m_graph.size(); i++) {
			is_visited[i] = false;
		}
		
		std::list<int> q;
		int p_index = index_in_graph(m_graph[0].front());
		is_visited[p_index] = true;
		q.push_back(p_index);
		std::list<MainPackage*>::const_iterator it;
		while (!q.empty()) {
			p_index = q.front();
			q.pop_front();
			for(it = m_graph[p_index].begin(); it != m_graph[p_index].end(); it++) {
				int i = index_in_graph(*it);
				(*it)->update();
				out << "Package " << (*it)->get_name() << " updated." << std::endl;
				if (!is_visited[i]) {
					is_visited[i] = true;
					q.push_back(i);
				}
			}
		}
		delete [] is_visited;
	
		return out;
	}
	
	package_operations Graph::remove_package(MainPackage *p) noexcept
	{
		if (!p) { return NULL_P; }
		if (!is_package_in_graph(p)) { return NOT_IN_PM; }
	
		int index = index_in_graph(p);
		for (size_t i = 0; i < m_graph.size(); i++) {
			if (i != index) {	
				/* delete package from list of dependencies */
				m_graph[i].remove(p);
			}
			if (i >= index) {
				/* shift remaining elements */
				m_graph[i] = m_graph[i + 1];
			}
		}

		return REMOVED;
	}
	
	bool is_auxiliary(const MainPackage *p) 
	{
		if (typeid(p) == typeid(AuxiliaryPackage*)) { return true; }
		return false;
	}
	
	bool is_any_package(const MainPackage *p)
	{
		return true;
	}
	
	void Graph::remove_nonused_packages(bool (*cond)(const MainPackage*)) noexcept
	{
		if (m_graph.empty()) { return; }
	
		bool *is_visited = new bool[m_graph.size()];
		for (size_t i = 0; i < m_graph.size(); i++) {
			is_visited[i] = false;
		}
	
		std::list<int> q;
		int p_index = index_in_graph(m_graph[0].front());
		is_visited[p_index] = true;
		q.push_back(p_index);
		std::list<MainPackage*>::const_iterator it;
		while (!q.empty()) {
			p_index = q.front();
			q.pop_front();
			for(it = m_graph[p_index].begin(); it != m_graph[p_index].end(); it++) {
				int i = index_in_graph(*it);
				if (cond(*it)) {
					if ((*it)->is_installed()) {
						remove_package(*it);
					}
				}
				if (!is_visited[i]) {
					is_visited[i] = true;
					q.push_back(i);
				}
			}
		}
		delete [] is_visited;
	}

	package_operations ProPro::install_package_request(std::ostream &out, std::istream &in, const EmptyPackage &ep)
	{
		MainPackage *package = ep.get_package();
		
		if (!package) { return NULL_P; }
		if (!m_graph.is_package_in_graph(package)) { return NOT_FOUND; }

		if (package->is_installed()) {
			package->update();
			return EXISTS_IN_PM;
		}

		package->install();
		std::list<MainPackage*> dep = package->get_dependencies();
		std::list<MainPackage*>::iterator it;
		char choice;
		/* install all dependencies needed for the package with request from the user */
		for(it = dep.begin(); it != dep.end(); it++) {
			out << "Install package " << (*it)->get_name() << "? (y/n) ";
			in >> choice;
			if (choice == 'y' || choice == 'Y') {
				(*it)->install();
			}
		}

		return INSTALLED;
	}

	package_operations ProPro::remove_package(const EmptyPackage &ep)
	{
		MainPackage *package = ep.get_package();
		package_operations res = m_graph.remove_package(package);
		m_graph.remove_nonused_packages(&is_any_package);
		return res;
	}

	ProPro& ProPro::remove_nonused_auxiliary() noexcept
	{
		m_graph.remove_nonused_packages(&is_auxiliary);
		return *this;
	}

	std::ostream& ProPro::update(std::ostream &out) noexcept
	{
		return m_graph.update_packages(out);
	}
}

