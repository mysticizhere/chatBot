#include <map>

template <typename K, typename V>
class interval_map
{
    friend void IntervalMapTest();
    V m_valBegin;
    std::map<K, V> m_map;

public:
    // constructor associates whole range of K with val
    interval_map(V const &val)
        : m_valBegin(val)
    {
    }

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign(K const &keyBegin, K const &keyEnd, V const &val)
    {
        if (!(keyBegin < keyEnd))
        {
            return;
        }

        auto it = m_map.lower_bound(keyBegin);
        if (it == m_map.end())
        {
            m_map.insert(m_map.end(), std::make_pair(keyBegin, val));
            m_map.insert(m_map.end(), std::make_pair(keyEnd, m_valBegin));
        }
        else
        {
            std::map<K, bool> keysToDelete;
            if (it != m_map.begin())
            {
                --it;
            }

            // auto toDelete=m_map.end();
            // bool toDel=0;
            while (it != m_map.end() && it->first < keyEnd)
            {
                if (keyBegin < it->first)
                {
                    keysToDelete[it->first];
                    // toDel=1;
                }

                if (!(keyBegin < it->first) && !(it->first < keyBegin))
                { // keyBegin == it->first
                    keysToDelete[it->first];
                    // toDel=1;
                }

                if (++it != m_map.end() && keyEnd < it->first)
                {
                    m_map.insert(--it, std::make_pair(keyEnd, (it)->second));
                    break;
                }
            }

            for (auto [keykeysToDelete, trash] : keysToDelete)
            {
                m_map.erase(keykeysToDelete);
            }

            if (!m_map.empty() && keyEnd < m_map.begin()->first)
            {
                m_map.insert(m_map.begin(), std::make_pair(keyEnd, m_valBegin));
            }

            m_map.insert(--(m_map.find(keyBegin)), std::make_pair(keyBegin, val));
        }

        it = m_map.find(keyBegin);
        std::map<K, bool> keysToDelete;
        auto nkeyBegin = keyBegin;
        while (it != m_map.begin())
        {
            --it;
            if (!(it->second == val))
            {
                break;
            }
            keysToDelete[nkeyBegin];
            nkeyBegin = it->first;
        }
        for (const auto &[key, trash] : keysToDelete)
        {
            m_map.erase(key);
        }

        it = m_map.find(nkeyBegin);
        it++;
        keysToDelete.clear();
        while (it != m_map.end())
        {
            if (!(it->second == val))
            {
                break;
            }
            auto key = it->first;
            keysToDelete[key];
            ++it;
        }
        for (const auto &[key, trash] : keysToDelete)
        {
            m_map.erase(key);
        }

        if (m_map.rbegin()->second != m_valBegin)
        {
            m_map.insert(m_map.end(), std::make_pair(keyEnd, m_valBegin));
        }
        if (m_map.begin()->second == m_valBegin)
        {
            m_map.erase(m_map.begin());
        }
    }

    // look-up of the value associated with key
    V const &operator[](K const &key) const
    {
        auto it = m_map.upper_bound(key);
        if (it == m_map.begin())
        {
            return m_valBegin;
        }
        else
        {
            return (--it)->second;
        }
    }
};

void print(typename std::map<unsigned int, unsigned int>::iterator itBegin, typename std::map<unsigned int, unsigned int>::iterator itEnd)
{

    for (itBegin; itBegin != itEnd; ++itBegin)
    {
        cout << "key = " << itBegin->first << " value = " << itBegin->second << endl;
    }
}

void IntervalMapTest()
{
    interval_map<unsigned int, unsigned int> myMap(10);

    try
    {
        cout << "interval[0, 0) value (10)" << endl;
        print(myMap.m_map.begin(), myMap.m_map.end());

        cout << endl
             << "interval[3, 6) value (20)" << endl;
        myMap.assign(3, 6, 20);
        print(myMap.m_map.begin(), myMap.m_map.end());

        cout << endl
             << "interval[2, 5) value (30)" << endl;
        myMap.assign(2, 5, 30);
        print(myMap.m_map.begin(), myMap.m_map.end());

        cout << endl
             << "interval[5, 7) value (60)" << endl;
        myMap.assign(5, 7, 60);
        print(myMap.m_map.begin(), myMap.m_map.end());

        cout << endl
             << "interval[0, 4) value (10)" << endl;
        myMap.assign(0, 4, 10);
        print(myMap.m_map.begin(), myMap.m_map.end());

        cout << endl
             << "interval[0, 3) value (150)" << endl;
        myMap.assign(0, 3, 150);
        print(myMap.m_map.begin(), myMap.m_map.end());
    }
    catch (int exception)
    {
        if (exception == 1)
            cout << endl
                 << "There is a gap in the specified interval !!!";
    }
}

int main(int argc, char *argv[])
{
    IntervalMapTest();
    return 0;
}