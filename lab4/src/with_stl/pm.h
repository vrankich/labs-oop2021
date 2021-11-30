#pragma once

#include <iostream> 
#include <limits>
#include <string> 
#include <vector> 
#include <list> 
#include <algorithm> 
#include <typeinfo> 
#include "packages.h" 
#include "empty_package.h" 
#include "../pm_exceptions.h"

class MainPackage;
class AuxiliaryPackage;
class EmptyPackage;
enum class package_operations;

class Graph {
private:
	std::vector<std::list<MainPackage*>> m_graph;
	void delete_graph();
	//bool is_reachable(const MainPackage*, const MainPackage*) const noexcept;
	bool is_reachable(size_t, const MainPackage*) const noexcept;
	int index_in_graph(const MainPackage*) const noexcept;
public:
	Graph() {}
	Graph(const Graph &g) : m_graph(g.m_graph) {}
	Graph(std::list<MainPackage*>&);
	Graph(Graph &&g) noexcept 
		: m_graph(std::move(g.m_graph)) { g.delete_graph(); }
	~Graph() noexcept { delete_graph(); }
	
	Graph& operator =(const Graph &g) noexcept { m_graph = g.m_graph; return *this; }
	Graph& operator =(Graph &&g) noexcept 
		{ m_graph = std::move(g.m_graph); g.delete_graph(); return *this; }
	//friend std::ostream& operator <<(std::ostream&, const Graph&) noexcept;
	
	const std::vector<std::list<MainPackage*>> &get_graph() const noexcept { return m_graph; }
	bool is_package_in_graph(const MainPackage*) const noexcept;
	Graph& add_edge(MainPackage*);
	package_operations add_package(MainPackage*);
	package_operations remove_package(MainPackage*) noexcept;
	std::ostream& update_packages(std::ostream&) noexcept;
	void remove_nonused_packages(bool (*cond)(const MainPackage*)) noexcept;
};

class ProPro {
private:
	Graph m_graph;
public:
	ProPro() = default;
	ProPro(std::list<MainPackage*> &l)
		: m_graph() { m_graph = l; }
	ProPro(const ProPro &pm) 
		: m_graph(pm.m_graph) {}
	ProPro(ProPro &&pm) noexcept
		: m_graph(pm.m_graph) {}
	~ProPro() = default;
	
	ProPro& operator =(const ProPro &pm) { m_graph = pm.m_graph; return *this; }
	ProPro& operator =(ProPro &&pm) { m_graph = pm.m_graph; return *this; }
	
	const Graph &get_graph() const noexcept { return m_graph; }
	package_operations add_package(MainPackage *p) { return m_graph.add_package(p); }
	package_operations remove_package(MainPackage*) noexcept;
	package_operations remove_package(const EmptyPackage&);
	package_operations install_package_auto(const EmptyPackage&);
	package_operations install_package_request(std::ostream&, std::istream&, const EmptyPackage&);
	ProPro& remove_nonused_auxiliary() noexcept;
	std::ostream& update(std::ostream&) noexcept;
};
