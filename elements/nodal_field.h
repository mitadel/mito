#include "../mito.h"

namespace mito {

template <typename T>
class NodalField {
  public:

    NodalField(size_t nodes, size_t dim, std::string name = "") 
        : _nodes(nodes), _dim(dim), _name(name), _nodalField(_nodes * _dim, 0.0) {}

    ~NodalField(){}

    /**
     * Operator()
     */
    const T& operator()(const size_t a, const size_t i) const {
        return _nodalField[_dim * a + i];
    }

    T& operator()(const size_t a, const size_t i) {
        return _nodalField[_dim * a + i];
    }

    /**
     * Iterators
     */
    typename std::vector<T>::iterator begin() {
        return _nodalField.begin();
    }

    typename std::vector<T>::iterator end() {
        return _nodalField.end();
    }

    typename std::vector<T>::const_iterator begin() const {
        return _nodalField.begin();
    }

    typename std::vector<T>::const_iterator end() const {
        return _nodalField.end();
    }

    /**
     * Accessors
     */
    inline const std::string& name() const {
        return _name;
    }

    inline int dim() const {
        return _dim;
    }

    inline int nodes() const {
        return _nodes;
    }

    inline size_t size() const {
        return _nodes * _dim;
    }

    /**
     * Set the field to zero.
     */
    inline void init() {
        std::fill(_nodalField.begin(), _nodalField.end(), 0.0);
    }

  private:
    /**
     * number of nodes
     */
    size_t _nodes;

    /**
     * dimension
     */
    size_t _dim;

    /**
     * name of the nodal field
     */
    std::string _name;

    /**
     * nodal field
     */
    std::vector<T> _nodalField;

};

template<typename T>
std::ostream& operator<<(std::ostream& os, const NodalField<T>& nodalField){
    
    os << "Nodal field \"" << nodalField.name() << "\" : ";

    if (nodalField.size() == 0) {
        os << "[]";
        return os;
    }

    os << "[(" << nodalField(0, 0); 
    for (size_t d = 1; d < nodalField.dim(); ++d) {
        os << ", " << nodalField(0, d);
    }
    os << ")";

    for (size_t i = 1; i < nodalField.nodes(); ++i)
    {
        os << ", (" << nodalField(i, 0);
        for (size_t d = 1; d < nodalField.dim(); ++d) {
            os << ", " << nodalField(i, d);
        }
        os << ")";
    }

    os << "]";

    return os;
}

}  // namespace mito

// end of file
