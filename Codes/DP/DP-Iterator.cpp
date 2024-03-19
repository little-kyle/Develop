#include<iostream>
#include<string>
#include<vector>

template<typename T,typename U>
class Iterator{
public:
    //typename表示后面的是一个类型名而不是变量名或静态成员名；
    typedef typename std::vector<T>::iterator iter_type;//定义一个模板类型，其实际类型是std::vector<T>的迭代器
    //构造函数；对一个容器创建一个迭代器；
    Iterator(U *p_data,bool reverse=false):m_p_data_(p_data){
        m_it_=m_p_data_->m_data_.begin();
    }
    //获取迭代器的第一个；
    void First(){
        m_it_=m_p_data_->m_data_.begin();
    }
    //获取迭代器的下一个迭代；
    void Next(){
        m_it_++;
    }
    //判断迭代器是否结束；
    bool IsDone(){
        return (m_it_ == m_p_data_->m_data_.end());
    }
    iter_type Current(){
        return m_it_;
    }

private:
    U *m_p_data_; //这里是一个U类型的指针，指向我们的容器；
    iter_type m_it_; //迭代的指向；
};

template<class T>
class Container{
    friend class Iterator<T,Container>;
public:
    //向数据数组中添加数据；
    void Add(T a){
        m_data_.push_back(a);
    }
    //创建一个迭代器；
    Iterator<T,Container>*CreateIterator(){
        return new Iterator<T,Container>(this);
    }
private:
    std::vector<T> m_data_;
};

class Data{
public:
    Data(int a=0):m_data_(a){}
    void set_data(int a){
        m_data_=a;
    }
    int data(){
        return m_data_;
    }
private:
    int m_data_;
};

void ClientCode(){
    std::cout<<"________________Iterator with int______________________________________" << std::endl;
    Container<int> cont; //创建一个容器，容器中存储的数据类型是int类型；
    for(int i=0;i<10;i++){
        cont.Add(i);  //向容器中添加10个数；
    }

    Iterator<int,Container<int>> *it = cont.CreateIterator(); //创建一个迭代器；
    for(it->First();!it->IsDone();it->Next()){
        std::cout<<*it->Current()<<std::endl;
    }

    Container<Data> cont2;
    Data a(100),b(1000),c(10000);
    cont2.Add(a);
    cont2.Add(b);
    cont2.Add(c);
    std::cout << "________________Iterator with custom Class______________________________" << std::endl;
    Iterator<Data,Container<Data>>*it2 = cont2.CreateIterator();
    for(it2->First();!it2->IsDone();it2->Next()){
        std::cout<<it2->Current()->data()<<std::endl;
    }
    delete it;
    delete it2;
}

int main(){
    ClientCode();
    return 0;
}



