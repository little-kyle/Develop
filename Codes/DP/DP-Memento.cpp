#include<iostream>
#include<ctime>
#include<vector>

//备忘录对象；是原发器状态快照的值对象。通常做法是将备忘录设为不可变得，并且通过构造函数一次性传递数据；
class Memento{
public:
    virtual ~Memento(){} 
    virtual std::string GetName() const=0; //获取名称；
    virtual std::string date() const=0; //获取日期；
    virtual std::string state() const=0; //获取该备忘录的状态；
};

//具体的备忘录；
class ConcreteMemento:public Memento{
private:
    std::string state_;
    std::string date_;

public:
    //通过构造函数将当前状态传入，并且记录当前的时间值；
    ConcreteMemento(std::string state):state_(state){
        this->state_=state;
        std::time_t now=std::time(0); //获取当前时间戳；
        this->date_=std::ctime(&now); //ctime函数将时间戳转化为表示时间的字符串格式；
    }
    //返回备忘录中记录的状态；
    std::string state()const override{
        return this->state_;
    }
    //返回备忘录的名称；
    std::string GetName() const override{
        return this->date_+"/("+this->state_.substr(0,9)+"...)";
    }
    //返回备忘录中记录的时间；
    std::string date()const override{
        return this->date_;
    }
};

//原发器，可以生成自身状态的快照，也可以在需要时通过快照恢复自身状态；
class Originator{
private:
    std::string state_;  //记录一个状态；

    //随机生成字符串；
    std::string GenerateRandomString(int length=10){
        const char alphanum[]=
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        int stringLength=sizeof(alphanum)-1;

        std::string random_string; 
        for(int i=0;i<length;i++){
            random_string+=alphanum[std::rand()%stringLength];
        }
        return random_string;
    }
public:
    //构造函数会初始化其传入的状态参数；
    Originator(std::string state):state_(state){
        std::cout<<"Originator:My initial state is: "<<this->state_<<"\n";
    }
    void DoSomething(){
        std::cout<<"Originator: I'm doing something important.\n";
        this->state_=this->GenerateRandomString(30);
        std::cout<<"Originator: and my state has changed to:"<<this->state_<<"\n";
    }
    //保存备忘录；返回一个新的备忘录对象；
    Memento *Save(){ 
        return new ConcreteMemento(this->state_);
    }
    //根据一个备忘录来恢复其对应的状态；
    void Restore(Memento *memento){
        this->state_=memento->state();
        std::cout<<"Originator: My state has changed to:"<<this->state_<<"\n";
    }
};

class Caretaker{
private:
    std::vector<Memento*> mementos_; //这个数组记录了很多指向备忘录的指针；
    Originator *originator_;  //原发器；
public:
    Caretaker(Originator* originator):originator_(originator){}
    ~Caretaker(){
        for(auto m:mementos_) delete m;
    }
    void Backup(){
        std::cout<<"\nCaretaker: Saving Originator's state...\n";
        this->mementos_.push_back(this->originator_->Save());
    }
    void Undo(){
        if(!this->mementos_.size())return;
        Memento *memento=this->mementos_.back(); //获取列表中的最后一个备忘录，也就是最新的一个备忘录；
        this->mementos_.pop_back(); //删除最后一个备忘录对象；
        std::cout<<"Caretaker: Restoring state to: "<<memento->GetName()<<"\n";
        try{
            this->originator_->Restore(memento); //尝试将该拿出来的备忘录信息恢复到origintor的状态；
        }catch(...){
            this->Undo();
        }
    }
    void ShowHistory() const {
        std::cout<<"Caretaker:Here's the list of mementos:\n";
        for(Memento *memento:this->mementos_){
            std::cout<<memento->GetName()<<"\n";
        }
    }
};

void ClientCode(){
    Originator *originator =new Originator("Super-duper-super-puper-super."); //构建一个原发器；原发器会将传入的字符串参数作为状态，初始化这个新建的原发器的状态；
    Caretaker *caretaker=new Caretaker(originator); //创建一个caretaker对象，记录了原发器，和其历史的备忘录对象；后续通过该对象进行；
    caretaker->Backup(); //备份，也就是将关联的origintor对象的状态记录在该对象的备忘录数组中；
    originator->DoSomething(); //origintir做了一些事情；其状态发生了改变；
    caretaker->Backup(); //再次备份origintor的新状态；
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething(); //又做了一些事情，没有备份；
    std::cout << "\n";
    caretaker->ShowHistory();
    std::cout << "\nClient: Now, let's rollback!\n\n";
    caretaker->Undo(); //将上次的操作恢复；
    std::cout << "\nClient: Once more!\n\n";
    caretaker->Undo();

    delete originator;
    delete caretaker;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    ClientCode();
    return 0;
}

/*
Originator:My initial state is: Super-duper-super-puper-super.

Caretaker: Saving Originator's state...
Originator: I'm doing something important.
Originator: and my state has changed to:1YcqiICxMVthQfoeSlEYNGgruv4XRg

Caretaker: Saving Originator's state...
Originator: I'm doing something important.
Originator: and my state has changed to:zAIDpaY3NSpT1GVMWxAn3WQqJth985

Caretaker: Saving Originator's state...
Originator: I'm doing something important.
Originator: and my state has changed to:ZSabyE3RhS1nTdhmlsmIrsYgrjAzqf

Caretaker:Here's the list of mementos:
Tue Mar 19 19:57:11 2024
/(Super-dup...)
Tue Mar 19 19:57:11 2024
/(1YcqiICxM...)
Tue Mar 19 19:57:11 2024
/(zAIDpaY3N...)

Client: Now, let's rollback!

Caretaker: Restoring state to: Tue Mar 19 19:57:11 2024
/(zAIDpaY3N...)
Originator: My state has changed to:zAIDpaY3NSpT1GVMWxAn3WQqJth985

Client: Once more!

Caretaker: Restoring state to: Tue Mar 19 19:57:11 2024
/(1YcqiICxM...)
Originator: My state has changed to:1YcqiICxMVthQfoeSlEYNGgruv4XRg
*/
