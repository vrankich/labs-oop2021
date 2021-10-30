#pragma once

#include <iostream> 

namespace PackageManager 
{
	class ProPro;

	struct Version {
		unsigned major;
		unsigned minor;
		unsigned patch;
		bool is_set;
		
		Version() : is_set(false) {}
		Version(unsigned maj, unsigned min, unsigned p)
			: major(maj)
			, minor(min)
			, patch(p)
			, is_set(true) {}
		Version(const Version&);

		Version& set(unsigned, unsigned, unsigned);
		std::ostream& operator <<(std::ostream&);
		std::istream& operator >>(std::istream&);
		Version& operator =(const Version&);
	};


	struct Strings {
		char **data = nullptr;
		unsigned count;
		
		Strings() : count(0) {}
		Strings(const Strings&);
		Strings(Strings &arr)
			: data(arr.data)
			, count(arr.count) { arr.data = nullptr; }
		~Strings() {
			for (unsigned i = 0; i < count; i++) {
				delete [] data[i];
			}
			delete [] data;
		}

		Strings& set(const char **, unsigned);
		std::ostream& operator <<(std::ostream&);
		std::istream& operator >>(std::istream&);
		Strings& operator =(const Strings&);
		Strings& operator =(Strings &&arr);
	};

	class MainPackage {
	private:
		bool m_installed;
	protected:
		char *m_name;
		Version m_version;
		char *m_publisher;
		Strings m_source;
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

		MainPackage& operator =(const MainPackage&);
		MainPackage& operator =(MainPackage&&) noexcept;
		MainPackage& operator ==(const MainPackage&) const noexcept;
		MainPackage& operator !=(const MainPackage&) const noexcept;
		friend std::ostream& operator <<(std::ostream&, const MainPackage&) noexcept;
		friend std::istream& operator >>(std::istream&, MainPackage&) noexcept;

		bool is_installed() const noexcept { return m_installed; }
		const char *get_name() const noexcept { return m_name; }
		const Version get_version() const noexcept { return m_version; }
		const char *get_publisher() const noexcept { return m_publisher; }
		const Strings& get_source() const noexcept { return m_source; }
		MainPackage& add_to_repository(ProPro&) const;
		bool remove_from_repository(ProPro&) const noexcept;
		MainPackage& update() noexcept;
	};

	class AuxiliaryPackage: public MainPackage {
	private:
		bool m_installed;
		Strings m_libs;
	public:
		AuxiliaryPackage()
			: m_installed(false) {}
		AuxiliaryPackage(const AuxiliaryPackage&);
		AuxiliaryPackage(AuxiliaryPackage&&) noexcept;
		~AuxiliaryPackage() = default;

		AuxiliaryPackage& operator =(const AuxiliaryPackage&);
		AuxiliaryPackage& operator =(AuxiliaryPackage&&);

		AuxiliaryPackage& separate(unsigned) noexcept; // ???
	};

	struct Item {
		MainPackage *package;
		Item *next;
		
		Item()
			: package(new MainPackage)
			, next(nullptr) {}
		Item(const Item &i)
			: package(i.package)
			, next(i.next) {} // check this
		Item(Item &&i) noexcept
			: package(i.package) // and this
			, next(i.next) { i.package = nullptr; i.next = nullptr; }
		~Item() noexcept { delete package; }

		Item& operator =(const Item&);
		Item& operator =(Item&&) noexcept;
	};

	class PackageList {
	private:
		Item *head;
	public:
		PackageList() : head(nullptr) {}
		PackageList(const PackageList &list) { head = list.head; }
		PackageList(PackageList &&list) noexcept { head = list.head; list.head = nullptr;}
		~PackageList() noexcept { delete head; }

		const Item *get_list() const noexcept { return head; }
		PackageList& add_item(const Item&);
		PackageList& operator =(const PackageList&) noexcept;
		PackageList& operator =(PackageList&&) noexcept;
	};

	class EmptyPackage {
	private:
		bool m_installed; // maybe not
		MainPackage *m_package;
		PackageList m_dependencies;
	public:
		EmptyPackage() 
			: m_installed(false)
			, m_package(nullptr) {}
		EmptyPackage(const EmptyPackage&);
		EmptyPackage(EmptyPackage&&) noexcept;
		~EmptyPackage() noexcept { 
			//delete [] m_name; 
			//delete m_dependencies;
			delete m_package;
		}

		EmptyPackage& operator =(const EmptyPackage&);
		EmptyPackage& operator =(EmptyPackage&&);

		EmptyPackage& unite_packages(std::ostream&, const PackageList&); // ???
	};

	struct Vertex;

	struct Edge {
		MainPackage *package;
		Vertex *v;
		Edge *next;
		Edge()
			: package(new MainPackage)
			, v(nullptr)
			, next(nullptr) {}
		Edge(const Edge&);
		Edge(Edge&&);
		~Edge() { delete package; } // + delete next ?

		Edge& operator =(const Edge&);
		Edge& operator =(Edge&&);
	};

	struct Vertex {
		Edge *first;
		Vertex *next;
		Vertex()
			: first(new Edge)
			, next(nullptr) {}
		Vertex(const Vertex&);
		Vertex(Vertex&&) noexcept;
		~Vertex() noexcept { delete first; } // delete next ?

		Vertex& operator =(const Vertex&) noexcept;
		Vertex& operator =(Vertex&&) noexcept;
	};

	class Graph {
	private:
		Vertex *first;
	public:
		Graph() : first(new Vertex) {}
		Graph(const Graph &g) : first(g.first) {}
		Graph(Graph &&g) noexcept : first(g.first) { g.first = nullptr; }
		~Graph() noexcept { delete first; }

		const Vertex *get_graph() const noexcept { return first; }
		Graph& operator =(const Graph &g) noexcept { first = g.first; return *this; }
		Graph& operator =(Graph &&g) noexcept;
		friend std::ostream& operator <<(std::ostream&, const Graph&) noexcept;
		friend std::istream& operator >>(std::istream&, Graph&) noexcept;
	};
 
	class ProPro {
	private:
		Graph m_graph;
	public:
		ProPro() = default;
		ProPro(const PackageList&);
		ProPro(const ProPro &pm) 
			: m_graph(pm.m_graph) {} // ?
		ProPro(ProPro &&pm) 
			: m_graph(pm.m_graph) {} // ???????
		~ProPro() = default;

		ProPro& operator =(const ProPro &pm) { m_graph = pm.m_graph; return *this; }
		ProPro& operator =(ProPro &&pm) { m_graph = pm.m_graph; return *this; }

		const Graph& get_graph() const noexcept { return m_graph; }
		bool remove_package(MainPackage&) noexcept;
		ProPro& add_package(MainPackage&);
		ProPro& install_package_auto(const EmptyPackage&);
		ProPro& install_package_request(const EmptyPackage&);
		ProPro& remove_package(const EmptyPackage&);
		ProPro& remove_nonused_auxiliary() noexcept;
		ProPro& update(std::ostream&) noexcept;
	};
}

namespace pm = PackageManager;
