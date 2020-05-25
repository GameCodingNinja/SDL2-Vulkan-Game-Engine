// consoleTestAp.cpp : Defines the entry point for the console application.
//
	
#include <iostream>
#include <chrono>

namespace NTest
{
#define MY_ENUM \
    X(V1) \
    X(V2) \
    X(V3)

#define X(name) name,
enum MyEnum : int
{
    MY_ENUM
};
#undef X
 
constexpr const char* MyEnumToString(MyEnum e) noexcept
{
    #define X(name) case(name): return #name;
    switch(e)
    {
        MY_ENUM
    }
    #undef X
    //return "UNKNOWN";
}
}
 
int main(int argc, char** argv)
{
    std::cout << NTest::MyEnumToString(NTest::V1) << std::endl;
    std::cout << NTest::MyEnumToString(NTest::V2) << std::endl;
    std::cout << NTest::MyEnumToString(NTest::V3) << std::endl;
    return 1;
}


/*#include <utilities/highresolutiontimer.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

int main()
{
    auto start = std::chrono::high_resolution_clock::now() + std::chrono::seconds(10);

    for(int i = 0; i < 500000; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        auto end = std::chrono::high_resolution_clock::now();
        auto result = start-end;
        auto milliDur = std::chrono::duration_cast<std::chrono::milliseconds>(result).count();
        auto secDur = std::chrono::duration_cast<std::chrono::seconds>(result).count();
        auto minDur = std::chrono::duration_cast<std::chrono::minutes>(result).count();

        std::cout << std::setfill('0') << std::setw(2) << minDur << ":"<< std::setfill('0') << std::setw(2) << (secDur % 60) << ":"<< std::setfill('0') << std::setw(2) << ((milliDur/10) % 100) << std::endl;
    
        if( milliDur/10 <= 0)
            break;
    }

    return 0;
}*/



// C++ 11, 14, 17 features

/*#include <iostream>
#include <vector>
#include <complex>

#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <iterator>

#include <tuple>


class [ [deprecated] ] CMyClass
{
public:

    CMyClass() :
        m_myVec {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        m_myArray {0}
    {
    }

private:

    std::vector<int> m_myVec;
    int m_myArray[50];
};
 
struct S {
    int n;
    std::string s;
    float d;
    bool operator<(const S& rhs) const
    {
        // compares n to rhs.n,
        // then s to rhs.s,
        // then d to rhs.d
        return std::tie(n, s, d) < std::tie(rhs.n, rhs.s, rhs.d);
    }
};

auto MyFunc()
{
    int valueInt = 42;
    std::string valueStr = "Test";
    float valueFloat = 3.14;
    
    return std::make_tuple(valueInt, valueStr, valueFloat, std::ignore);
}

template <auto ... vs> struct HeterogenousValueList {};
using MyList = HeterogenousValueList<'a', 100, 'b'>;
template<auto n> struct B {  };

int main()
{

    std::vector<int> myVec {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 
    auto multiply = [](auto x, auto y) {return x + y;};

    std::cout << multiply(5, 9) << std::endl;
    std::cout << multiply(5.1, 9.7) << std::endl;

    auto vecSize = [vec = std::move(myVec)]() {return vec.size();};

    std::cout << vecSize() << std::endl;
    std::cout << myVec.size() << std::endl;

    auto million = 1'000'000;
    auto pi = 3.14159'26535'89793;

    std::cout << million << std::endl;
    std::cout << pi << std::endl;

    using namespace std::complex_literals;
    auto myComplexFloat = 29if;

    std::cout << std::norm(myComplexFloat) << std::endl;

    std::set<S> mySet;
 
    // pre C++17:
    {
	    S value{42, "Test", 3.14};
	    std::set<S>::iterator iter;
	    bool inserted;
 
	    // unpacks the return val of insert into iter and inserted
	    std::tie(iter, inserted) = mySet.insert(value);

	    if (inserted)
		    std::cout << "Value was inserted\n";
    }
	
	// with C++17:
    {
        S value{100, "abc", 100.0};
        const auto [iter, inserted] = mySet.insert(value);
		
        if (inserted)
		    std::cout << "Value(" << iter->n << ", " << iter->s << ", ...) was inserted" << "\n";
    }

    auto tuple = MyFunc();
    std::get<0>(tuple);

    if(auto [a, b, c, d] = MyFunc(); a > c)
    {
        std::cout << "A is greater" << std::endl;
    }

    std::cout << multiply(5, 9) << std::endl;

    if(int init = 1; init == 2)
        std::cout << init << std::endl;

    std::make_tuple("string", 7);

    for (auto[iss, name] = std::pair(std::istringstream(head), std::string {}); getline(iss, name); )
    {
    // Process name
    }
}*/

/*#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <chrono>
#include <atomic>
#include <cstdlib>
#include <cstdint>
#include <utilities/pool.h>
#include <utilities/ascii_escape_code.hpp>

using namespace std;
using namespace std::chrono;
using namespace ascii_escape_code;

std::string with_commas(uint64_t value)
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

template<typename PT>
void benchmark(const char* pool_name, uint64_t tasks, uint64_t reps)
{
	cout << black << bold << pool_name << normal << " (" << red << with_commas(tasks) << black << " tasks, " << red << with_commas(reps) << black << " reps)" << flush;

	atomic_uint64_t result = 0;
	auto work = [&result](uint64_t r)
	{
		uint64_t sum = 0;
		for (auto i = 1; i <= r; ++i)
		{
			auto t = rand();
			sum += t + 1;
			sum -= t;
		}
		result += sum;
	};

	auto start_time = high_resolution_clock::now();
	{
		PT pool;
		for (uint64_t i = 1; i <= tasks / 2; ++i)
		{
			pool.enqueue_work(work, reps);
			[[maybe_unused]] auto p = pool.enqueue_task(work, reps);
		}
	}
	auto end_time = high_resolution_clock::now();

	cout << "\t" << red << duration_cast<microseconds>(end_time - start_time).count() / 1000.f << " ms" << black;
	cout << "\t(", (result == tasks * reps ? cout << green : cout << red), cout << with_commas(result) << reset << ")" << endl;
}

int main()
{
    cout << "test started..." << endl;
	uint64_t TASK_START = 100'000;
	uint64_t TASK_STEP  = 100'000;
	uint64_t TASK_STOP  = 1'000'000;

	uint64_t REPS_START = 100;
	uint64_t REPS_STEP  = 100;
	uint64_t REPS_STOP  = 1'000;

	for(auto t = TASK_START; t <= TASK_STOP; t += TASK_STEP)
	{
		if(TASK_START < TASK_STOP) cout << "********************************************************************************" << endl;
		for(auto r = REPS_START; r <= REPS_STOP; r += REPS_STEP)
		{
			benchmark<simple_thread_pool>("simple  ", t, r);
			benchmark<thread_pool>       ("advanced", t, r);
			if(REPS_START < REPS_STOP) cout << endl;
		}
		if(TASK_START < TASK_STOP) cout << endl;
	}
}*/

