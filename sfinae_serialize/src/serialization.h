#pragma once

#include <ostream>
#include <istream>
#include <type_traits>
#include <cassert>

using namespace std;

namespace serialization {

    using std::true_type;
    using std::false_type;
    using std::enable_if;
    using std::is_pod;

    typedef uint64_t size_type;

    template<class stream, class type1, class type2>
    void serialize(stream &s, std::pair<type1, type2>& p);

    template<class stream, class type1, class type2>
    void deserialize(stream &s, std::pair<type1, type2>& p);


    // task 1
    template <class T>
    typename enable_if<is_pod<T>::value>::type
    serialize(std::ostream& out, T& obj) {
        out.write(reinterpret_cast<char*>(&obj), sizeof(obj));
    }

    template <class T>
    typename enable_if<is_pod<T>::value>::type
    deserialize(std::istream& in, T &obj) {
        in.read(reinterpret_cast<char*>(&obj), sizeof(obj));
    }

    // task 2 and 3
    template <class T>
    struct has_iterator
    {
        template<class U>
        static true_type test(typename U::iterator *);
        template<class U>
        static std::false_type test(...);

        enum { value = decltype(test<T>(nullptr))::value };
        typedef std::integral_constant<bool, value> type;
    };

    template <class T>
//    typename enable_if<has_iterator<T>::value>::type
    typename std::enable_if<has_iterator<T>::value>::type
    serialize(std::ostream& out, T& container) {
        size_type e_size = container.size();
        serialize(out, e_size);
        for (auto e : container) {
            serialize(out, e);
        }
    }

    template <class T>
    typename enable_if<has_iterator<T>::value>::type
    deserialize(std::istream& in, T& container) {
        size_type size = 0;
        deserialize(in, size);
        container.clear();
        typename T::value_type e;
        for (int i = 0; i < size; ++i) {
            deserialize(in, e);
            container.insert(container.end(), std::move(e));
        }
    }

    // task 4
    template <class stream, class K, class V>
    void serialize(stream& s, std::pair<K, V>& p) {
        typedef typename std::remove_const<K>::type KK;
        typedef typename std::remove_const<V>::type VV;

        serialize(s, const_cast<KK&>(p.first )); // to solve const map key problem
        serialize(s, const_cast<VV&>(p.second)); // just to be symmetric
    };

    template <class stream, class K, class V>
    void deserialize(stream& s, std::pair<K, V>& p) {
        typedef typename std::remove_const<K>::type KK;
        typedef typename std::remove_const<V>::type VV;

        deserialize(s, const_cast<KK&>(p.first )); // to solve const map key problem
        deserialize(s, const_cast<VV&>(p.second)); // just to be symmetric
    };

    // task 5
    template <class T>
    struct is_container_or_pod
    {
        enum { value = has_iterator<T>::value || std::is_pod<T>::value };
        typedef std::integral_constant<bool, value> type;
    };

//    template <class T>
//    void serialize(std::ostream& out, T& container,
//                   typename std::enable_if<!is_container_or_pod<T>::value>::type* = nullptr)
//    {}
//
//    template <class T>
//    void deserialize(std::istream& in, T& container,
//                   typename std::enable_if<!is_container_or_pod<T>::value>::type* = nullptr)
//    {}

////    template <class T>
////    typename enable_if<!is_pod<T>::value>::type
//    void serialize(...) {
//        assert("Type is not serializable");
//    }
//
////    template <class T>
////    typename enable_if<!is_pod<T>::value>::type
//    void deserialize(...) {
//        assert("Type is not serializable");
//    }

} // namespace serialization

///////////////////////////////////////////
///////////////////////////////////////////////
//
//#pragma once
//
//#include <type_traits>
//#include <cstdint>
//#include <ostream>
//#include <istream>
//
//namespace serialization
//{
//    typedef uint64_t size_type;
//    namespace type_traits
//    {
//        template <class T>
//        struct is_container
//        {
//            template<class U>
//            static std::true_type test(typename U::iterator *);
//            template<class U>
//            static std::false_type test(...);
//
//            enum { value = decltype(test<T>(nullptr))::value };
//            typedef std::integral_constant<bool, value> type;
//        };
//
//        template <class T>
//        struct is_container_or_pod
//        {
//            enum { value = is_container<T>::value || std::is_pod<T>::value };
//            typedef std::integral_constant<bool, value> type;
//        };
//    } // type_traits
//
//    //////////////////////////////////////////////////////////////////////////
//    // Forward declarations of all the non read-write specializations
//    template<class stream, class type1, class type2>
//    void serialize(stream &s, std::pair<type1, type2>& p);
//
//    //////////////////////////////////////////////////////////////////////////
//    // readers
//    template<class type>
//    void serialize(std::istream& is, type& obj,
//                   typename std::enable_if<std::is_pod<type>::value>::type* = nullptr)
//    {
//        is.read(reinterpret_cast<char*>(&obj), sizeof(obj));
//    }
//
//    template<class type>
//    void serialize(std::istream& is, type& container,
//                   typename std::enable_if<type_traits::is_container<type>::value>::type* = nullptr)
//    {
//        size_type sz = 0;
//        serialize(is, sz);
//
//        container.clear();
//        for (size_t i = 0; i < sz; ++i)
//        {
//            typename type::value_type obj;
//            serialize(is, obj);
//            container.insert(container.end(), std::move(obj));
//        }
//    }
//
//    //////////////////////////////////////////////////////////////////////////
//    // writers
//    template<class type>
//    void serialize(std::ostream& os, type &obj,
//                   typename std::enable_if<std::is_pod<type>::value>::type* = nullptr)
//    {
//        os.write(reinterpret_cast<char*>(&obj), sizeof(obj));
//    }
//
//    template<class type>
//    void serialize(std::ostream& os, type &container,
//                   typename std::enable_if<type_traits::is_container<type>::value>::type* = nullptr)
//    {
//        size_type size = container.size();
//        serialize(os, size);
//
//        for (auto it = container.begin(); it != container.end(); ++it)
//            serialize(os, *it);
//    }
//
//    //////////////////////////////////////////////////////////////////////////
//    // definitions of all the non read-write specializations
//    template<class stream, class type1, class type2>
//    void serialize(stream &s, std::pair<type1, type2>& p)
//    {
//        // to support non-pod map and unordered_map
//        typedef typename std::remove_const<type1>::type first_type;
//        typedef typename std::remove_const<type2>::type second_type;
//
//        serialize(s, const_cast<first_type&>(p.first )); // to solve const map key problem
//        serialize(s, const_cast<second_type&>(p.second)); // just to be simmetric
//    }
//
//    // helper for user's code readability
//    template<class type>
//    void deserialize(std::istream& is, type& obj)
//    {
//        serialize(is, obj);
//    }
//
//    // We can place here more optimized serialize implementations.
//    // Example: one-shot copy for continuous containers (vector, string)
//} // namespace serialization
