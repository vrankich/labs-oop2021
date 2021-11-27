#include "pm.h" 

bool Graph::is_package_in_graph(const MainPackage *p) const noexcept
{
	if (!p) { return false; }
	MainPackage *temp_p = nullptr;
	for (size_t i = 0; i < m_graph.size(); i++) {
		temp_p = m_graph[i].front();
		if (*temp_p == *p) { return true; }
	}
	return false;
}
void Graph::delete_graph()
{
	for (size_t i = 0; i < m_graph.size(); i++) {	
		delete m_graph[i].front();
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

	int p1_index = index_in_graph(p1);
	if (p1_index == -1) { 
		delete [] is_visited;
		return false; 
	}
	int p2_index = index_in_graph(p2);
	if (p2_index == -1) { 
		delete [] is_visited;
		return false; 
	}
	
	is_visited[p1_index] = true;
	std::list<int> q;
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
	if (!p) { return package_operations::NULL_P; }
	if (is_package_in_graph(p)) { return package_operations::EXISTS_IN_PM; } // maybe throw an exception
	
	std::list<MainPackage*>::const_iterator it;
	std::list<MainPackage*> dependencies = p->get_dependencies();
	/* graph should be without cycles */
	for (it = dependencies.begin(); it != dependencies.end(); it++) {
		/* check whether adding a package would form a cycle */
		if(is_reachable(p, *it)) { return package_operations::CONFLICT; }
	}
	/* add new vertex with edges to graph */
	dependencies.push_front(p);
	m_graph.push_back(dependencies);
	
	return package_operations::ADDED;
}

std::ostream& Graph::update_packages(std::ostream &out) noexcept
{
	if (m_graph.empty()) { return out; }
	
	bool *is_visited = new bool[m_graph.size()];
	for (size_t i = 0; i < m_graph.size(); i++) {
		is_visited[i] = false;
	}
	
//	std::list<int> q;
//	int p_index = index_in_graph(m_graph[0].front());
//	is_visited[p_index] = true;
//	q.push_back(p_index);
//	std::list<MainPackage*>::const_iterator it;
//	while (!q.empty()) {
//		p_index = q.front();
//		q.pop_front();
//		for(it = m_graph[p_index].begin(); it != m_graph[p_index].end(); it++) {
//			int i = index_in_graph(*it);
//			(*it)->update_version();
//			out << "Package " << (*it)->get_name() << " updated." << std::endl;
//			if (!is_visited[i]) {
//				is_visited[i] = true;
//				q.push_back(i);
//			}
//		}
//	}

	for (size_t i = 0; i < m_graph.size(); i++) {
		m_graph[i].front()->update_version();
		out << "Package " << m_graph[i].front()->get_name() << " updated." << std::endl;
	}

	delete [] is_visited;

	return out;
}

package_operations Graph::remove_package(MainPackage *p) noexcept
{
	if (!p) { return package_operations::NULL_P; }
	if (!is_package_in_graph(p)) { return package_operations::NOT_IN_PM; }

	size_t index = index_in_graph(p);
	delete m_graph[index].front();
	for (size_t i = 0; i < m_graph.size(); i++) {
		if (i != index) {	
			/* delete package from list of dependencies */
			m_graph[i].remove(p);
		}
		if (i >= index && i < m_graph.size() - 1) {
			/* shift remaining elements */
			m_graph[i] = m_graph[i + 1];
		}
	}
	m_graph.resize(m_graph.size() - 1);

	return package_operations::REMOVED;
}

bool is_auxiliary(const MainPackage *p) 
{
	if (typeid(*p) == typeid(AuxiliaryPackage)) { return true; }
	return false;
}

bool is_any_package(const MainPackage *p)
{
	return true;
}

void Graph::remove_nonused_packages(bool (*cond)(const MainPackage*)) noexcept
{
	if (m_graph.empty()) { return; }

	MainPackage *p;
	for (size_t i = 0; i < m_graph.size(); i++) {
		p = m_graph[i].front();
		if (cond(p)) {
			if (!p->is_installed()) {
				remove_package(p);
			}
		}
	}

	//bool *is_visited = new bool[m_graph.size()];
	//for (size_t i = 0; i < m_graph.size(); i++) {
	//	is_visited[i] = false;
	//}

	//std::list<int> q;
	//int p_index = index_in_graph(m_graph[0].front());
	//is_visited[p_index] = true;
	//q.push_back(p_index);
	//std::list<MainPackage*>::const_iterator it;
	//while (!q.empty()) {
	//	p_index = q.front();
	//	q.pop_front();
	//	for(it = m_graph[p_index].begin(); it != m_graph[p_index].end(); it++) {
	//		int i = index_in_graph(*it);
	//		if (cond(*it)) {
	//			if ((*it)->is_installed()) {
	//				remove_package(*it);
	//			}
	//		}
	//		if (!is_visited[i]) {
	//			is_visited[i] = true;
	//			q.push_back(i);
	//		}
	//	}
	//}
	//delete [] is_visited;
}
package_operations ProPro::remove_package(MainPackage *mp) noexcept
{
	return m_graph.remove_package(mp);
}

package_operations ProPro::install_package_auto(const EmptyPackage &ep)
{
	MainPackage *package = ep.get_package();
	
	if (!package) { return package_operations::NULL_P; }
	if (!m_graph.is_package_in_graph(package)) { 
		return package_operations::NOT_FOUND; 
	}
	if (package->is_installed()) {
		package->update_version();
		return package_operations::EXISTS_IN_PM;
	}
	package->install();
	std::list<MainPackage*> dep = package->get_dependencies();
	std::list<MainPackage*>::iterator it;
	/* install all dependencies needed for the package */
	for(it = dep.begin(); it != dep.end(); it++) {
		(*it)->install();
	}
	return package_operations::INSTALLED;
}
package_operations ProPro::install_package_request(std::ostream &out, std::istream &in, const EmptyPackage &ep)
{
	MainPackage *package = ep.get_package();
	
	if (!package) { return package_operations::NULL_P; }
	if (!m_graph.is_package_in_graph(package)) { 
		return package_operations::NOT_FOUND; 
	}
	if (package->is_installed()) {
		package->update_version();
		return package_operations::EXISTS_IN_PM;
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
	return package_operations::INSTALLED;
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

