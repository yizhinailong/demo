import std;

namespace n716 {
    template<typename T>
    struct vector
    {
        vector(std::size_t const n) : data_(n) {}

        vector(std::initializer_list<T>&& l) : data_(l) {}

        std::size_t size() const noexcept
        { 
            return data_.size();
        }

        T const & operator[](const std::size_t i) const
        {
            return data_[i];
        }

        T& operator[](const std::size_t i)
        {
            return data_[i];
        }

    private:
        std::vector<T> data_;
    };

    template<typename T, typename U>
    auto operator+ (vector<T> const & a, vector<U> const & b)
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());
        vector<result_type> result(a.size());
        for (std::size_t i = 0; i < a.size(); ++i)
        {
            result[i] = a[i] + b[i];
        }
        return result;
    }

    template<typename T, typename U>
    auto operator* (vector<T> const & a, vector<U> const & b)
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());
        vector<result_type> result(a.size());
        for (std::size_t i = 0; i < a.size(); ++i)
        {
            result[i] = a[i] * b[i];
        }
        return result;
    }

    template<typename T, typename S>
    auto operator* (S const& s, vector<T> const& v)
    {
        using result_type = decltype(std::declval<T>() + std::declval<S>());

        vector<result_type> result(v.size());
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            result[i] = s * v[i];
        }

        return result;
    }
} // namespace n716

auto main() -> int {
    {
        n716::vector<int> v1{ 1,2,3 };
        n716::vector<int> v2{ 4,5,6 };
        double a{ 1.5 };
        
        n716::vector<double> v3 = v1 + a * v2;    // {7.0, 9.5, 12.0}
        n716::vector<int> v4 = v1 * v2 + v1 + v2; // {9, 17, 27}
    }
    return 0;
}
