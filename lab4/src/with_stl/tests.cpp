#include "tests.h" 

void test_installation() 
{
	std::cout << std::endl << "--- installation ---" << std::endl;
	std::cout << std::endl;

	std::vector<std::string> source;
	source.push_back("some_link");
	source.push_back("another_link");

	MainPackage *p = new MainPackage;
	std::vector<MainPackage*> packages;
	p->set_name("cmatrix");
	p->set_publisher("someone");
	p->set_source(source);
	packages.push_back(p);

	//p = dynamic_cast<AuxiliaryPackage*>(p);
	p = new AuxiliaryPackage;
	p->set_name("cookie");
	p->set_publisher("me");
	source.push_back("my_link");
	p->set_source(source);
	packages.push_back(p);

	std::cout << *packages[0] << std::endl;
	std::cout << *packages[1] << std::endl;

	packages[0]->install();
	packages[1]->install();
	packages[0]->update_version();
	packages[1]->update_version();
	std::cout << *packages[0] << std::endl;
	std::cout << *packages[1] << std::endl;

	for (size_t i = 0; i < packages.size(); i++) {
		delete packages[i];
	}
}

void test_repo()
{
	std::list<MainPackage*> list;
	std::list<MainPackage*> dep;
	std::vector<std::string> source;
	source.push_back("some_link");
	source.push_back("another_link");

	MainPackage *p = new MainPackage;
	p->set_name("chromium");
	p->set_publisher("google");
	p->set_source(source);
	list.push_back(p);

	p = new MainPackage;
	p->set_name("bulka");
	p->set_publisher("france");
	p->set_source(source);
	list.push_back(p);
	dep.push_back(p);

	p = new AuxiliaryPackage;
	p->set_name("teatime");
	p->set_publisher("me");
	source.push_back("my_link");
	p->set_source(source);
	list.push_back(p);

	dep.push_back(p);
	p = new AuxiliaryPackage;
	p->set_name("time");
	p->set_publisher("you");
	source.push_back("your_link");
	p->set_source(source);
	p->set_dependencies(dep);
	list.push_back(p);

	p = dynamic_cast<AuxiliaryPackage*>(p);
	p = new AuxiliaryPackage;
	p->set_name("ofc");
	p->set_publisher("lalaland");
	source.push_back("linklinklink");
	p->set_source(source);
	p->set_dependencies(dep);
	list.push_back(p);

	EmptyPackage empty1(p);

	p = new MainPackage;
	p->set_name("sublime");
	p->set_publisher("ti");
	p->set_dependencies(dep);
	p->set_source(source);
	list.push_back(p);

	EmptyPackage empty2(p);

	ProPro pm(list);
//	pm.install_package_request(std::cout, std::cin, empty1);
//	std::cout << std::endl << *(empty1.get_package());
	pm.install_package_auto(empty2);
	std::cout << std::endl << *(empty2.get_package());
	std::cout << std::endl;
	
	p = new AuxiliaryPackage;
	p->set_name("vim");
	p->set_publisher("ti");
	p->set_dependencies(dep);
	p->set_source(source);
	p->add_to_repository(pm);

	pm.remove_nonused_auxiliary();
	pm.update(std::cout);

	std::cout << std::endl;
	std::vector<std::list<MainPackage*>> graph = pm.get_graph().get_graph();
}

void for_real_tests()
{
	ProPro pm;
	MainPackage *p = nullptr;
	Version v(0, 2, 1);
	std::vector<std::string> source;
	source.push_back("lalala");
	std::list<MainPackage*> dep;

	p = new MainPackage("orange", v, "market", source, dep);
	dep.push_back(p);
	p = new MainPackage("banana", v, "market", source, dep);
	p->add_to_repository(pm);
	//ASSERT_EQ(p->add_to_repository(pm), package_operations::ADDED);
	dep.push_back(p);
	
	std::vector<std::list<MainPackage*>> graph = pm.get_graph().get_graph();
	std::list<MainPackage*>::const_iterator it = graph[0].begin();
	//ASSERT_EQ((*it)->get_name(), "banana");
	it++;
	//ASSERT_EQ((*it)->get_name(), "orange");

	//delete dep.front();

	// what to do with cyclic adding????????
	p = new MainPackage("flower", v, "market", source, dep);
	dep.clear();
	dep.push_back(p);
	MainPackage *p1 = new MainPackage("potato", v, "market", source, dep);
	dep.clear();
	dep.push_back(p1);
	p->set_dependencies(dep);

	p->add_to_repository(pm);
	p1->add_to_repository(pm);

	p->add_to_repository(pm);
	p1->add_to_repository(pm);

	graph = pm.get_graph().get_graph();
	it = graph[1].begin();
	//ASSERT_TRUE(**it == *p);
	it++;
	//ASSERT_TRUE(**it == *p1);
	it = graph[2].begin();
	//ASSERT_TRUE(**it == *p1);
	it++;
	//ASSERT_TRUE(**it == *p);

	//ASSERT_TRUE(*(p1->get_dependencies().front()) == *p);
	//ASSERT_TRUE(*(p->get_dependencies().front()) == *p1);
}


void run_tests()
{
	//test_installation();
	//test_repo();
	for_real_tests();
}
