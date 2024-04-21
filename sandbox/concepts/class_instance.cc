#include <iostream>

class myHappyClass {
  public:
    // default constructor
    myHappyClass() = default;
    // default copy constructor
    myHappyClass(const myHappyClass &) = delete;
};

// binds to nonconst instances, nonconst references and temporaries
template <class F>
concept happy_class_c = requires(F c) {
    [](myHappyClass &) {
    }(c);
};

// // binds to const and nonconst instances and references and temporaries
// template <class F>
// concept happy_class_c = requires(F c) {
//     [](const myHappyClass &) {
//     }(c);
// };

// // binds to const and nonconst instances and references and temporaries
// // (but only if the copy constructor is available)
// template <class F>
// concept happy_class_c = requires(F c) {
//     [](myHappyClass) {
//     }(c);
// };

int
main()
{
    // report
    std::cout << happy_class_c<myHappyClass> << std::endl;
    std::cout << happy_class_c<myHappyClass &> << std::endl;
    std::cout << happy_class_c<const myHappyClass> << std::endl;
    std::cout << happy_class_c<const myHappyClass &> << std::endl;
    std::cout << happy_class_c<myHappyClass &&> << std::endl;

    // all done
    return 0;
}