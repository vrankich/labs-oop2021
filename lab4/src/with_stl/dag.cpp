#include "dag.h" 

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
	
		size_t index = index_in_graph(p);
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


