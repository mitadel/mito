// code guard
#if !defined(mito_utilities_Singleton_h)
#define mito_utilities_Singleton_h


namespace mito::utilities {

    template <class RESOURCE_T>
    class Singleton {

        using resource_t = RESOURCE_T;

      public:
        template <class... Args>
        static auto GetInstance(Args &&... args) -> resource_t &
        {
            static resource_t resource(std::forward<Args>(args)...);
            return resource;
        }

        // delete metamethods
        Singleton() = delete;
        ~Singleton() = delete;
        Singleton(const Singleton &) = delete;
        Singleton & operator=(const Singleton &) = delete;
    };

}    // namespace mito

#endif    // mito_utilities_Singleton_h

// end of file
