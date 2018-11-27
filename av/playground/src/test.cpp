#include "test.h"
#include<iostream>
#include <fstream>
#include <thread>
#include <map>
#include <vector>
#include <ostream>
#include <sstream>

void function_1() {
    std::cout << "Hello C++11 thread" << std::endl;
    
}

Test::Test() {
	std::cout << "test" << std::endl;	
}

Test::~Test() {
 	std::cout << "~ test" << std::endl;
}

/// c++ 写文件操作
void Test::testFileStream() {
	std::ofstream test_file("./ttt.txt");
    test_file << "1234567" << std::endl;
    test_file << "48320483209" << std::endl;
    test_file << 2 << 5 << 3.08 << 3.9;
    const char buf[6] = {'1','b','d','e','f','g'};
    for(int i = 0; i < 10; i++) {
        test_file.write(buf,6);
    }
    test_file.close();
    
    std::ifstream w_test_file("./ttt.txt");
    if(!w_test_file.is_open()) {
        std::cout << "error opening file" << std::endl;
        return;
    }
//    std::streambuf buf;
//    w_test_file >> buf;
//    std::cout << buf << std::endl;
    //w_test_file.read(<#char_type *__s#>, <#streamsize __n#>)
}


/// shared_ptr 引用计数操作
void Test::testSmartPoint() {

    //std::string file_full_name(argv[1]);
    //avformat_network_init();

    //file_test();
    //print_media_info(fileName);
    //extra_audio_data(file_full_name);
    //avformat_network_deinit();
    std::cout << "***************" <<  __func__ << "***************" << std::endl;
    std::shared_ptr<Test> p1 = std::make_shared<Test>();
    std::cout <<  "1 ref:" << p1.use_count() << std::endl;
    {
        std::shared_ptr<Test> p2 = p1;
        std::cout << "2 ref:" << p1.use_count() <<std::endl;
    }
    std::cout << "3 ref:" << p1.use_count() << std::endl;
    std::cout << "***************" <<  "end"  << "***************" << std::endl;
}


void Test::testLambda() {
    /// https://www.jianshu.com/p/d686ad9de817
    std::cout << "***************" <<  __func__ << "***************" << std::endl;
    auto simlambda = []()->void{ std::cout << "Hello, Lambda" << std::endl; };
    auto add = [](int a, int b)->int { return a+b; };
    simlambda();
    std::cout << add(3,5) << std::endl;
    /// 可以省略 返回类型 参数列表
    
    auto simlambda1 = []{ std::cout << "Hello, Lambda" << std::endl; };
    auto add1 = [](int a, int b){ return a+b; };
    simlambda1();
    std::cout << add1(6,7) << std::endl;
    
    /// [] 表示捕捉块， 为空是表示没有捕获任何变量
    int x = 10;
    x++;
    auto add_x = [x](int a){
        return a + x;
    }; /// 复制捕捉x  x 在闭包中默认不能被修改。如果想要修改需要加 mutable 修饰闭包
    
    auto add_x1 = [x](int a) mutable {
        x = 3;
        return a + x;
    };
    std::cout << "add_x :" << add_x(3) << " add_x1: " << add_x1(3) << std::endl;
    
    auto muti_x = [&x](int a) {
        x++;
        return a * x;
    };/// 引用捕捉x  x 可以修改；
    
    std::cout << "muti_x:" << muti_x(4) << " x:" << x << std::endl;
    std::cout << "***************" <<  "end"  << "***************" << std::endl;
}


void Test::testTypeId() {
    /// typeid操作符的返回结果是名为type_info的标准库类型的对象的引用 可以使用.name 方法获取
     std::cout << "***************" <<  __func__ << "***************" << std::endl;
    int a = 3;
    auto simlambda = []()->void{ std::cout << "Hello, Lambda" << std::endl; };
    std::cout << typeid(simlambda).name() << std::endl;
    std::cout << "***************" <<  "end"  << "***************" << std::endl;
}

void Test::testThread() {
    std::cout << "***************" <<  __func__ << "***************" << std::endl;
    std::thread th(&Test::doSomeThreadWork,this);
    th.join();
    std::cout << "***************" <<  __func__ <<  "end"  << "***************" << std::endl;
}