/*#include <iostream>
#include <chrono>
#include <cstdlib>
#include <utilities/pool.h>
using namespace std;
using namespace chrono;
 
const unsigned int COUNT = 10'000'000;
const unsigned int REPS = 10;
 
int main()
{
    cout << "test started..." << endl;
	srand(0);
	auto start = high_resolution_clock::now();
	{
		simple_thread_pool tp;
		for(int i = 0; i < COUNT; ++i)
			tp.enqueue_work([i]() {
				int x;
				int reps = REPS + (REPS * (rand() % 5));
				for(int n = 0; n < reps; ++n)
					x = i + rand();
			});
	}
	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start);
	cout << "simple_thread_pool duration = " << duration.count() / 1000.f << " s" << endl;
 
	srand(0);
	start = high_resolution_clock::now();
	{
		thread_pool tp;
		for(int i = 0; i < COUNT; ++i)
			tp.enqueue_work([i]() {
				int x;
				int reps = REPS + (REPS * (rand() % 5));
				for(int n = 0; n < reps; ++n)
					x = i + rand();
			});
	}
	end = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(end - start);
	cout << "thread_pool duration = " << duration.count() / 1000.f << " s" << endl;

    return 0;
}*/

// XML file load test
/*#include <iostream>
#include <utilities/highresolutiontimer.h>
#include <utilities/tagparser.h>
#include <utilities/xmlParser.h>

int main()
{
    //std::cout << "Test started..." << std::endl;

    CTagParser tagParser;

    CHighResTimer::Instance().timerStart();
    
    tagParser.loadXML("usabilitywiki-20191220-stub-meta-history.xml");
    
    //std::cout << "Tag Parser Execution time: " << CHighResTimer::Instance().timerStop() << std::endl;

    CHighResTimer::Instance().timerStart();

    XMLNode::openFileHelper( "usabilitywiki-20191220-stub-meta-history.xml", "mediawiki" );

    //std::cout << "XML Node Execution time: " << CHighResTimer::Instance().timerStop() << std::endl;
    
    return 0;
}*/

// rvalue / lvalue test
/*#include <vector>
#include <iostream>
#include <string>

template <typename type>
void printMe(type && str)
{
    std::cout << str << std::endl;
}

class CMyClass
{
public:

    CMyClass()
    {
        m_data = {0,1,2,3,4,5,6,7,8,9};
        std::cout << "default constructor" << std::endl;
    }

    CMyClass(const CMyClass & myClass) :
        m_data(myClass.m_data)
    {
        std::cout << "lvalue copy constructor" << std::endl;
    }

    CMyClass(CMyClass && myClass) :
        m_data(std::move(myClass.m_data))
    {
        std::cout << "rvalue copy constructor" << std::endl;
    }

    CMyClass(std::vector<int> && data) :
        m_data(std::move(data))
    {
        std::cout << "rvalue reference" << std::endl;
    }

    CMyClass(std::vector<int> & data) :
        m_data(data)
    {
        std::cout << "lvalue reference" << std::endl;
    }

    std::vector<int> get()
    {
        //std::vector<int> data = {0,1,2,3,4,5,6,7,8,9};

        return m_data;
    }

    void operator = ( CMyClass & myClass )
    {
        m_data = myClass.m_data;
        std::cout << "lvalue =" << std::endl;
    }

    void operator = ( const CMyClass & myClass )
    {
        m_data = myClass.m_data;
        std::cout << "const lvalue =" << std::endl;
    }

    void operator = ( CMyClass && myClass )
    {
        m_data = std::move(myClass.m_data);
        std::cout << "rvalue =" << std::endl;
    }

    std::vector<int> m_data;

};

CMyClass getMyClass()
{
    CMyClass myClass;
    myClass.m_data.push_back(12);
    myClass.m_data.push_back(15);
    myClass.m_data.push_back(22);

    return myClass;
}


int main()
{
    CMyClass result( getMyClass() );

    CMyClass copyClass = std::move(result);

    result = std::move(copyClass);
    

    std::string myStr = "test String";
    printMe(myStr);

    std::vector<int> data1 = {0,1,2,3,4,5,6,7,8,9};

    CMyClass myClass(std::move(data1));

    std::vector<int> data2 = myClass.get();

    return 0;
}*/


/*#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <utilities/highresolutiontimer.h>

int main()  // 7124.35 ms
{
    std::cout << "Test started..." << std::endl;

    std::vector<int> dataVec;
    dataVec.reserve(100000);
    for(int i = 0; i < 100000; i++)
        dataVec.push_back(i);

    std::random_shuffle( dataVec.begin(), dataVec.end() );
    
    CHighResTimer::Instance().timerStart();
    
    //std::set<int> testSet;
    std::unordered_set<int> testSet;
    for(int iter : dataVec)
        testSet.insert(iter);

    for(int iter : dataVec)
        testSet.find(iter);
    
    std::cout << "Execution time: " << CHighResTimer::Instance().timerStop() << std::endl;
    
    return 0;
}*/

/*#include <utilities/matrix.h>

int main()
{
    CMatrix m_matrix;
    CMatrix m_matrixRotate;

    m_matrix.setScale(CPoint<float>(1.5, 1.3, 1.7));
    m_matrix.rotate(CPoint<float>(57, 287, 79));
    m_matrix.translate(CPoint<float>(200, 700, 880));
    m_matrix.rotate(CPoint<float>(57, 287, 79));
    m_matrix.translate(CPoint<float>(200, 700, 880));
    m_matrix.rotate(CPoint<float>(57, 287, 79));
    m_matrix.setScale(CPoint<float>(-1.5, -1.3, -1.7));

    m_matrixRotate.rotate(CPoint<float>(57, 287, 79));
    m_matrixRotate.rotate(CPoint<float>(57, 287, 79));
    m_matrixRotate.rotate(CPoint<float>(57, 287, 79));

    return 0;
}*/

