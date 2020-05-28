#include <iostream>
#include <memory>

using namespace std;

class base {
public:
    virtual void func() = 0;
    virtual void func2() = 0;
    virtual bool compare(int&) = 0;

protected:
    int elem = 6;
};

class base_core : virtual public base {
public:
    void func() override {
        cout << "BaseCoreFunc" << endl;
    }

    void func2() override {
        cout << "BaseCoreFunc2" << endl;
    }

    bool compare(int&) override {
        cout << "BaseCoreCompare" << endl;
        return true;
    }
};

template <typename T = base_core, typename = typename std::enable_if<std::is_base_of<base_core, T>::value>::type>
class base_decoration_1 : virtual public T {
public:
    void func() override {
        cout << "BaseDecoration1Func" << endl;
    }

    void func2() override {
        cout << "BaseDecoration1Func2" << endl;
    }

    bool compare(int& obj) override {
        cout << "BaseDecoration1Compare" << endl;
        return T::compare(obj) && T::elem == obj;
    }
};

template <typename T = base_core, typename = typename std::enable_if<std::is_base_of<base_core, T>::value>::type>
class base_decoration_2 : virtual public T {
public:
    void func() override {
        cout << "BaseDecoration2Func" << endl;
    }

    void func2() override {
        cout << "BaseDecoration2Func2" << endl;
    }

    bool compare(int& obj) override {
        cout << "BaseDecoration2Compare" << endl;
        return T::compare(obj) && T::elem == obj;
    }
};

template <typename Decoration = base_core, typename = typename std::enable_if<std::is_base_of<base_core, Decoration>::value>::type>
class advanced_core : virtual public base_decoration_2<base_core>, virtual public Decoration {
public:
    void func() override {
        cout << "AdvancedCoreFunc" << endl;
    }

    void func2() override {
        cout << "AdvancedCoreFunc2" << endl;
    }

    bool compare(int& obj) override {
        cout << "AdvancedCoreCompare" << endl;
        return base_decoration_2<base_core>::compare(obj) && Decoration::compare(obj) && Decoration::elem == obj;
    }
};

int main() {
    {
        std::shared_ptr<base> base = std::make_shared<advanced_core<base_decoration_1<base_decoration_1<base_decoration_2<>>>>>();
//        base* base = new advanced_core<base_core>();
        int a = 6;
        base->compare(a);
    }
    return 0;
}