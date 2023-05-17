// code guard
#if !defined(mito_utilities_Singleton_h)
#define mito_utilities_Singleton_h


namespace mito::utilities {

    template <class resource_t>
    class Singleton {

      public:
        template <class... Args>
        static resource_t & GetInstance(Args &&... args)
        {
            static resource_t instance(args...);
            return instance;
        }

      protected:
        Singleton()
        {
            std::cout << "Singleton constructor" << std::endl;
            return;
        }
        ~Singleton() { std::cout << "Singleton destructor" << std::endl; }

        Singleton(const Singleton &) = delete;
        Singleton & operator=(const Singleton &) = delete;
    };

}    // namespace mito

#endif    // mito_utilities_Singleton_h

// end of file