/*#include <cstdint>
#include <vector>
#include <boost/crc.hpp>

int main()
{
    boost::crc_optimal<16, 0xFFFF, 0, 0, false, false> result;

    std::vector<uint8_t> buffer = {8,1,1,0,1,0};

    result.process_bytes( buffer.data(), buffer.size() );

    uint8_t value = result.checksum();

    return 0;
}*/

/*class iStatic
{
public:
    static int myStatic;
};

int iStatic::myStatic = 5;

class CMyStatic1 : public iStatic
{};

class CMyStatic2 : public iStatic
{};

int main()
{
    iStatic miStatic;
    CMyStatic1 myStatic1;
    CMyStatic2 myStatic2;
    
    myStatic2.myStatic = 10;
    
    return 0;
}*/


/*#include <iostream>

int main()
{
    // This does not correlate to the number of pi digits currently 1,717 digits
    const int max_range = 6000;
    
    int r[max_range + 1];
    int i, k, b, d, c = 0;
    for (i = 0; i < max_range; i++) {
        r[i] = 2000;
    }
    for (k = max_range; k > 0; k -= 14) {
        d = 0;
        i = k;
        for (;;) {
            d += r[i] * 10000;
            b = 2 * i - 1;
            r[i] = d % b;
            d /= b;
            i--;
            if (i == 0) break;
            d *= i;
        }
        printf("%.4d", c + d / 10000);
        c = d % 10000;
    }
    
    return 0;
}*/

/*#include <iostream>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <script/scripthighresolutiontimer.h>

#include <scriptstdstring/scriptstdstring.h>

int main()
{
    // Load the script list table
    CScriptMgr::Instance().loadListTable( "scriptListTable.lst" );
    
    // Register the script items
    RegisterStdString( CScriptMgr::Instance().getEnginePtr() );
    NScriptGlobals::Register();
    NScriptHighResolutionTimer::Register();

    CScriptMgr::Instance().loadGroup("(test)");

    CScriptMgr::Instance().prepare("(test)", "Script_Run");
    
    CScriptMgr::Instance().update();
    
    return 0;
}*/

/*#include <iostream>
#include <thread>
#include <managers/signalmanager.h>

void threadFunc()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "threadFunc call signal manager: " << std::this_thread::get_id() << std::endl;
    
    CSignalMgr::Instance().Broadcast_LoadSignal();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void TestFunc()
{
    std::cout << "TestFunc called by signal manager: " << std::this_thread::get_id() << std::endl;
}

int main()
{
    CSignalMgr::Instance().Connect_Load( TestFunc );
    
    std::cout << "Main thread: " << std::this_thread::get_id() << std::endl;
    
    std::thread thread(threadFunc);
    
    thread.join();
    
    return 0;
}*/

/*#include <iostream>
#include <future>
#include <thread>

int main()
{
    auto sleep = [](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 1;
    };
    
    std::packaged_task<int()> task(sleep);

    auto f = task.get_future();
    
    std::thread thread(std::move(task));
    thread.detach();
    
    std::cout << "Thread started. Waiting on future" << std::endl;
    
    std::cout << "Future returned: " << f.get() << std::endl;

    return 0;
}*/

/*#include <iostream>
#include <stdint.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/matrix.h>

int main()  // 7124.35 ms
{
    long long int value = 0;
    long long int test;
    long long int maxValue = 10000000;
   
    std::cout << "Test started..." << std::endl;
    
    CHighResTimer::Instance().timerStart();
    
    do
    {
        test = ++value;

        do
        {
            if ((test & 1) == 0)
                test /= 2;
            //if( (test % 2) == 0 )
            //    test /= 2;
            else
                test = (test * 3) + 1;
        }
        while (test > 1);
    }
    while ((test > 0) && (value < maxValue));
    
    std::cout << "Value: " << value << ", Execution time: " << CHighResTimer::Instance().timerStop() << std::endl;
    
    return 0;
}*/

/*#include <utilities/xmlParser.h>


int main()
{
    XMLNode node = XMLNode::openFileHelper( "actorDataList.lst", "spriteList" );
    
    const XMLNode collisionNode = node.getChildNode("collision");
    if( !collisionNode.isEmpty() )
    {
        return 1;
    }
    
    return 0;
}*/

/*#include <utilities/highresolutiontimer.h>
#include <iostream>
#include <thread>

int main()
{
    CHighResTimer::Instance().TimerStart();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    
    std::cout << "Execution time: " << CHighResTimer::Instance().TimerStop() << "\n";
    
    return 0;
}*/

/*#include <iostream>
#include <limits>
#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <list>
#include <queue>
#include <memory>

#include <physics/physicscomponent2d.h>
#include <physics/physicscomponent3d.h>
#include <script/scriptcomponent.h>
#include <sprite/sprite.h>
#include <2d/object2d.h>

int main()
{
    int scrSize = sizeof(CScriptComponent);    // 24
    int phySize2d = sizeof(CPhysicsComponent2D); // 40
    int phySize3d = sizeof(CPhysicsComponent3D); // 24
    int objSize = sizeof(CObject);             // 80
    int objSize2D = sizeof(CObject2D);         // 152
    
    int sprSize = sizeof(CSprite);             // 128
    
    int strSize = sizeof(std::string);         // 32
    int mapSize = sizeof(std::map<std::string, std::string>); // 48
    int vecSizeInt = sizeof(std::vector<int>);    // 24
    int vecSizePtr = sizeof(std::vector<int *>);    // 24
    int lstSize = sizeof(std::list<int>);      // 24
    int queSize = sizeof(std::queue<int>);     // 80
    int deqSize = sizeof(std::deque<int>);     // 80
    
    int unqSize = sizeof(std::unique_ptr<int>);  // 8
    int shrSize = sizeof(std::shared_ptr<int>);  // 16
    
    int max = std::numeric_limits<int8_t>::max();
    int min = std::numeric_limits<int8_t>::min();

    return 0;
}*/

/*#include <iostream>

class CFoo
{
public:
    void Set( int val)
    { value = val; }
    
    int Get() const
    { return value; }
    
private:
    int value;
};

int main()
{
    CFoo foo;
    const CFoo * const pFoo = &foo;
    const CFoo & rFoo = foo;
    
    
    pFoo->Get();
    rFoo.Get();
    
    foo.Get();
    
    return 0;
}*/

/*#include <algorithm>
#include <vector>
#include <iostream>
#include <ctime> 

int main()
{
    std::vector<int> shuffle = {0,1,2,3,4,5};
    std::srand ( unsigned ( std::time(0) ) );
    
    for( int i = 0; i < 5; ++i )
    {
        std::random_shuffle( shuffle.begin(), shuffle.end() );

        for( auto iter : shuffle )
            std::cout << iter << " ";

        std::cout << std::endl;
    }
    
    return 0;
}*/