void Test::doSomeThreadWork() {
    std::cout << "***************" <<  __func__ << "***************" << std::endl;
    std::cout << "***************" <<  "end"  << "***************" << std::endl;
}

void Test::testThread2() {
    /*
     this_thread::get_id: 返回当前线程的id
     this_thread::yield: 让调度器先运行其它的线程，这在忙于等待状态时很有用
     this_thread::sleep_for: 将当前线程置于阻塞状态，时间不少于参数所指定的时间段
     this_thread::sleep_util: 在指定的时刻来临前，一直将当前的线程置于阻塞状态
     */
    std::cout << "***************" <<  __func__ << "***************" << std::endl;
    std::thread tx(&Test::doSomeThreadWork2,this,3);
    tx.detach();
    std::cout << "***************" <<  __func__ <<  "end"  << "***************" << std::endl;
}

void Test::doSomeThreadWork2(int a) {
    std::cout << "***************" <<  __func__ << "***************" << std::endl;
    std::cout << "arg:" << a << std::endl;
    /// 获取最佳的线程数量
    std::cout << std::thread::hardware_concurrency()  << std::endl;
    std::cout << "***************" <<  __func__ <<  "end"  << "***************" << std::endl;
}

void Test::testThread3() {
    std::thread th1(function_1);
    th1.detach();
    if (th1.joinable()) {
        th1.join();
    }
}

// 普通函数
int add(int i, int j) {
    return i + j;
    
}

void Test::testStdFunction() {
    /// https://www.jianshu.com/p/2dad1cad2661
    /// https://blog.csdn.net/wangshubo1989/article/details/49134235
    /*
            通常std::function是一个函数对象类，它包装其它任意的函数对象，
            被包装的函数对象具有类型为T1, …,TN的N个参数，并且返回一个可转换到R类型的值。
            std::function使用 模板转换构造函数接收被包装的函数对象；
            特别是，闭包类型可以隐式地转换为std::function。
        */
    
    /*
            std::function<Layer*()> 表示一个可调用调用对象 接受0个参数，返回Layer*
        */
    

    // lambda表达式
    auto mod = [](int i, int j){return i % j; };
    // 函数对象类
    struct divide
    {
        int operator() (int denominator, int divisor)
        {
            return denominator / divisor;
        }
    };
    
    /// 受限的map
    std::map<char, int(*)(int,int)> ops_limit;
    ops_limit.insert({'+', add});
    ops_limit.insert(std::make_pair('%', mod));
    //ops_limit.insert({'/',divide()})
    
    std::map<std::string, std::function<int(int,int)>> ops;
    ops.insert({"+",add});
    ops.insert({"%",mod});
    ops.insert({"÷",divide()});
    ops.insert({"-",std::minus<int>()});
    ops.insert({"*",[](int i, int j){ return i * j; }});
    
    std::map<char, std::function<int(int,int)>> ops1 = {
        {'+',add},
        {'%',mod},
        {'/',divide()},
        {'-',std::minus<int>()},
        {'*',[](int i, int j){return i * j; }}
    };
    
    std::cout << ops1['+'](3,5) << std::endl;
    std::cout << ops1['%'](5,3) << std::endl;
    std::cout << ops1['/'](3,5) << std::endl;
    std::cout << ops1['-'](3,5) << std::endl;
    std::cout << ops1['*'](3,5) << std::endl;
}

void Test::testStdMove() {
    ///// https://www.jianshu.com/p/ff4dc98f4a8c
}

template <typename T>
std::ostream& operator <<(std::ostream& out, std::vector<T> vec) {
    std::stringstream stream;
    stream << "[" << *vec.begin();
    std::for_each(vec.begin(), vec.end(), [&stream](const T& value){
        stream << "," << value;
    });
    stream << "]";
    out << stream.str();
    return out;
}


void Test::testStdforeach() {
    /// std::
    int a[] = {1,2,3,5,6,7,8,9,10};
    std::vector<int> tt(a,a+sizeof(a)/sizeof(int));
    std::for_each(tt.begin(), tt.end(), [&tt](int& value){
        value += 10;
    });
    std::cout << "tt:" << tt << std::endl;
    std::vector<int> aa = {10,9,8,9,6,5,4,3,2,1};
    std::cout << "aa:" << aa << std::endl;
}
