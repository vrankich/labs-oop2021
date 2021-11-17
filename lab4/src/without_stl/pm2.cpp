#include "cringe.h" 

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

	std::ostream& Version::operator <<(std::ostream &out)
	{
		out << major << "." << minor << "." << patch;
		return out;
	}

	std::istream& Version::operator >>(std::istream &in)
	{
		

		return in;
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
		if (patch < 99) { 
			patch++; 
			return *this;
		}
		patch = 0;
		if (minor < 99) {
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

	Strings::Strings(const Strings &s)
	{
		int len = 0;
		char **temp = new char*[s.count];
		for (unsigned i = 0; i < s.count; i++) {
			len = strlen(s.data[i]);
			temp[i] = new char[len + 1];
			memcpy(temp[i], s.data[i], len + 1);
		}
		data = temp;
	}

	Strings& Strings::set(const char **strings, unsigned n)
	{
		delete_data();
		if (!strings) { return *this; }

		int len = 0;
		char **temp = new char*[n];
		for (unsigned i = 0; i < n; i++) {
			len = strlen(strings[i]);
			temp[i] = new char[len + 1];
			memcpy(temp[i], strings[i], n + 1);
		}
		data = temp;

		return *this;
	}

	Strings& Strings::delete_data() noexcept
	{
		for (unsigned i = 0; i < count; i++) {
			delete [] data[i];
		}
		delete [] data;
		data = nullptr;
		count = 0;

		return *this;
	}

	std::ostream& Strings::operator <<(std::ostream &out)
	{
		if (!data) { 
			out << "no data"; 
			return out;
		}

		for (unsigned i = 0; i < count; i++) {
			if (data[i]) {
				out << "\"" << data[i] << "\"\n";
			}
		}
		return out;
	}

	std::istream& get_str(std::istream &input, char *&res)
	{
		const int buf_size = 81;
		char buf[buf_size], *temp;
		int len = 0, chunk_len, str_len;
		
		do {
			input.get(buf, buf_size, '\n');
			if (!input.good()) {
				return input;
			} else if (input.gcount() > 0) {
				chunk_len = strlen(buf);
				str_len = len + chunk_len;
				temp = res;
				res = new char[str_len + 1];
				memcpy(res, temp, len);
				memcpy(res + len, buf, chunk_len);
				len = str_len;
				delete [] temp;
			} else {
				input.clear();
				input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while(input.gcount() == buf_size - 1);
		
		if (len > 0) {
			res[len] = '\0';
		} else {
			res = new char[1];
		}
		
		return input;
	}

	std::istream& Strings::operator >>(std::istream &in)
	{
		delete_data();

		unsigned n = 0;
		in >> n;
		if (!in.good() || n == 0) { return in; }

		data = new char*[n];
		for (unsigned i = 0; i < n; i++) {
			get_str(in, data[i]);
			count++;
			if (!in.good()) {
				delete_data();
				return in;
			}
		}

		return in;
	}

	Strings& Strings::operator =(const Strings &s)
	{
		if (this == &s) { return *this; }
		
		delete_data();
		if (!s.data) { return *this; }

		int len = 0;
		char **temp = new char*[s.count];
		for (unsigned i = 0; i < s.count; i++) {
			len = strlen(s.data[i]);
			temp[i] = new char[len + 1];
			memcpy(temp[i], s.data[i], len + 1);
		}
		data = temp;

		return *this;
	}

	Strings& Strings::operator =(Strings &&s)
	{
		delete_data();
		data = s.data;
		count = s.count;
		s.data = nullptr;
		s.count = 0;
		return *this;
	}

	template <typename T1, typename T2>
	pair<T1, T2> &pair<T1, T2>::set(T1 &new_first, T2 &new_second)
	{
		first = new_first;
		second = new_second;
		return *this;
	}

	template <typename T1, typename T2>
	pair<T1, T2> &pair<T1, T2>::operator =(const pair<T1, T2> &pair)
	{
		first = pair.first;
		second = pair.second;
		return *this;
	}

	MainPackage::MainPackage(const MainPackage &mp)
	{
		m_installed = mp.m_installed;
		m_version = mp.m_version;
		m_source = mp.m_source;

		int len = 0;
		if (mp.m_name) {
			len = strlen(mp.m_name);
			m_name = new char[len + 1];
			memcpy(m_name, mp.m_name, len + 1);
		}
		if (mp.m_publisher) {
			len = strlen(mp.m_publisher);
			m_publisher = new char[len + 1];
			memcpy(m_publisher, mp.m_publisher, len + 1);
		}
	}

	MainPackage::MainPackage(MainPackage &&mp) noexcept
	{
		m_installed = mp.m_installed;
		m_name = mp.m_name;
		m_version = mp.m_version;
		m_publisher = mp.m_publisher;
		m_source = mp.m_source;
		mp.m_name = nullptr;
		mp.m_publisher = nullptr;
	}

	MainPackage& MainPackage::operator =(const MainPackage &mp)
	{
		if (this == &mp) { return *this;}

		m_installed = mp.m_installed;
		m_version = mp.m_version;
		m_source = mp.m_source;

		int len = 0;
		delete [] m_name;
		if (mp.m_name) {
			len = strlen(mp.m_name);
			m_name = new char[len + 1];
			memcpy(m_name, mp.m_name, len + 1);
		}
		delete [] m_publisher;
		if (mp.m_publisher) {
			len = strlen(mp.m_publisher);
			m_publisher = new char[len + 1];
			memcpy(m_publisher, mp.m_publisher, len + 1);
		}

		return *this;
	}

	MainPackage& MainPackage::operator =(MainPackage &&mp) noexcept
	{
		delete [] m_name;
		delete [] m_publisher;
		m_installed = mp.m_installed;
		m_name = mp.m_name;
		m_version = mp.m_version;
		m_publisher = mp.m_publisher;
		m_source = std::move(mp.m_source);
		mp.m_name = nullptr;
		mp.m_publisher = nullptr;
		return *this;
	}

	void MainPackage::install() noexcept
	{
		if (m_installed) {
			update();
			return;
		}
		m_installed = true;
	}

//	std::ostream& operator <<(std::ostream &out, const MainPackage &mp) noexcept
//	{
//
//		return out;
//	}
//
//	std::istream& operator >>(std::istream &in, MainPackage &mp) noexcept
//	{
//
//		return in;
//	}

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

	AuxiliaryPackage& AuxiliaryPackage::operator =(const AuxiliaryPackage &ap)
	{
		if (this == &ap) { return *this; }
		MainPackage::operator =(ap);
		m_installed = ap.m_installed;
		m_libs = ap.m_libs;
		return *this;
	}

	AuxiliaryPackage& AuxiliaryPackage::operator =(AuxiliaryPackage &&ap) noexcept
	{
		MainPackage::operator =(std::move(ap));
		m_installed = ap.m_installed;
		m_libs = std::move(ap.m_libs);
		return *this;
	}

	void AuxiliaryPackage::install() noexcept
	{
		if (m_installed) {
			update();
			return;
		}
		m_installed = true;
	}

	PackagePart* AuxiliaryPackage::separate(unsigned count, pair<char*, Strings> *data) noexcept // ???
	{
		if (count <= 0) { return nullptr; }

		PackagePart *parts = new PackagePart[count];
		for (unsigned i = 0; i < count; i++) {
			parts[i].set(data[i]);
		}
		return parts;
	}

	Link* ListIterator::operator ()() noexcept
	{
		Link *res = m_link;
		m_link = m_link ? m_link->next : m_list->m_head;
		return res;
	} 

	EmptyPackage::EmptyPackage(const EmptyPackage &ep)
	{
		m_package = ep.m_package;
		m_dependencies = ep.m_dependencies;
	}

	EmptyPackage::EmptyPackage(EmptyPackage &&ep) noexcept
	{
		m_package = ep.m_package;
		ep.m_package = nullptr;
		m_dependencies = std::move(ep.m_dependencies);
	}

	EmptyPackage& EmptyPackage::operator =(const EmptyPackage &ep)
	{
		if (this == &ep) { return *this; }

		delete m_package;
		m_package = ep.m_package;
		m_dependencies = ep.m_dependencies;
		return *this;
	}

	EmptyPackage& EmptyPackage::operator =(EmptyPackage &&ep) noexcept
	{
		//m_installed = ep.m_installed;
		delete m_package;
		m_package = ep.m_package;
		ep.m_package = nullptr;
		m_dependencies = std::move(ep.m_dependencies);
		return *this;
	}

	bool ProPro::remove_package(MainPackage *mp) noexcept
	{

		return false;
	}
	
	ProPro& ProPro::add_package(MainPackage *mp)
	{

		return *this;
	}

	// unite functions
	ProPro& ProPro::install_package_auto(const EmptyPackage &ep)
	{
		MainPackage *package = ep.get_package();
		
		if (package->is_installed()) {
			package->update();
			return *this;
		}

		// TODO: What to do with default version?
		package->install();
		List list(ep.get_dependencies().get_head());
		ListIterator iter(list);
		PackageLink *ptr = nullptr;
		Link *ptr_link = nullptr;
		while (ptr_link = iter()) {
			ptr = static_cast<PackageLink *>(ptr);
			ptr->get_package()->install();
		}

		return *this;
	}

	ProPro& ProPro::install_package_request(std::ostream &out, std::istream &in, const EmptyPackage &ep)
	{
		MainPackage *package = ep.get_package();
		
		if (package->is_installed()) {
			package->update();
			return *this;//
		}

		package->install();
		List list(ep.get_dependencies().get_head());
		ListIterator iter(list);
		PackageLink *ptr = nullptr;
		Link *ptr_link = nullptr;
		out << "Install dependencies" << std::endl;
		char choice;
		while (ptr_link = iter()) { // WAAARNING
			ptr = static_cast<PackageLink *>(ptr);
			out << "Install " << ptr->get_package()->get_name() << "? (y/n) ";
			in >> choice;
			if (!in.good()) { return *this; } // TODO: add error code
			if (choice == 'y' || choice == 'Y') {
				ptr->get_package()->install();
			}
		}

		return *this;
	}

	ProPro& ProPro::remove_package(const EmptyPackage &ep)
	{
		if (!remove_package(ep.get_package())) { return *this; }

		// remove nonused packages
		// maybe return an error code ?
	}

	ProPro& ProPro::remove_nonused_auxiliary() noexcept
	{

		return *this;
	}

	ProPro& ProPro::update(std::ostream&) noexcept
	{

		return *this;
	}
}

