#ifndef IMASIENGINE_CACHED_HPP
#define IMASIENGINE_CACHED_HPP

namespace ImasiEngine
{
    template<typename T>
    class Cache
    {
    private:

        T _value;
        bool _isValid;

    public:

        template<
            typename ...ConstructorArgs,
            typename = typename std::enable_if<
                std::is_constructible<T, ConstructorArgs...>::value
            >::type
        >
        explicit Cache(ConstructorArgs ...args)
            : _value(args...)
            , _isValid(true)
        {
        }

        template<
            typename = typename std::enable_if<
                std::is_copy_constructible<T>::value
            >::type
        >
        explicit Cache(const Cache<T>& cached)
            : _value(cached._value)
            , _isValid(cached._isValid)
        {
        }

        template<
            typename = typename std::enable_if<
                std::is_move_constructible<T>::value
            >::type
        >
        explicit Cache(Cache<T>&& cached)
            : _value(std::move(cached._value))
            , _isValid(cached._isValid)
        {
        }

        template<
            typename AssignArg,
            typename = typename std::enable_if<
                std::is_assignable<T, AssignArg>::value
            >::type
        >
        Cache<T>& operator=(AssignArg arg)
        {
            _value = arg;
            return *this;
        }

        operator T&()
        {
            return _value;
        }

        T& value()
        {
            return _value;
        }

        void setCache(const T& value)
        {
            _value = value;
        }

        void validateCache()
        {
            _isValid = true;
        }

        void invalidateCache()
        {
            _isValid = false;
        }

        bool hasValidCache() const
        {
            return _isValid;
        }

        bool hasInvalidCache() const
        {
            return !_isValid;
        }
    };
}

#endif