//#include "mt19937ar.h"
/*#include <random>
#include <chrono>
#include <iostream>

int main()
{
    auto begin = std::chrono::high_resolution_clock::now();
    
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto seed = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();

    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(0,50);
    distribution(generator);
    
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    
    std::cout << duration << std::endl;*/

    //unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;

    /*unsigned long init[5], length=5;
    for( unsigned long i = 0; i < length; ++i )
    {
        auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
        init[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    }*/
    /*init_genrand(seed);
    //init_by_array(init, length);
    printf("1000 outputs of genrand_int32()\n");
    for (i=0; i<1000; i++) {
      printf("%10lu ", genrand_int32());
      if (i%5==4) printf("\n");
    }
    printf("\n1000 outputs of genrand_real2()\n");
    for (i=0; i<1000; i++) {
      printf("%10.8f ", genrand_real2());
      if (i%5==4) printf("\n");
    }*/

    // Wait here before exiting
    /*getchar();

    return 0;
}*/

/*
#include <iostream>
#include <chrono>
#include <utilities/IXMLParser.h>
#include <utilities/xmlParser.h>

int main()
{
    IXMLDomParser iDom;
    
    auto start = std::chrono::high_resolution_clock::now().time_since_epoch();
    
    ITCXMLNode xMainNode = iDom.openFileHelper("menuOther.lst", "objectDataList2D");
    
    auto finish = std::chrono::high_resolution_clock::now().time_since_epoch();
    std::cout << "New Parser time: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ns\n";

    
    start = std::chrono::high_resolution_clock::now().time_since_epoch();
    
    // this open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( "menuOther.lst", "objectDataList2D" );
    
    finish = std::chrono::high_resolution_clock::now().time_since_epoch();
    std::cout << "Old Parser time: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ns\n";
    
    
    if( xMainNode.isEmpty() )
        std::cout << "Node Empty" << std::endl;
    else
    {
        ITCXMLNode node = xMainNode.getChildNode( "objectList" );
        std::cout << "Number of children: " << node.nChildNode() << std::endl;
        
        std::cout << "First Attribute: " << node.getChildNode().getAttributeValue() << std::endl;
    }
    
    if( mainNode.isEmpty() )
        std::cout << "Node Empty" << std::endl;
    else
    {
        XMLNode node = mainNode.getChildNode( "objectList" );
        std::cout << "Number of children: " << node.nChildNode() << std::endl;
        
        std::cout << "First Attribute: " << node.getChildNode().getAttributeValue() << std::endl;
    }
    
    
    start = std::chrono::high_resolution_clock::now().time_since_epoch();
    
    IXMLReaderFile iReader("twitter.json");
    //IXMLReaderFile iReader("test.json");
    IJSONPullParser pp(&iReader);
    IXMLDomParser jsonDom;
    ITCXMLNode jsonNode=jsonDom.parse(&pp);
    
    std::cout << jsonNode.getElementByPath("results/[2]/metadata/recent_retweets") << std::endl;
    //std::cout << jsonNode.getElementByPath("customers/[1]/firstName") << std::endl;
    
    //std::cout << jsonNode.getChildNode().getElementByPath("lastName") << std::endl;
    
    finish = std::chrono::high_resolution_clock::now().time_since_epoch();
    std::cout << "JSON Parser time: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ns\n";
    
   
    std::cout << "The End" << std::endl;
    
    

    // Wait here before exiting
    #if defined(_WINDOWS)
    getchar();
    #endif

    return 0;
}
*/

/*
#include <iostream>
#include <chrono>
#include <2d/physicscomponent2d.h>
#include <memory>

int main()
{
    auto start = std::chrono::high_resolution_clock::now().time_since_epoch();

    //auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(start).count() << "ns\n";

    // Wait here before exiting
    getchar();

    return 0;
}
*/

/*
#include <SDL.h>
#include <iostream>
#include <cinttypes>
#include <stdint.h>
#include <thread>
#include <mutex>
#include <utilities/highresolutiontimer.h>
//#include <boost/multiprecision/cpp_int.hpp>

// Largest value for a 32 bit uint - overflow value: 159487 max count 382 of 156159
// Largest value for a 64 bit uint - overflow value: 23035537407 max count 1213 of 17828259369
// Largest value for a 128 bit uint - overflow value: 0000000 max count 1219 of 31694683369


//using namespace boost::multiprecision;
//typedef unsigned int calcType;
typedef uint64_t calcType;
//typedef __uint128_t calcType;
//typedef int calcType;

//calcType OVERFLOW_MASK = 0xC0000000; // 32-bit
calcType OVERFLOW_MASK = 0xC000000000000000; // 64-bit
//calcType OVERFLOW_MASK = 0xC0000000000000000000000000000000; // 128-bit

calcType value = 1;
calcType maxCountValue = 0;
std::mutex mutex;
bool testFinished = false;
int maxCount = 0;

void timer_callback()
{
    if( !testFinished )
        std::cout << "Value Reached: " << value << " - Max Count: " << maxCount << " of value: " << maxCountValue << std::endl;
}

class later
{
public:
    template <class callable, class... arguments>
    later(int after, bool async, callable&& f, arguments&&... args)
    {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        if (async)
        {
            std::thread([after, task]() {
                do
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(after));
                    task();
                } while (!testFinished);
            }).detach();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }
    }

};

void SpeedTest()
{
    calcType test;
    calcType overflow;
    calcType MASK = OVERFLOW_MASK;
   
    do
    {
        test = ++value;

        do
        {
            if ((test & 1) == 0)
                test >>= 1;
            //if( (test % 2) == 0 )
                //test /= 2;
            else
            {
                overflow = test;
                test = (test * 3) + 1; // 5804

                if( (test & MASK) && (((test - 1) / 3) != overflow) )
                {
                    test = 0;
                    testFinished = true;
                    std::cout << "Overflow Value: " << value << std::endl;
                }
            }
        }
        while (test > 1);
    }
    while ((test > 0) && (value < 1000000));
}

void Calc()
{
    calcType test;
    calcType overflow;
    calcType preTestValue;
    calcType MASK = OVERFLOW_MASK;
    bool overFlowed(false);
    int counter;

    do
    {
        mutex.lock();
        preTestValue = test = ++value;
        mutex.unlock();

        counter = 0;

        do
        {
            if ((test & 1) == 0) // faster
                test >>= 1;
            //if ((test % 2) == 0)
                //test /= 2;
            else
            {
                overflow = test;
                test = (test * 3)  + 1;

                // Test for overflow
                if( (test & MASK) && (((test - 1) / 3) != overflow) )
                {
                    overFlowed = true;

                    mutex.lock();
                    std::cout << "Overflow Value: " << preTestValue << " - Value to overflow: " << overflow << " - Max Count: " << maxCount << " of value: " << maxCountValue << std::endl;
                    mutex.unlock();
                    
                    test = 0;
                }
            }

            ++counter;
        }
        while (test > 1 );

        mutex.lock();
        if (maxCount < counter)
        {
            maxCount = counter;
            maxCountValue = preTestValue;
        }
        mutex.unlock();
    }
    while ((test > 0) && !testFinished );

    testFinished = true;

    if (test < 0 )
    {
        mutex.lock();
        if( !overFlowed )
            std::cout << "Exit Value: " << value << std::endl;
        mutex.unlock();
    }
}


int main()
{
    // Speed test
    std::cout << "Test started..." << std::endl;
    
    
    CHighResTimer::Instance().TimerStart();
    
    value = 1;
    SpeedTest();
    
    CHighResTimer::Instance().TimerStop();
    
    std::cout << "Execution time: " << CHighResTimer::Instance().GetElapsedTime() << std::endl;
    
    
    
    // Test code;
    //value = 23035530000;
   // Calc();
    
    
    // Full execution
    //value = 1;

    //later later_test1(60000 * 1, true, &timer_callback);
   
    //std::thread thread1(Calc);
    //std::thread thread2(Calc);
    //std::thread thread3(Calc);
    //std::thread thread4(Calc);
    //std::thread thread5(Calc);
    //std::thread thread6(Calc);
    //std::thread thread7(Calc);
    //std::thread thread8(Calc);

    //thread1.join();
    //thread2.join();
    //thread3.join();
    //thread4.join();
    //thread5.join();
    //thread6.join();
    //thread7.join();
    //thread8.join();

    //std::cout << "Final Value: " << value << std::endl;
    
    
    
    
    // Wait here before exiting
    //getchar();
}*/

