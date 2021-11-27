#pragma once

#include <iostream> 
#include <cstring> 
#include <limits> 
#include <typeinfo> 
#include "../pm_exceptions.h"

namespace PackageManager 
{
	class ProPro;

	struct Version {
		unsigned major;
		unsigned minor;
		unsigned patch;
		
		Version(unsigned maj = 0, unsigned min = 0, unsigned p = 0)
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

//	struct Strings {
//		char **data = nullptr;
//		unsigned count;
//		
//		Strings() : count(0) {}
//		Strings(const Strings&);
//		Strings(Strings &&s)
//			: data(s.data)
//			, count(s.count) { s.data = nullptr; s.count = 0; }
//		~Strings() { delete_data(); }
//
//		Strings& set(const char **, unsigned);
//		std::ostream& operator <<(std::ostream&);
//		std::istream& operator >>(std::istream&);
//		Strings& operator =(const Strings&);
//		Strings& operator =(Strings &&arr);
//	private:
//		Strings& delete_data() noexcept;
//	};

	template <typename T>
	class Vector {
	private:
		T *m_arr;
		size_t m_size;
	public:
		Vector(T *arr = nullptr, size_t size = 0) noexcept
			: m_arr(arr)
			, m_size(size) {}
		Vector(const Vector&);
		Vector(Vector &&v) noexcept 
			: m_arr(v.m_arr)
			, m_size(v.m_size) { v.m_arr = nullptr; } 
		~Vector() noexcept { delete [] m_arr; }

		size_t size() const noexcept { return m_size; }

		T& operator =(const Vector&);
		T& operator =(Vector &&) noexcept;
		T& operator [](size_t i) {
			if (i > m_size) {
				throw invalid_vector_size();
			}
			return m_arr[i];
		}
	};

	template <typename T> Vector<T>::Vector(const Vector<T> &v)
	{
		m_size = v.m_size;
		m_arr = new T[m_size];
		for (size_t i = 0; i < m_size; i++) {
			m_arr[i] = v.m_arr[i];
		}
	}

	template <typename T> T& Vector<T>::operator =(const Vector<T> &v)
	{
		delete [] m_arr;
		m_size = v.m_size;
		m_arr = new T[m_size];
		for (size_t i = 0; i < m_size; i++) {
			m_arr[i] = v.m_arr[i];
		}
		//return *this;
	}

	template <typename T> T& Vector<T>::operator =(Vector<T> &&v) noexcept
	{
		delete [] m_arr;
		m_size = v.m_size;
		m_arr = v.m_arr;
		v.m_arr = nullptr;
		return *this;
	}


	struct Link {
		Link *next;
		Link() noexcept : next(nullptr) {}
		Link(Link *link) noexcept : next(link) {}
		virtual ~Link() { delete next; }
		Link& operator =(const Link &l) noexcept
			{ next = l.next; return *this; }
		Link& operator =(Link &&l) noexcept
			{ next = l.next; l.next = nullptr; return *this; }
	};

	class List {
	private:
		Link *m_head;
	public:
		List() noexcept : m_head(nullptr) {}
		List(Link *link) noexcept : m_head(link) {}
		List(const List &l) : m_head(l.m_head) {}
		List(List &&l) : m_head(l.m_head) { l.m_head = nullptr; }
		~List() { delete m_head; }
		
		List& operator =(const List &l)
			{ m_head = l.m_head; return *this; }
		List& operator =(List &&l) noexcept
			{ m_head = l.m_head; l.m_head = nullptr; return *this; }
		Link *get_head() const noexcept { return m_head; }
		void insert(Link *l) noexcept { l->next = m_head; m_head = l; }
		friend class ListIterator;
	};

	class ListIterator {
	private:
		Link *m_link;
		List *m_list;
	public:
		ListIterator(List &l) noexcept
			: m_list(&l)
			, m_link(l.m_head) {}
		~ListIterator() { delete m_link; delete m_list; }
		Link* operator ()() noexcept;
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

	class MainPackage {
	protected:
		bool m_installed;
		char *m_name;
		Version m_version;
		char *m_publisher;
		Vector<char*> m_source;
		List m_dependencies;
	public:
		MainPackage()
			: m_installed(false)
			, m_name(nullptr)
			, m_publisher(nullptr) {}
		MainPackage(const MainPackage&);
		MainPackage(MainPackage&&) noexcept;
		virtual ~MainPackage() noexcept {
			delete [] m_name;
			delete [] m_publisher;
		}

		virtual MainPackage& operator =(const MainPackage&);
		virtual MainPackage& operator =(MainPackage&&) noexcept;
		bool operator ==(const MainPackage&) noexcept;
		bool operator !=(const MainPackage&) noexcept;

		bool is_installed() const noexcept { return m_installed; }
		const char *get_name() const noexcept { return m_name; }
		Version get_version() const noexcept { return m_version; }
		const char *get_publisher() const noexcept { return m_publisher; }
		Vector<char*> get_source() const noexcept { return m_source; }
		List get_dependencies() const noexcept { return m_dependencies; }
		MainPackage& install() noexcept;
		MainPackage& add_to_repository(ProPro&);
		package_operations remove_from_repository(ProPro&) noexcept;
		MainPackage& update() noexcept;
		virtual MainPackage *unite_packages(std::istream&, List&);
	};

