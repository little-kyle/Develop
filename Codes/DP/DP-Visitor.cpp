
class ConcreteComponentA;
class ConcreteComponentB;

//访问者基类；因为我们有两个组件A和B，所以访问者提供了对两个组件的访问；
class Visitor{
public:
    virtual void VisitConcreteComponentA(const ConcreteComponentA *element)const=0;
    virtual void VisitConcreteComponentB(const ConcreteComponentB *element)const=0;
};

//组件基类；
class Component{
public:
    virtual ~Component(){}
    virtual void Accept(Visitor *visitor) const=0; //const表示这个成员函数不会修改类的成员变量；
};
//具体组件类；
class ConcreteComponentA:public Component{
public:
    //accept函数用于接收访问者，传入的访问者对象可以访问其访问该组件的函数；将组件本身传给访问者的访问函数；
    void Accept(Visitor *visitor)const override{
        visitor->VisitConcreteComponentA(this); //接收的这个访问者可以访问该成分；
    }
    std::string ExclusiveMethodofConcreteComponentA() const {
        return "A";
    }
};

class ConcreteComponentB : public Component {
  /**
   * Same here: visitConcreteComponentB => ConcreteComponentB
   */
public:
    void Accept(Visitor *visitor) const override {
        visitor->VisitConcreteComponentB(this);
    }
    std::string SpecialMethodOfConcreteComponentB() const {
        return "B";
    }
};

//具体的访问者；每一个访问者都会有访问每个组件的函数；其需要组件对象的地址；
class ConcreteVisitor1:public Visitor{
public:
    void VisitConcreteComponentA(const ConcreteComponentA *element)const override{
        std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor1\n";
    }
    void VisitConcreteComponentB(const ConcreteComponentB *element) const override {
        std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor1\n";
    }
};
class ConcreteVisitor2 : public Visitor {
public:
    void VisitConcreteComponentA(const ConcreteComponentA *element) const override {
        std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
    }
    void VisitConcreteComponentB(const ConcreteComponentB *element) const override {
        std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
    }
};

void ClientCode(std::array<const Component *, 2> components, Visitor *visitor) {
  // ...
    for (const Component *comp : components) {
        comp->Accept(visitor);
    } 
  // ...
}

int main() {
    std::array<const Component *, 2> components = {new ConcreteComponentA, new ConcreteComponentB}; //创建一个std::array容器，存储了所有可用的组件对象；
    std::cout << "The client code works with all visitors via the base Visitor interface:\n";
    ConcreteVisitor1 *visitor1 = new ConcreteVisitor1; //创建访问者对象；
    ClientCode(components, visitor1);
    std::cout << "\n";
    std::cout << "It allows the same client code to work with different types of visitors:\n";
    ConcreteVisitor2 *visitor2 = new ConcreteVisitor2;
    ClientCode(components, visitor2);

    for (const Component *comp : components) {
        delete comp;
    }
    delete visitor1;
    delete visitor2;

    return 0;
}

/*
The client code works with all visitors via the base Visitor interface:
A + ConcreteVisitor1
B + ConcreteVisitor1

It allows the same client code to work with different types of visitors:
A + ConcreteVisitor2
B + ConcreteVisitor2
*/

