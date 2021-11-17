#pragma once

#include <iostream> 
#include <cstring> 
#include <limits> 
#include <typeinfo> 

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
		std::ostream& operator <<(std::ostream&);
		std::istream& operator >>(std::istream&);
		Version& operator =(const Version&);
		Version& operator ++() noexcept;
		Version operator ++(int) noexcept;
	};

	struct Strings {
		char **data = nullptr;
		unsigned count;
		
		Strings() : count(0) {}
		Strings(const Strings&);
		Strings(Strings &&s)
			: data(s.data)
			, count(s.count) { s.data = nullptr; s.count = 0; }
		~Strings() { delete_data(); }

		Strings& set(const char **, unsigned);
		// private:
		Strings& delete_data() noexcept;
		std::ostream& operator <<(std::ostream&);
		std::istream& operator >>(std::istream&);
		Strings& operator =(const Strings&);
		Strings& operator =(Strings &&arr);
	};

	template <typename T1, typename T2>
	struct pair {
		T1 first;
		T2 second;
		
		pair() : first(T1()), second(T2()) {}
		pair(const T1 &_first, const T2 &_second)
			: first(_first)
			, second(_second) {}

		pair& set(T1&, T2&);
		pair& operator =(const pair&);
	};


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
		~List() { delete m_head; }
		
		List& operator =(const List &l) noexcept
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

	class MainPackage {
	private:
		bool m_installed;
	protected:
		char *m_name;
		Version m_version;
		char *m_publisher;
		Strings m_source;
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

		virtual bool is_installed() const noexcept { return m_installed; }
		const char *get_name() const noexcept { return m_name; }
		const Version get_version() const noexcept { return m_version; }
		const char *get_publisher() const noexcept { return m_publisher; }
		const Strings& get_source() const noexcept { return m_source; }
		virtual void install() noexcept;
		MainPackage& add_to_repository(ProPro&);
		bool remove_from_repository(ProPro&) noexcept;
		MainPackage& update() noexcept;
	};

	class AuxiliaryPackage: public MainPackage {
	private:
		bool m_installed;
		Strings m_libs;
	public:
		AuxiliaryPackage() : m_installed(false) {}
		AuxiliaryPackage(const Strings &libs)
			: m_installed(false)
			, m_libs(libs) {}
		AuxiliaryPackage(const AuxiliaryPackage&);
		AuxiliaryPackage(AuxiliaryPackage&&) noexcept;
		virtual ~AuxiliaryPackage() = default;

		AuxiliaryPackage& operator =(const AuxiliaryPackage&);
		AuxiliaryPackage& operator =(AuxiliaryPackage&&) noexcept;

		virtual bool is_installed() const noexcept override { return m_installed; }
		virtual const Strings &get_libs() { return m_libs; }
		void install() noexcept override;
		// maybe return List
		AuxiliaryPackage* separate(unsigned, pair<char*, Strings>*) noexcept; // ???
	};

	class PackageLink: public Link {
	protected:
		MainPackage *m_package;
	public:
		PackageLink(MainPackage *p = nullptr) noexcept : m_package(p) {}
		MainPackage *get_package() const noexcept { return m_package; }
		~PackageLink() { delete m_package; }
	};

	//class PackagePart: public AuxiliaryPackage {
	//private:
	//	bool m_installed;
	//	char *m_name;
	//	Strings m_libs;
	//public:
	//	PackagePart() : m_installed(false), m_name(nullptr) {}
	//	PackagePart(char *name, const Strings &libs)
	//		: m_installed(false)
	//		, m_name(name)
	//		, m_libs(libs) {}
	//	~PackagePart() { delete [] m_name; }

	//	PackagePart& operator =(const PackagePart &pp)
	//		{ AuxiliaryPackage::operator =(pp); return *this;}
	//	PackagePart& operator =(PackagePart &&pp) noexcept
	//		{ AuxiliaryPackage::operator =(std::move(pp)); return *this;}

	//	bool is_installed() const noexcept override { return m_installed; }
	//	const Strings &get_libs() override { return m_libs; }
	//	PackagePart& set(const pair<char*, Strings>&);
	//};

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

		const Graph& get_graph() const noexcept { return m_graph; }
		bool remove_package(MainPackage*) noexcept;
		ProPro& add_package(MainPackage*);
		ProPro& install_package_auto(const EmptyPackage&);
		ProPro& install_package_request(std::ostream&, std::istream&, const EmptyPackage&);
		ProPro& remove_package(const EmptyPackage&);
		ProPro& remove_nonused_auxiliary() noexcept;
		ProPro& update(std::ostream&) noexcept;
	};

	// MainPackage
	List unite_packages(std::ostream&, const List&); // ???????
}

namespace pm = PackageManager;
