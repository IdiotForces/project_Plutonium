
#include <iostream>

class base_novdtor {
	public:
	~base_novdtor() {
		std::cout << "base_novdtor::~base_novdtor()" << std::endl;
	}
};

class derived_novdtor : public base_novdtor {
	public:
	~derived_novdtor() {
		std::cout << "derived_novdtor::~derived_novdtor()" << std::endl;
	}
};

class base_vdtor {
	public:
	virtual ~base_vdtor() {
		std::cout << "base_vdtor::~base_vdtor()" << std::endl;
	}
};

class derived_vdtor : public base_vdtor {
	public:
	~derived_vdtor() {
		std::cout << "derived_vdtor::~derived_vdtor()" << std::endl;
	}
};

class test_case {
	public:

	virtual const char *name() const = 0;
	void test() const {
		std::cout << "Executing Test case: " << name() << std::endl;
		do_test();
		std::cout << "EOF." << std::endl;
		std::cout << std::endl;
	}
	virtual ~test_case() { }

	protected:

	virtual void do_test() const = 0;
};

class test_case_novbase_object : public test_case {
	public:
	const char *name() const override {
		return "novbase_object"; }
	protected:
	void do_test() const override {
		base_novdtor obj;
	}
};

class test_case_vbase_object : public test_case {
	public:
	const char *name() const override {
		return "novbase_object"; }
	protected:
	void do_test() const override {
		base_vdtor obj;
	}
};

class test_case_novderived_object : public test_case {
	public:
	const char *name() const override {
		return "novderived_object"; }
	protected:
	void do_test() const override {
		derived_novdtor obj;
	}
};

class test_case_vderived_object : public test_case {
	public:
	const char *name() const override {
		return "vderived_object"; }
	protected:
	void do_test() const override {
		derived_vdtor obj;
	}
};

class test_case_novbase_ptr_novbase : public test_case {
	public:
	const char *name() const override {
		return "novbase_ptr_novbase"; }
	protected:
	void do_test() const override {
		base_novdtor *ptr = new base_novdtor();
		delete ptr;
	}
};

class test_case_novbase_ptr_novderived : public test_case {
	public:
	const char *name() const override {
		return "novbase_ptr_novderived"; }
	protected:
	void do_test() const override {
		base_novdtor *ptr = new derived_novdtor();
		delete ptr;
	}
};

class test_case_vbase_ptr_vbase : public test_case {
	public:
	const char *name() const override {
		return "vbase_ptr_vbase"; }
	protected:
	void do_test() const override {
		base_vdtor *ptr = new base_vdtor();
		delete ptr;
	}
};

class test_case_vbase_ptr_vderived : public test_case {
	public:
	const char *name() const override {
		return "vbase_ptr_vderived"; }
	protected:
	void do_test() const override {
		base_vdtor *ptr = new derived_vdtor();
		delete ptr;
	}
};

class test_case_novderived_ptr_novderived : public test_case {
	public:
	const char *name() const override {
		return "novderived_ptr_novderived"; }
	protected:
	void do_test() const override {
		derived_novdtor *ptr = new derived_novdtor();
		delete ptr;
	}
};

class test_case_vderived_ptr_vderived : public test_case {
	public:
	const char *name() const override {
		return "vderived_ptr_vderived"; }
	protected:
	void do_test() const override {
		derived_vdtor *ptr = new derived_vdtor();
		delete ptr;
	}
};

int main () {
	std::cout << "Base and derived class object: " << std::endl;
	test_case_vbase_object().test();
	test_case_novbase_object().test();
	test_case_vderived_object().test();
	test_case_novderived_object().test();

	std::cout << "Base class ptr to base class and derived class object: " << std::endl;
	test_case_novbase_ptr_novbase().test();
	test_case_novbase_ptr_novderived().test();
	test_case_vbase_ptr_vbase().test();
	test_case_vbase_ptr_vderived().test();

	std::cout << "Derived class ptr to derived class object: " << std::endl;
	test_case_vderived_ptr_vderived().test();
	test_case_novderived_ptr_novderived().test();

	return 0;
}
