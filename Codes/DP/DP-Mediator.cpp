#include<iostream>
#include<string>
#include<stdlib.h>

class BaseComponent;
//中介者类；接口；
class Mediator{
public:
    virtual void Notify(BaseComponent *sender,std::string event) const=0;
};
//组件的接口；组件会维护一个指向中介者的指针；
class BaseComponent{
protected:
    Mediator *mediator_;
public:
    //构造函数；
    BaseComponent(Mediator *mediator=nullptr):mediator_(mediator){} //构造函数初始化其中介者；
    void set_mediator(Mediator *mediator){
        this->mediator_=mediator;
    }
};
class Component1:public BaseComponent{
public:
    void DoA(){
        std::cout<<"Component 1 does A.\n";
        this->mediator_->Notify(this,"A"); //通过调用中介者来完成实际的操作；
    }
    void DoB(){
        std::cout<<"Component 1 does B.\n";
        this->mediator_->Notify(this,"B");
    }
};
class Component2:public BaseComponent{
public:
    void DoC(){
        std::cout<<"Component 2 does C.\n";
        this->mediator_->Notify(this,"C"); 
    }
    void DoD(){
        std::cout<<"Component 2 does D.\n";
        this->mediator_->Notify(this,"D");
    }
};

//具体的中介者；中介者中都会包含其连接的组件；
class ConcreteMediator:public Mediator{
private:
    Component1 *component1_; 
    Component2 *component2_;
public:
    //将该中介者所联系的组件都绑定到该中介上；
    ConcreteMediator(Component1 *c1,Component2*c2):component1_(c1),component2_(c2){
        this->component1_->set_mediator(this);
        this->component2_->set_mediator(this);
    }
    //由中介者来实际作为中间；
    void Notify(BaseComponent *sender,std::string event)const override{
        if(event=="A"){
            std::cout<<"Mediator reacts on A and triggers following operations:\n";
            this->component2_->DoC();
        }
        if(event=="D"){
            std::cout<<"Mediator reacts on D and triggers following operations:\n";
            this->component1_->DoB();
            this->component2_->DoC();
        }
    }
};

void ClientCode(){
    Component1 *c1=new Component1;
    Component2 *c2=new Component2;
    ConcreteMediator *mediator =new ConcreteMediator(c1,c2); //创建c1和c2的中介者；
    std::cout<<"Client triggers operation A.\n";
    c1->DoA(); //这里c1组件调用DoA函数，DoA函数不会直接去操作目标对象，而是会通过中介来进行操作；
    std::cout<<"\n";
    std::cout<<"Client triggers operation D.\n";
    c2->DoD();

    delete c1;
    delete c2;
    delete mediator;
}

int main(){
    ClientCode();
    //system("pause");
    return 0;
}

/*
Client triggers operation A.
Component 1 does A.
Mediator reacts on A and triggers following operations:
Component 2 does C.

Client triggers operation D.
Component 2 does D.
Mediator reacts on D and triggers following operations:
Component 1 does B.
Component 2 does C.
*/



