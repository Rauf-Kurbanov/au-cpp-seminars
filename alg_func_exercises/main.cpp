#include <iostream>
#include <functional>
#include <algorithm>
#include <cassert>
#include <queue>

template<typename T, size_t n>
size_t ARRAY_SIZE(const T (&)[n])
{
    return n;
}

using namespace std;

template <class Iterator>
using ivt = typename std::iterator_traits<Iterator>::value_type;

template <class IterT, class CompT = std::less<ivt<IterT>>>
int elem_num_in_sorted(IterT begin, IterT end, ivt<IterT> val, CompT comp = CompT()) {
    auto range = equal_range(begin, end, val);
    return distance(range.first, range.second);
}

template <class Cont>
using ContIt = typename Cont::iterator;

template <class Cont>
using ContVt = typename Cont::value_type;

template <class Cont>
ContIt<Cont> set_add(Cont &sorted_cont, const ContVt<Cont> val)
{
    if (binary_search(sorted_cont.begin(), sorted_cont.end(), val))
        return sorted_cont.end();
    else
        return sorted_cont.insert(lower_bound(sorted_cont.begin(), sorted_cont.end(), val), val);
}


template <class C, class P>
void erase_if(C &cont, P pred)
{
    auto it = remove_if(cont.begin(), cont.end(), pred);
    cont.erase(it, cont.end());
}

template <class IterT>
void merge_sort(IterT begin, IterT end)
{
    if (distance(begin, end) == 1)
        return;
    IterT mid = begin;
    std::advance(mid, std::distance(begin, end) / 2);
    merge_sort(begin, mid);
    merge_sort(mid , end);
    inplace_merge(begin, mid, end);
}

template <class IterT>
void heap_sort(IterT begin, IterT end)
{
    make_heap(begin, end);
    size_t cnt = distance(begin, end);
    for (cnt; cnt > 0; --cnt, --end)
        pop_heap(begin, end);
}

template <class Cont>
void print(Cont cont)
{
    for (auto x : cont)
    {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    // task 1
    int sorted_arr[] = {1, 2, 3, 4, 5, 6, 7, 7, 7, 8, 9, 10, 11};
    assert(elem_num_in_sorted(sorted_arr, sorted_arr + ARRAY_SIZE(sorted_arr), 7) == 3);

    // task 2
    vector<int> set;
    assert(*set_add(set, 10) == 10);
    assert(set_add(set, 10) == set.end());
    assert(*set_add(set, 5) == 5);
    assert((set[0] == 5) && (set[1] == 10));

    // task 3
    vector<int> set3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    erase_if(set3, [](int val) { return val > 6; });
    for (auto x : set3)
        cout << x << " ";
    cout << endl;

    // task 4
    std::vector<int> vec = {
            10, 9, 4, 345, 54, 2, 4, 12
            , 4, 2, 34, 34, 56, 4, 67
            , 3465, 3, 4, 5, 34, 5
    };
    print(vec);
    merge_sort(vec.begin(), vec.end());
    print(vec);

    // task 5
    vec = {
            10, 9, 4, 345, 54, 2, 4, 12
            , 4, 2, 34, 34, 56, 4, 67
            , 3465, 3, 4, 5, 34, 5
    };
    print(vec);
    heap_sort(vec.begin(), vec.end());
    print(vec);


    return 0;
}