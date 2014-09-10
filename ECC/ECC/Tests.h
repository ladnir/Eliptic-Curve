#pragma once

template<class T>
class Tests
{
public:
	
	static bool runAll();

	static bool testMultiply();
	static bool testMultiply1();

	static bool testWikiDivision();
	static bool testWikiInverse();
	static bool testWikiBruteForceInverse();
	static bool testBruteForceGCD();

	static bool testGCD();
	static bool testMultDivide();

    static bool testGCDvsInverse();
    static bool testGCDvsBFInverse();
};