/*#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <utilities/threadpool.h>

class CThreadTest
{
public:
    int Calc( int n )
    {
        int firstTerm = 1, secondTerm = 1, nextTerm;
        
        while ( nextTerm < n)
        {
            nextTerm = firstTerm + secondTerm;
            firstTerm = secondTerm;
            secondTerm = nextTerm;
        }
        
        return nextTerm;
    }
};

// std::this_thread::sleep_for(std::chrono::seconds(1));

int main()
{
    std::vector< std::future<void> > jobVec;

    using namespace std::chrono_literals;
    
    auto & mutex = CThreadPool::Instance().getMutex();
    CThreadPool::Instance().init( 1, 2 );
    
    for( int i = 0; i < 8; ++i )
    {
        jobVec.emplace_back(
            CThreadPool::Instance().postRetFut([&mutex] {
                {
                    std::unique_lock<std::mutex> lock( mutex );
                    std::cout << "Task started in thread: " << std::this_thread::get_id() << std::endl;
                }
                
                //std::this_thread::sleep_for(std::chrono::seconds(1));
                std::this_thread::sleep_for(1s);
                
                {
                    std::unique_lock<std::mutex> lock( mutex );
                    std::cout << "Task finished in thread: " << std::this_thread::get_id() << std::endl;
                }
            })
        );

        CThreadPool::Instance().post([&mutex] {
            {
                std::unique_lock<std::mutex> lock( mutex );
                std::cout << "Task started in thread: " << std::this_thread::get_id() << std::endl;
            }
            
            //std::this_thread::sleep_for(std::chrono::seconds(1));
            std::this_thread::sleep_for(1s);
            
            {
                std::unique_lock<std::mutex> lock( mutex );
                std::cout << "Task finished in thread: " << std::this_thread::get_id() << std::endl;
            }
        });
    }

    //std::this_thread::sleep_for(std::chrono::seconds(2));

    // Wait for all the jobs to finish
    // get() is a blocking call, waiting for each job to return
    //for( auto && iter : jobVec ) iter.get();

    // Wait for posts that don't return a future
    CThreadPool::Instance().wait();

    return 0;
}*/



/*#include <iostream>
#include <common/point.h>
#include <common/defs.h>

int main(int argc, char **argv)
{
    CPoint<float> point1(-15,10);
    CPoint<float> point2(10,10);
    float magnitude1 = point1.GetLength2D();
    float magnitude2 = point2.GetLength2D();

    float scalar = point1.GetDotProduct2D(point2);
    
    float scalarNomalized = scalar / (magnitude1 * magnitude2);
    
    float radians = acos( scalarNomalized );

    float degrees = radians * defs_RAD_TO_DEG;

    std::cout << "Using vectors: scalar: " << scalar << ", normalized scaler: " << scalarNomalized <<  ", radians: " << radians << ", degrees: " << degrees << std::endl;
    
    
    point1.Normalize2D();
    point2.Normalize2D();

    float scalar2 = point1.GetDotProduct2D(point2);
    
    float radians2 = acos( scalar2 );
    
    float degrees2 = radians2 * defs_RAD_TO_DEG;
    
    std::cout << "Using unit vectors: scalar: " << scalar2 << ", radians: " << radians2 << ", degrees: " << degrees2 << std::endl;
    

    system("PAUSE");

    return 0;
}*/


/*#include <iostream>
#include <common/point.h>
#include <common/defs.h>

int main(int argc, char **argv)
{
    CPoint<float> light(6,8,5);
    CPoint<float> normal(0.5, 0.5, 0);
    float lightMag = light.GetLength();

    // Can do it this way
    float angel = ( normal.x * light.x +
                    normal.y * light.y +
                    normal.z * light.z ) / lightMag;
    
    std::cout << "Light Angle: " << angel << std::endl;
    
    // or this way
    light.Normalize();
    
    angel = ( normal.x * light.x +
              normal.y * light.y +
              normal.z * light.z );
    
    std::cout << "Light Angle: " << angel << std::endl;

    system("PAUSE");

    return 0;
}*/

