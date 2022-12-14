// code guard
#if !defined(mito_utilities_Singleton_h)
#define mito_utilities_Singleton_h


namespace mito::utilities {

    template <class RESOURCE_T>
    class Singleton {

        // my template parameter
        using resource_t = RESOURCE_T;

      public:
        static auto GetInstance() -> resource_t &
        {
            if (!resource) {
                resource = new resource_t();
                return *resource;
            }

            return *resource;
        }

      private:
        // the singleton
        static resource_t * resource;
    };

    // initialization of static member
    template <class RESOURCE_T>
    RESOURCE_T * mito::utilities::Singleton<RESOURCE_T>::resource = nullptr;

}    // namespace mito

#endif    // mito_utilities_Singleton_h

// end of file
