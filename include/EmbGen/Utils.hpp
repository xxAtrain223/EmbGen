#ifndef EMBGEN_UTILS_HPP
#define EMBGEN_UTILS_HPP

#include <algorithm>
#include <vector>
#include <map>

namespace emb
{
    namespace gen
    {
        template<typename K, typename V>
        K selectFirst(std::pair<K, V> pair)
        {
            return pair.first;
        }

        template<typename K, typename V>
        V selectSecond(std::pair<K, V> pair)
        {
            return pair.second;
        }

        template<typename K, typename V>
        std::vector<K> getMapKeys(std::map<K, V> map)
        {
            std::vector<K> keys;
            std::transform(map.begin(), map.end(), std::back_inserter(keys), selectFirst<K, V>);
            return keys;
        }
    }
}

#endif // EMBGEN_UTILS_HPP