/*#include <iostream>
#include <random>

#define NUM_PARTICLES 1

// Two dimensional vector.
typedef struct {
    float x;
    float y;
} Vector2;

// Two dimensional particle.
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float mass;
} Particle;

// Global array of particles.
Particle particles[NUM_PARTICLES];

// Prints all particles' position to the output. We could instead draw them on screen
// in a more interesting application.
void  PrintParticles() {
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        Particle *particle = &particles[i];
        printf("particle[%i] (%.2f, %.2f)\n", i, particle->position.x, particle->position.y);
    }
}

// Initializes all particles with random positions, zero velocities and 1kg mass.
void InitializeParticles() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,50);
  
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        particles[i].position = (Vector2){distribution(generator), distribution(generator)};
        particles[i].velocity = (Vector2){0, 0};
        particles[i].mass = 1;
    }
}

// Just applies Earth's gravity force (mass times gravity acceleration 9.81 m/s^2) to each particle.
Vector2 ComputeForce(Particle *particle) {
    return (Vector2){0, particle->mass * -9.81};
}

int main (int argc, char *argv[]) {
    float totalSimulationTime = 10; // The simulation will run for 10 seconds.
    float currentTime = 0; // This accumulates the time that has passed.
    float dt = 1; // Each step will take one second.
    
    InitializeParticles();
    PrintParticles();
    
    while (currentTime < totalSimulationTime) {
        // We're sleeping here to keep things simple. In real applications you'd use some
        // timing API to get the current time in milliseconds and compute dt in the beginning 
        // of every iteration like this:
        // currentTime = GetTime()
        // dt = currentTime - previousTime
        // previousTime = currentTime
        //sleep(dt);

        for (int i = 0; i < NUM_PARTICLES; ++i) {
            Particle *particle = &particles[i];
            Vector2 force = ComputeForce(particle);
            Vector2 acceleration = (Vector2){force.x / particle->mass, force.y / particle->mass};
            particle->velocity.x += acceleration.x * dt;
            particle->velocity.y += acceleration.y * dt;
            particle->position.x += particle->velocity.x * dt;
            particle->position.y += particle->velocity.y * dt;
        }
        
        PrintParticles();
        currentTime += dt;
    }
}*/


/*#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>

class CCat
{};

class CAnimal
{};

class CDog : public CAnimal
{
public:
    CDog(const CCat & cat, int){};
    ~CDog() { std::cout << "CDog::dtor" << std::endl; }
    
    int test = 0;
    double myDouble = 1.0;
};

int main (int argc, char *argv[]) 
{
    //std::unique_ptr<CDog[]> unique_ary(new CDog[3]);
    //std::shared_ptr<CDog> shared_ary(new CDog[3], std::default_delete<CDog[]>());
    
    std::map<const std::string, CDog> dogMap;
    
    int test1 = 5;
    int test2 = 10;
    std::string testStr = "test";
    CCat cat;
    
    dogMap.emplace(std::piecewise_construct,  std::forward_as_tuple(testStr),  std::forward_as_tuple(cat, test2));
    
    std::vector<CDog> dogVec;
    
    dogVec.emplace_back( cat, test2 );
}*/


/*#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
int nthreads, tid;

// Fork a team of threads giving them their own copies of variables
#pragma omp parallel private(nthreads, tid)
  {

  // Obtain thread number
  tid = omp_get_thread_num();
  printf("Hello World from thread = %d\n", tid);

  // Only master thread does this
  if (tid == 0) 
    {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }

  }  // All threads join master thread and disband

  system("PAUSE");

}*/


/*#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/move/move.hpp>
#include <iostream>

boost::mutex mutex;

int sleep_print(int seconds)
{
    mutex.lock();
    std::cout << "Waiting in thread (" << seconds << ")" << std::endl;
    mutex.unlock();
    boost::this_thread::sleep(boost::posix_time::milliseconds(seconds));
    mutex.lock();
    std::cout << "leaving thread (" << seconds << ")" << std::endl;
    mutex.unlock();
    return 0;
}

void push_job(int seconds, boost::asio::io_service& io_service, std::vector<boost::shared_future<int> >& pending_data)
{
    typedef boost::packaged_task<int> task_t;
    boost::shared_ptr<task_t> task = boost::make_shared<task_t>(boost::bind(&sleep_print, seconds));
    boost::shared_future<int> fut(task->get_future());
    pending_data.push_back(fut);
    io_service.post(boost::bind(&task_t::operator(), task));
}

int main()
{
    boost::asio::io_service io_service;
    boost::scoped_ptr<boost::thread_group> threads( new boost::thread_group );
    boost::asio::io_service::work work(io_service);

    for (size_t i = 0; i < boost::thread::hardware_concurrency() ; ++i)
    {
	    threads->create_thread(boost::bind(&boost::asio::io_service::run,
		    &io_service));
    }

    std::vector<boost::shared_future<int> > pending_data; // vector of futures

    push_job(1000, io_service, pending_data);
    push_job(1001, io_service, pending_data);
    push_job(1002, io_service, pending_data);
    push_job(1003, io_service, pending_data);
    push_job(1004, io_service, pending_data);
    push_job(1005, io_service, pending_data);
    push_job(1006, io_service, pending_data);
    push_job(1007, io_service, pending_data);
    push_job(1008, io_service, pending_data);
    push_job(1009, io_service, pending_data);
    push_job(1010, io_service, pending_data);

    boost::wait_for_all(pending_data.begin(), pending_data.end());

    // Stop all thread activity before deleting the thread group
    io_service.stop();

    // Allow threads to clean up while shared task pointers are still in scope.
    threads.reset();

    system("PAUSE");
	
    return 0;
}*/

/*#define STRICT
#include <fstream>
#include <iostream>
#include <vector>

#include <common/point.h>
#include <common/defs.h>

#include <stdio.h>


int main(int argc, char **argv)
{
    float tmp1 = 2.1679230356;
    float tmp2 = 0.00000000000000001;

    std::cout << "value as int1: " << *(int*)&tmp1 << ", value as int2: " << *(int*)&tmp2 << std::endl;

    system("PAUSE");

	return 0;
}*/

/*#include <iostream>
#include <common/point.h>
#include <common/defs.h>

int main(int argc, char **argv)
{
    CPoint<float> point1(-6,8);
    CPoint<float> point2(5,12);
    float magnitude1 = point1.GetLength();
    float magnitude2 = point2.GetLength();

    float scalar = point1.GetDotProduct(point2);
    
    float scalarNomalized = scalar / (magnitude1 * magnitude2);
    
    float radians = acos( scalarNomalized );

    float degrees = radians * defs_RAD_TO_DEG;

    std::cout << "Using vectors: scalar: " << scalar << ", normalized scaler: " << scalarNomalized <<  ", radians: " << radians << ", degrees: " << degrees << std::endl;
    
    
    point1.Normalize2D();
    point2.Normalize2D();

    float scalar2 = point1.GetDotProduct(point2);
    
    float radians2 = acos( scalar2 );
    
    float degrees2 = radians2 * defs_RAD_TO_DEG;
    
    std::cout << "Using unit vectors: scalar: " << scalar2 << ", radians: " << radians2 << ", degrees: " << degrees2 << std::endl;
    

    system("PAUSE");

    return 0;
}*/

