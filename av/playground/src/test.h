#ifndef TEST_H
#define TEST_H

class Test {
public:
	Test();
	~Test();
	void testFileStream();
    void testSmartPoint();
    void testLambda();
    void testTypeId();
    void testThread();
    void doSomeThreadWork();
    void testThread2();
    void doSomeThreadWork2(int a);
    void testThread3();
    void testStdFunction();
    void testStdMove();
};

#endif
