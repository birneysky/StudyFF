#include "test.h"
#include<iostream>
#include <fstream>

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