/*int main(int argc, char **argv)
{
    CPoint<float> point1(-6,8);
    CPoint<float> point2(5,12);
    point1.Normalize();
    point2.Normalize();
    float scalar, radians, degrees;

    scalar = point1.GetDotProduct(point2);

    radians = acos(scalar);

    degrees = radians * defs_RAD_TO_DEG;

    std::cout << "scalar: " << scalar << ", radians: " << radians << ", degrees: " << degrees << std::endl;

	system("PAUSE");

	return 0;
}*/


/*int main(int argc, char **argv)
{
	int test(6);

	auto func = [&] { std::cout << "The no is: " << test << std::endl; };
	func();

	system("PAUSE");

	return 0;
}*/

/*
#define STRICT
#include <fstream>
#include <iostream>

#include <stdio.h>


unsigned int ConvertEndianness( unsigned long value )
{
	return (((value&0x000000FF)<<24)+((value&0x0000FF00)<<8)+
			((value&0x00FF0000)>>8)+((value&0xFF000000)>>24));
}


int main(int argc, char **argv)
{ 
	FILE *pFile = fopen("C:/screen shots/picture001.png", "rb");

	struct size
	{
		unsigned int width, height;
	};

	size tmp;

	fseek( pFile, 16, SEEK_SET );
	fread( &tmp, 1, sizeof(tmp), pFile );
	//std::ifstream in( "C:\\screen shots\\picture001.png" );

	//in.seekg(16);
	//in.read((char *)&width, 4);
	//in.read((char *)&height, 4);

	tmp.width = ConvertEndianness(tmp.width);
	tmp.height = ConvertEndianness(tmp.height);

	std::cout << "C:\\screen shots\\picture001.png" << " is " << tmp.width << " pixels wide and " << tmp.height << " pixels high.\n";

	system("PAUSE");

	return 0;
}
*/


/*
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <iostream>
#include <string>
#include <utilities/genfunc.h>
using namespace std;


int main(int argc, char **argv)
{

	WORD version = GetVersion();
	WORD major = (WORD) (LOBYTE(version));
	WORD minor = (WORD) (HIBYTE(version));

	cout << "Major: " << major << "; Minor: " << minor << endl;

	system("PAUSE");

	return 0;
}
*/


/*
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#include <iostream>
#include <string>
#include <vector>
using namespace std;



int main(int argc, char **argv)
{



	system("PAUSE");

	return 0;
}
*/

/*
int main(int argc, char **argv)
{
	int StudentAge;

	try {
		cout << "Student Age: ";
		cin >> StudentAge;

		if(StudentAge < 0)
			throw exception( "Positive Number Required!" );

		cout << "\nStudent Age: " << StudentAge << "\n\n";
	}
	catch( std::exception & ex )
	{
		cout << "Error: " << ex.=.what();
	}

	cout << "\n";



	system("PAUSE");

	return 0;
}
*/

/*
#include <fstream>

// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
class gps_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & degrees;
        ar & minutes;
        ar & seconds;
    }
    int degrees;
    int minutes;
    float seconds;
public:
    gps_position(){};
    gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {}
};

int main() {
    // create and open a character archive for output
    std::ofstream ofs("c:/serialized.txt");

    // create class instance
    const gps_position g(35, 59, 24.567f);

    // save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << g;
    	// archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its orginal state
    gps_position newg;
    {
        // create and open an archive for input
        std::ifstream ifs("c:/serialized.txt");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> newg;
        // archive and stream closed when destructors are called
    }
    return 0;
}
*/


/*
#include <fstream>

// include headers that implement a archive in simple text format
#include <boost/archive/xml_oarchive.hpp> // saving
#include <boost/archive/xml_iarchive.hpp> // loading

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <boost/filesystem.hpp>

#include "boost/date_time/posix_time/posix_time.hpp"

#include <boost/scoped_array.hpp>

#include <utilities/genfunc.h>

#include <ShlObj.h>

//#include <boost/serialization/nvp.hpp>

/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
class CGPS_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar 
			& BOOST_SERIALIZATION_NVP(degrees)
            & BOOST_SERIALIZATION_NVP(minutes)
            & BOOST_SERIALIZATION_NVP(seconds)
		    & BOOST_SERIALIZATION_NVP(vecString)
			// or
			//& boost::serialization::make_nvp("the_degrees", degrees)
			//& boost::serialization::make_nvp("the_minutes", minutes)
			//& boost::serialization::make_nvp("the_seconds", seconds)
			//& boost::serialization::make_nvp("the_vecString", vecString)
			
			;
    }
    int degrees;
    int minutes;
    float seconds;
	std::vector<std::string> vecString;
public:
    CGPS_position(){};
    CGPS_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {   vecString.push_back("I am the walrus");
		vecString.push_back("candy");
		vecString.push_back("jaw breakers");
		vecString.push_back("lemon heads"); }
};

int main()
{

	//wchar_t* path = NULL;
	//SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);

	//char charPath[MAX_PATH];
	//wcstombs( charPath, path, sizeof(charPath) );
	// mbstowcs - Convert multibyte string to wide-character string

	//CoTaskMemFree(path);

	std::string path = NGenFunc::GetWindowsFolderPath( FOLDERID_Documents );

	bool tmp = boost::filesystem::is_regular_file( path + "\\GameMaker\\controllerMapping.cfg" );

	std::time_t time = boost::filesystem::last_write_time(path + "\\GameMaker\\controllerMapping.cfg");

	char timeChar[200];
	std::strftime( timeChar, 200, "%H:%M%p  %m/%d/%y", std::localtime(&time) );

	std::string timeStr = timeChar;
	std::transform(timeStr.begin(), timeStr.end(), timeStr.begin(), ::tolower);

	//boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

	boost::posix_time::time_duration duration(0,0,0);

	boost::posix_time::time_duration conversion( boost::posix_time::duration_from_string( boost::posix_time::to_simple_string(duration) ) );
	//boost::posix_time::time_duration conversion( boost::posix_time::duration_from_string("0") );

	
	std::cout << "Time Duration: " << conversion.hours() << ":" << conversion.minutes() << std::endl;
	std::cout << "As string: " << boost::posix_time::to_simple_string(duration) << std::endl;

	//boost::posix_time::time_duration now(0,14,32);

	//boost::posix_time::time_duration later(150,24,55);

	//boost::posix_time::time_duration remaining = later - now;

	//boost::posix_time::time_duration remaining2( boost::posix_time::duration_from_string(boost::posix_time::to_simple_string(remaining)) );

	//std::cout << "Now the modification time is " << timeStr << std::endl;
	//std::cout << "Time Difference: " << remaining2.hours() << ":" << remaining2.minutes() << std::endl;
	//std::cout << "As string: " << boost::posix_time::to_simple_string(remaining2) << std::endl;

	//boost::filesystem::path directory( path + "/RabidSquirrelGames/Lost" );
	boost::filesystem::path directory( "./" );
	boost::filesystem::directory_iterator iter(directory), end;

	for(;iter != end; ++iter)
		std::cout << "file NAme: " << iter->path().filename().string() << std::endl;


    // save data to archive
    {
		CGPS_position gps_position(35, 59, 24.567f);

		std::ofstream ofs("c:/serialized.xml");
        boost::archive::xml_oarchive oa(ofs);
        // write class instance to archive
        oa << BOOST_SERIALIZATION_NVP(gps_position);
		// or
		//oa << boost::serialization::make_nvp("the_gps_position", gps_position);
    	// archive and stream closed when destructors are called
    }

    // read data from archive
    {
		CGPS_position gps_position;

        // create and open an archive for input
        std::ifstream ifs("c:/serialized.xml");
        boost::archive::xml_iarchive ia(ifs);
        // read class state from archive
		ia >> BOOST_SERIALIZATION_NVP(gps_position);
		// or
		//ia >> boost::serialization::make_nvp("the_gps_position", gps_position);
        // archive and stream closed when destructors are called
    }

	system("PAUSE");

    return 0;
}

*/


