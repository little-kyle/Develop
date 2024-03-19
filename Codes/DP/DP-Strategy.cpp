#include<iostream>
#include<memory>
#include<algorithm>

//所有的可选策略的基类；
class Strategy{
public:
    virtual ~Strategy()=default;
    virtual std::string doAlgorithm(std::string_view data)const=0;  
};

//该context指向一个策略对象；
class Context{
private:
    //这里的unique_ptr是智能指针，自动释放不需要的内存，独占（确保只有一个这个指针对同一块内存具有独占权）
    std::unique_ptr<Strategy> strategy_;
public:
    explicit Context(std::unique_ptr<Strategy>&&strategy={}):strategy_(std::move(strategy)){}
    void set_strategy(std::unique_ptr<Strategy> &&strategy){
        strategy_=std::move(strategy);
    }
    void doSomeBusinessLogic() const{
        if(strategy_){
            std::cout << "Context: Sorting data using the strategy (not sure how it'll do it)\n";
            std::string result = strategy_->doAlgorithm("aecbd");
            std::cout << result << "\n";
        } else {
            std::cout << "Context: Strategy isn't set\n";
        }
    }
};

class ConcreteStrategyA:public Strategy{
public:
    std::string doAlgorithm(std::string_view data) const override{
        std::string result(data);
        std::sort(std::begin(result),std::end(result));
        
        return result;
    }
};

class ConcreteStrategyB:public Strategy{
    std::string doAlgorithm(std::string_view data)const override{
        std::string result(data);
        std::sort(std::begin(result),std::end(result),std::greater<>());
        return result;
    }
};

void ClientCode(){
    Context context(std::make_unique<ConcreteStrategyA>());
    std::cout<<"Client:Strategy is set to normal sorting.\n";
    context.doSomeBusinessLogic();
    std::cout<<"\n";
    std::cout<<"Client:Strategy is set to reverse sorting.\n";
    context.set_strategy(std::make_unique<ConcreteStrategyB>());
    context.doDomeBusinessLogic();
}

int main(){
    ClientCode();
    return 0;
}

/*
Client: Strategy is set to normal sorting.
Context: Sorting data using the strategy (not sure how it'll do it)
abcde

Client: Strategy is set to reverse sorting.
Context: Sorting data using the strategy (not sure how it'll do it)
edcba
*/
