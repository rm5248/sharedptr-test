#include <memory>
#include <stdio.h>
#include <time.h>

class BaseClass {
public:
    BaseClass(){}
    virtual ~BaseClass(){}

    virtual void someOperation() = 0;
};

class DerivedClass : public BaseClass{
public:
    DerivedClass(){}
    ~DerivedClass(){}

    void someOperation(){
        static int value = 0;
        for( int x = 0; x < 100; x++ ){
            value += x;
        }
    }
};

class NonderivedClass{
public:
    NonderivedClass(){}
    ~NonderivedClass(){}

    void someOperation(){
        static int value = 0;
        for( int x = 0; x < 100; x++ ){
            value += x;
        }
    }
};

/**
 * Pass in a shared pointer to the function
 */
void sharedptr_Base( std::shared_ptr<BaseClass> basePtr ){
    basePtr->someOperation();
}

void sharedptr_Derived( std::shared_ptr<DerivedClass> derivedPtr ){
    derivedPtr->someOperation();
}

void sharedptr_nonderived( std::shared_ptr<NonderivedClass> derivedPtr ){
    derivedPtr->someOperation();
}

/**
 * Pass in a reference to the shared pointer
 */
void reference_sharedptr_Base( std::shared_ptr<BaseClass>& basePtr ){
    basePtr->someOperation();
}

void reference_sharedptr_Derived( std::shared_ptr<DerivedClass>& derivedPtr ){
    derivedPtr->someOperation();
}

void reference_sharedptr_nonderived( std::shared_ptr<NonderivedClass>& derivedPtr ){
    derivedPtr->someOperation();
}

/**
 * Pass in the raw pointer
 */
void rawpointer_Base( BaseClass* base ){
    base->someOperation();
}

void rawpointer_Derived( DerivedClass* derive ){
    derive->someOperation();
}

void rawpointer_nonderived( NonderivedClass* derive ){
    derive->someOperation();
}

static void printTimes( const char* function, struct timespec start, struct timespec end ){
    printf("%s took about %.5f seconds\n",
           function,
           ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - 
           ((double)start.tv_sec + 1.0e-9*start.tv_nsec));
}

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
#define RUN_TEST(variable, function) clock_gettime( CLOCK_MONOTONIC, &start );\
    for( int x = 0; x < 40000000; x++ ){ \
        function( variable );\
    }\
    clock_gettime( CLOCK_MONOTONIC, &end );\
    printTimes( STRINGIFY(function), start, end );

/** MAIN **/
int main( int argc, char** argv ){
    std::shared_ptr<DerivedClass> derive( new DerivedClass() );
    std::shared_ptr<NonderivedClass> nonderive( new NonderivedClass() );
    std::shared_ptr<BaseClass> base( new DerivedClass );
    struct timespec start;
    struct timespec end;

    /**************************** Base Tests ************************/
    RUN_TEST( base, sharedptr_Base );

    RUN_TEST( base, reference_sharedptr_Base );

    BaseClass* rawptr_Base = base.get();
    RUN_TEST( rawptr_Base, rawpointer_Base );

    /**************************** Derived Tests ************************/
    RUN_TEST( derive, sharedptr_Derived );

    RUN_TEST( derive, reference_sharedptr_Derived );

    DerivedClass* rawptr_Derived = derive.get();
    RUN_TEST( rawptr_Derived, rawpointer_Derived );

    /**************************** Nonderived Tests ************************/
    RUN_TEST( nonderive, sharedptr_nonderived );

    RUN_TEST( nonderive, reference_sharedptr_nonderived );

    NonderivedClass* rawptr_Nonderived = nonderive.get();
    RUN_TEST( rawptr_Nonderived, rawpointer_nonderived );
}
