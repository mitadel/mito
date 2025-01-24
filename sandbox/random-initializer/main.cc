// -*- C++ -*-
//
// bianca giovanardi
// (c) 1998-2024 all rights reserved
//


#include <random>
#include <array>

namespace pyre::tensor {

    // concept for a subscriptable container
    template <class F>
    concept subscriptable_c = requires(F a, size_t i) {
        { a[i] };
    };

    // a random initializer for a generic N-dimensional real-valued data structure supporting the
    // subscript operator
    template <int N, class distributionT, std::uniform_random_bit_generator generatorT>
    class RandomInitializer {
        using real_type = double;
        using random_distribution_t = distributionT;
        using random_generator_t = generatorT;

      public:
        // constructor from a random distribution and a random generator
        RandomInitializer(
            const random_distribution_t & distribution, const random_generator_t & generator) :
            _distribution(distribution),
            _generator(generator),
            _numbers()
        {
            // start with a fresh batch of random numbers
            generate_new_batch();
        }

      public:
        // generate a new batch of N random numbers
        auto generate_new_batch() -> void
        {
            // generate a new batch of random numbers
            for (int i = 0; i < N; ++i) {
                _numbers[i] = _distribution(_generator);
            }

            // all done
            return;
        }

        // initialize the container with the current batch of {N} real values
        auto initialize(subscriptable_c auto & container) -> void
        {
            // fill the container with the current batch of random numbers
            for (int i = 0; i < N; ++i) {
                container[i] = _numbers[i];
            }

            // all done
            return;
        }

      private:
        // the random distribution
        random_distribution_t _distribution;
        // the random generator
        random_generator_t _generator;
        // the latest batch of N random numbers
        std::array<double, N> _numbers;
    };

    // alias for random initializer
    template <int N, class distributionT, std::uniform_random_bit_generator generatorT>
    using random_initializer_t = RandomInitializer<N, distributionT, generatorT>;

    // factory for a random initializer
    template <int N, class distributionT, std::uniform_random_bit_generator generatorT>
    constexpr auto random_initializer(
        const distributionT & distribution, const generatorT generator)
        -> random_initializer_t<N, distributionT, generatorT>
    {
        return random_initializer_t<N, distributionT, generatorT>(distribution, generator);
    }

}    // namespace pyre::tensor

// random number distribution
std::uniform_real_distribution<> dis(-1.0, 1.0);
// random number generator
std::mt19937 gen;

int
main()
{
    // a random initializer for 9-number batches
    auto random_initializer = pyre::tensor::random_initializer<9>(dis, gen);

    // generate a new batch of random numbers
    random_initializer.generate_new_batch();

    // array
    std::array<double, 9> tensor1_c;
    random_initializer.initialize(tensor1_c);

    // // tensor
    // pyre::tensor::matrix_t<3> tensor1;
    // random_initializer.initialize(tensor1);

    // generate a new batch of random numbers
    random_initializer.generate_new_batch();

    // array
    std::array<double, 9> tensor2_c;
    random_initializer.initialize(tensor2_c);

    // // tensor
    // pyre::tensor::matrix_t<3> tensor2;
    // random_initializer.initialize(tensor2);

    return 0;
}