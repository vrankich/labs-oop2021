#pragma once

#include <iostream> 
#include <cstring> 
#include <limits>
#include <string> 
#include <vector> 
#include <list> 
#include <typeinfo>
#include <algorithm> 
#include "../pm_exceptions.h"

namespace PackageManager 
{
	class ProPro;

	struct Version {
		unsigned major;
		unsigned minor;
		unsigned patch;
		
		Version(unsigned maj = 0, unsigned min = 0, unsigned p = 1)
			: major(maj)
			, minor(min)
			, patch(p) {}
		Version(const Version&);

		Version& set(unsigned, unsigned, unsigned);
		Version& operator =(const Version&);
		Version& operator ++() noexcept;
		Version operator ++(int) noexcept;
		friend std::ostream& operator <<(std::ostream&, const Version&);
		friend std::istream& operator >>(std::istream&, Version&);
	};

	class MainPackage {
	protected:
		bool m_installed;
		std::string m_name;
		Version m_version;
		std::string m_publisher;
		std::vector<std::string> m_source;
		std::list<MainPackage*> m_dependencies;
		void delete_dependencies();
	public:
		MainPackage() : m_installed(false) {}
		MainPackage(const MainPackage&);
		MainPackage(MainPackage&&) noexcept;
		virtual ~MainPackage() noexcept {
			delete_dependencies();
		}

		virtual MainPackage& operator =(const MainPackage&);
		virtual MainPackage& operator =(MainPackage&&) noexcept;
		bool operator ==(const MainPackage&) noexcept;
		bool operator !=(const MainPackage&) noexcept;

		bool is_installed() const noexcept { return m_installed; }
		std::string get_name() const noexcept { return m_name; }
		Version get_version() const noexcept { return m_version; }
		std::string get_publisher() const noexcept { return m_publisher; }
		std::vector<std::string> get_source() const noexcept { return m_source; }
		std::list<MainPackage*> get_dependencies() const noexcept { return m_dependencies; }
		MainPackage& install() noexcept;
		MainPackage& add_to_repository(ProPro&);
		bool remove_from_repository(ProPro&) noexcept;
		MainPackage& update() noexcept;
	
		virtual MainPackage *unite_packages(std::istream&, std::list<MainPackage*>&);
	};

	class AuxiliaryPackage: public MainPackage {
	private:
		std::vector<std::string> m_libs;
	public:
		//AuxiliaryPackage() {}
		AuxiliaryPackage(const std::vector<std::string> &libs) : m_libs(libs) {}
		AuxiliaryPackage(const AuxiliaryPackage &ap) 
			: MainPackage(ap) { m_libs = ap.m_libs; }
		AuxiliaryPackage(AuxiliaryPackage &&ap) noexcept
			: MainPackage(ap) { m_libs = std::move(ap.m_libs); }
		~AuxiliaryPackage() = default;

		AuxiliaryPackage& operator =(const AuxiliaryPackage&);
		AuxiliaryPackage& operator =(AuxiliaryPackage&&) noexcept;
		bool operator ==(const AuxiliaryPackage&) noexcept;
		bool operator !=(const AuxiliaryPackage&) noexcept;

		std::vector<std::string> get_libs() { return m_libs; }
		void set_libs(const std::vector<std::string> &libs) { m_libs = libs; }
		MainPackage *unite_packages(std::istream&, std::list<MainPackage*>&) override;
		std::list<MainPackage*> separate(unsigned, const std::vector<std::vector<std::string>>&);
	};

	class EmptyPackage {
	private:
		std::string m_name;
		MainPackage *m_package;
	public:
		//EmptyPackage() : m_package(nullptr) {}
		EmptyPackage(const std::string &name, MainPackage *mp)
			: m_name(name)
			, m_package(mp) {}
		EmptyPackage(const EmptyPackage&) noexcept;
		EmptyPackage(EmptyPackage&&) noexcept;
		~EmptyPackage() noexcept { delete m_package; }

		EmptyPackage& operator =(const EmptyPackage&);
		EmptyPackage& operator =(EmptyPackage&&) noexcept;

		std::string get_name() const noexcept { return m_name; }
		MainPackage *get_package() const noexcept { return m_package; }
	};

	enum package_operations {
		NULL_P,
		EXISTS_IN_PM,
		NOT_IN_PM,
		CONFLICT,
		ADDED,
		REMOVED,
		NOT_REMOVED,
		INSTALLED,
		NOT_FOUND,
	};

	class Graph {
	private:
		std::vector<std::list<MainPackage*>> m_graph;
		void delete_graph();
		bool is_reachable(const MainPackage*, const MainPackage*) const noexcept;
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
		std::vector<std::list<MainPackage*>> get_graph() const noexcept { return m_graph; }
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
			: m_graph(l) {}
		ProPro(const ProPro &pm) 
			: m_graph(pm.m_graph) {}
		ProPro(ProPro &&pm) noexcept
			: m_graph(pm.m_graph) {}
		~ProPro() = default;

		ProPro& operator =(const ProPro &pm) { m_graph = pm.m_graph; return *this; }
		ProPro& operator =(ProPro &&pm) { m_graph = pm.m_graph; return *this; }

		Graph get_graph() const noexcept { return m_graph; }
		package_operations add_package(MainPackage *p) { return m_graph.add_package(p); }
		package_operations remove_package(MainPackage*) noexcept;
		package_operations remove_package(const EmptyPackage&);
		package_operations install_package_auto(const EmptyPackage&);
		package_operations install_package_request(std::ostream&, std::istream&, const EmptyPackage&);
		ProPro& remove_nonused_auxiliary() noexcept;
		std::ostream& update(std::ostream&) noexcept;
	};
}

namespace pm = PackageManager;
