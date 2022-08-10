// code guard
#if !defined(mito_mesh_PointCloud_h)
#define mito_mesh_PointCloud_h

// QUESTION: should Point and PointCloud live in the mesh namespace?
namespace mito::mesh {

    template <int D>
    class PointCloud {
      private:
        using cloud_t = std::unordered_map<oriented_simplex_ptr<0>, point_t<D>>;

      public:
        PointCloud() = delete;

      public:
        static void print()
        {
            // iterate on map
            for (auto item : _cloud) {
                std::cout << "Vertex: " << item.first.get() << std::endl;
                std::cout << "Point: " << item.second << std::endl;
            }
            // all done
            return;
        }

        // support for ranged for loops (wrapping grid)
        static inline const auto begin() { return _cloud.cbegin(); }
        static inline const auto end() { return _cloud.cend(); }

        static int size() { return _cloud.size(); }

        static auto insert(const oriented_simplex_ptr<0> & vertex, const point_t<D> & point)
        {
            return _cloud.insert(
                std::pair<oriented_simplex_ptr<0>, const point_t<D>>(vertex, point));
        }

        static auto insert(const oriented_simplex_ptr<0> & vertex, const point_t<D> && point)
        {
            return _cloud.insert(
                std::pair<oriented_simplex_ptr<0>, const point_t<D>>(vertex, point));
        }

        // TODO: accessor operator[](point_t) -> a list of all vertices sitting on the same point
        static const point_t<D> & point(const oriented_simplex_ptr<0> &vertex)
        {
            return _cloud.find(vertex)->second;
        }

      private:
        static cloud_t _cloud;
    };

    // initialize static attribute
    template <int D>
    typename PointCloud<D>::cloud_t PointCloud<D>::_cloud = PointCloud<D>::cloud_t();

}    // namespace mito

#endif    // mito_mesh_PointCloud_h

// end of file