	class AuxiliaryPackage: public MainPackage {
	private:
		Vector<char*> m_libs;
	public:
		AuxiliaryPackage(const Vector<char*> &libs) : m_libs(libs) {}
		AuxiliaryPackage(const AuxiliaryPackage&);
		AuxiliaryPackage(AuxiliaryPackage&&) noexcept;
		virtual ~AuxiliaryPackage() = default;

		AuxiliaryPackage& operator =(const AuxiliaryPackage&);
		AuxiliaryPackage& operator =(AuxiliaryPackage&&) noexcept;
		bool operator ==(const AuxiliaryPackage&) noexcept;
		bool operator !=(const AuxiliaryPackage&) noexcept;

		Vector<char*> get_libs() { return m_libs; }
		void set_libs(const Vector<char*> &libs) { m_libs = libs; }
		MainPackage *unite_packages(std::istream&, List&) override;
		List separate(unsigned, const Vector<Vector<char*>>&);
	};

	class PackageLink: public Link {
	protected:
		MainPackage *m_package;
	public:
		PackageLink(MainPackage *p = nullptr) noexcept : m_package(p) {}
		MainPackage *get_package() const noexcept { return m_package; }
		~PackageLink() { delete m_package; }
	};

	class EmptyPackage {
	private:
		char *m_name;
		MainPackage *m_package;
	public:
		EmptyPackage() : m_package(nullptr) {}
		EmptyPackage(MainPackage *mp, const List &d)
			: m_package(mp) {}
		EmptyPackage(const EmptyPackage&);
		EmptyPackage(EmptyPackage&&) noexcept;
		~EmptyPackage() noexcept { delete m_package; }

		EmptyPackage& operator =(const EmptyPackage&);
		EmptyPackage& operator =(EmptyPackage&&) noexcept;

		const char *get_name() const noexcept { return m_name; }
		MainPackage *get_package() const noexcept { return m_package; }
	};

	struct Vertex;

	struct Edge {
		Vertex *m_v;
		Edge *next;
		Edge()
			: m_v(nullptr)
			, next(nullptr) {}
		Edge(const Edge&);
		Edge(Edge&&);
		virtual ~Edge() { delete next; } 

		Edge& operator =(const Edge&);
		Edge& operator =(Edge&&);
	};

	class EdgePackage: public Edge {
	protected:
		MainPackage *m_package;
	public:
 		EdgePackage(MainPackage *p = nullptr) : m_package(p) {}
		~EdgePackage() { delete m_package; }
		
		EdgePackage& operator =(const EdgePackage &ep)
			{ m_package = ep.m_package; return *this; }
		EdgePackage& operator =(EdgePackage &&ep) noexcept
			{ m_package = ep.m_package; ep.m_package = nullptr; return *this; }	
		MainPackage *get_package() const noexcept { return m_package; }
	};

	struct Vertex {
		Edge *first;
		Vertex *next;
		Vertex()
			: first(new Edge)
			, next(nullptr) {}
		Vertex(const Vertex&);
		Vertex(Vertex&&) noexcept;
		~Vertex() noexcept { delete first; delete next; }

		Vertex& operator =(const Vertex&) noexcept;
		Vertex& operator =(Vertex&&) noexcept;
	};

	class Graph {
	private:
		Vertex *m_first;
	public:
		Graph() : m_first(new Vertex) {}
		Graph(const Graph &g) : m_first(g.m_first) {}
		Graph(Graph &&g) noexcept : m_first(g.m_first) { g.m_first = nullptr; }
		~Graph() noexcept { delete m_first; }

		Graph& operator =(const Graph &g) noexcept { m_first = g.m_first; return *this; }
		Graph& operator =(Graph &&g) noexcept;
		friend std::ostream& operator <<(std::ostream&, const Graph&) noexcept;
		friend std::istream& operator >>(std::istream&, Graph&) noexcept;

		Vertex *get_graph() const noexcept { return m_first; }
		Graph& add_edge();
	};
 
	class ProPro {
	private:
		Graph m_graph;
	public:
		ProPro() = default;
		ProPro(const List&);
		ProPro(const ProPro &pm) 
			: m_graph(pm.m_graph) {}
		ProPro(ProPro &&pm) 
			: m_graph(pm.m_graph) {}
		~ProPro() = default;

		ProPro& operator =(const ProPro &pm) { m_graph = pm.m_graph; return *this; }
		ProPro& operator =(ProPro &&pm) { m_graph = pm.m_graph; return *this; }

		Graph get_graph() const noexcept { return m_graph; }
		package_operations add_package(MainPackage *p) { return m_graph.add_edge(p); }
		package_operations remove_package(MainPackage*) noexcept;
		package_operations remove_package(const EmptyPackage&);
		package_operations install_package_auto(const EmptyPackage&);
		package_operations install_package_request(std::ostream&, std::istream&, const EmptyPackage&);
		ProPro& remove_nonused_auxiliary() noexcept;
		std::ostream& update(std::ostream&) noexcept;
	};
}

namespace pm = PackageManager;