/*
 // Boost serialization Example
#include <fstream>

#include <boost/serialization/vector.hpp>

// a portable text archive
#include <boost/archive/text_oarchive.hpp> // saving
#include <boost/archive/text_iarchive.hpp> // loading

// a portable text archive using a wide character stream
#include <boost/archive/text_woarchive.hpp> // saving
#include <boost/archive/text_wiarchive.hpp> // loading

// a portable XML archive
#include <boost/archive/xml_oarchive.hpp> // saving
#include <boost/archive/xml_iarchive.hpp> // loading

// a portable XML archive which uses wide characters - use for utf-8 output
#include <boost/archive/xml_woarchive.hpp> // saving
#include <boost/archive/xml_wiarchive.hpp> // loading

// a non-portable native binary archive
#include <boost/archive/binary_oarchive.hpp> // saving
#include <boost/archive/binary_iarchive.hpp> // loading


/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
class gps_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar
			& degrees
			& minutes
            & seconds
			& testVec;
    }
    int degrees;
    int minutes;
    float seconds;
	std::vector<int> testVec;
public:
    gps_position(){};
    gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {   testVec.push_back(0);
		testVec.push_back(1);
		testVec.push_back(2);
		testVec.push_back(3);
		testVec.push_back(4);
		testVec.push_back(5);
		testVec.push_back(6);
		testVec.push_back(7);
		testVec.push_back(8);
		testVec.push_back(9); }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(gps_position)

int main() {
    // create and open a character archive for output
    std::ofstream ofs_txt("c:\\serialized.txt");
	//std::ofstream ofs_bin("c:\\serialized.dat");
	//std::ofstream ofs_xml("c:\\serialized.xml");

    // create class instance
    const gps_position g(35, 59, 24.567f);

    // save data to archive
    {
        
		boost::archive::text_oarchive oa_txt(ofs_txt);
		//boost::archive::binary_oarchive oa_bin(ofs_bin);
		//boost::archive::text_oarchive oa_xml(ofs_xml);
        // write class instance to archive
        oa_txt << g;
		//oa_bin << g;
		//oa_xml << g;
    	// archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its orginal state
    gps_position newg;
    {
        // create and open an archive for input
        std::ifstream ifs_txt("c:\\serialized.txt");
		//std::ifstream ifs_bin("c:\\serialized.dat");
		//std::ifstream ifs_xml("c:\\serialized.xml");
        boost::archive::text_iarchive ia_txt(ifs_txt);
		//boost::archive::binary_iarchive ia_bin(ifs_bin);
		//boost::archive::xml_iarchive ia_xml(ifs_xml);
        // read class state from archive
        ia_txt >> newg;
		//ia_bin >> newg;
		//ia_xml >> newg;
        // archive and stream closed when destructors are called
    }

	system("PAUSE");
    return 0;
}
*/

/* // Boost State Chart Example

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/mpl/list.hpp>
#include <iostream>

using namespace std;

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

struct EvPressButton : sc::event<EvPressButton>
{
  EvPressButton()
  {
    cout << "Button is pressed" << endl;
  }
};

struct EvReleaseButton : sc::event<EvReleaseButton>
{
  EvReleaseButton()
  {
    cout << "Button is released" << endl;
  }
};

struct EvToggleLight : sc::event<EvToggleLight> {};

struct Active;
struct Switch : sc::state_machine<Switch, Active> {};

struct ButtonReleased;
struct ButtonPressed;
struct LightOff;
struct LightOn;

struct Active: sc::simple_state<Active, Switch,mpl::list<ButtonReleased, LightOff> > {};
struct ButtonPressed : sc::state<ButtonPressed, Active::orthogonal<0> >
{
  typedef sc::transition<EvReleaseButton, ButtonReleased> reactions;
  ButtonPressed(my_context ctx) : my_base(ctx)
  {
    post_event(EvToggleLight());
  }
};

struct ButtonReleased : sc::simple_state<ButtonReleased, Active::orthogonal<0> >
{
  typedef sc::transition<EvPressButton, ButtonPressed> reactions;
};

struct LightOff : sc::simple_state<LightOff, Active::orthogonal<1> >
{
  typedef sc::transition<EvToggleLight, LightOn> reactions;
  LightOff()
  {
    cout << "Light is off" << endl;
  }
};

struct LightOn : sc::simple_state<LightOn, Active::orthogonal<1> >
{
  typedef sc::transition<EvToggleLight, LightOff> reactions;
  LightOn()
  {
    cout << "Light is on" << endl;
  }
};

int main()
{
  Switch sw;
  sw.initiate();
  for (int i = 0; i < 5; i++)
  {
    sw.process_event(EvPressButton());
    sw.process_event(EvReleaseButton());
  }

  system("PAUSE");
  return 0;
}
*/

