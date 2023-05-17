// code guard
#if !defined(mito_utilities_Singleton_h)
#define mito_utilities_Singleton_h


namespace mito::utilities {

    // template <class RESOURCE_T>
    // class Singleton {

    //     // my template parameter
    //     using resource_t = RESOURCE_T;

    //   public:
    //     template <class... Args>
    //     static auto GetInstance(Args &&... args) -> resource_t &
    //     {
    //         if (!resource) {
    //             resource = new resource_t(args...);
    //             return *resource;
    //         }

    //         return *resource;
    //     }

    //     static auto DeleteInstance() -> void
    //     {
    //         delete resource;
    //         resource = nullptr;
    //     }

    //   protected:
    //     Singleton()
    //     {
    //         std::cout << "Singleton constructor" << std::endl;
    //         return;
    //     }
    //     ~Singleton() { std::cout << "Singleton destructor" << std::endl; }
    //     Singleton(const Singleton &) = delete;
    //     Singleton & operator=(const Singleton &) = delete;

    //   private:
    //     // the singleton
    //     static resource_t * resource;
    // };

    // // initialization of static member
    // template <class RESOURCE_T>
    // RESOURCE_T * utilities::Singleton<RESOURCE_T>::resource = nullptr;

    template <class RESOURCE_T>
    class Singleton {

        using resource_t = RESOURCE_T;

      public:
        template <class... Args>
        static auto GetInstance(Args &&... args) -> resource_t &
        {
            if (!resource) {
                resource = std::unique_ptr<resource_t>(new resource_t(args...));
            }

            return *resource;
        }

        static auto DestroyInstance() -> void { resource.reset(); }

      protected:
        Singleton()
        {
            std::cout << "Singleton constructor" << std::endl;
            return;
        }
        virtual ~Singleton() { std::cout << "Singleton destructor" << std::endl; }

        Singleton(const Singleton &) = delete;
        Singleton & operator=(const Singleton &) = delete;

      private:
        static std::unique_ptr<resource_t> resource;
    };

    template <class RESOURCE_T>
    std::unique_ptr<RESOURCE_T> utilities::Singleton<RESOURCE_T>::resource = nullptr;

}    // namespace mito

#endif    // mito_utilities_Singleton_h

// end of file
