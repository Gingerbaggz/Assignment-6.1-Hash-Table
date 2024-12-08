#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>

template <typename KeyType, typename ValueType>
class HashTable {
private:
    std::vector<std::pair<KeyType, ValueType>> table;
    std::vector<bool> occupied;
    size_t current_size;
    size_t capacity;

    size_t hash(const KeyType& key) const {
        // Simple hash function (modulo of prime number for better distribution)
        return std::hash<KeyType>{}(key) % capacity;
    }

public:
    HashTable(size_t cap = 100) : capacity(cap), current_size(0) {
        table.resize(capacity);
        occupied.resize(capacity, false);
    }

    bool insert(const KeyType& key, const ValueType& value) {
        if (current_size == capacity) {
            std::cerr << "Hash table is full!\n";
            return false;  // Cannot insert if the table is full
        }

        size_t index = hash(key);
        size_t original_index = index;

        while (occupied[index]) {
            if (table[index].first == key) {
                // Key already exists, update the value
                table[index].second = value;
                return true;
            }
            index = (index + 1) % capacity;  // Linear probing
            if (index == original_index) {
                return false;  // Avoid infinite loop
            }
        }

        // Insert the key-value pair
        table[index] = { key, value };
        occupied[index] = true;
        ++current_size;
        return true;
    }

    bool retrieve(const KeyType& key, ValueType& value) const {
        size_t index = hash(key);
        size_t original_index = index;

        while (occupied[index]) {
            if (table[index].first == key) {
                value = table[index].second;
                return true;
            }
            index = (index + 1) % capacity;  // Linear probing
            if (index == original_index) {
                return false;  // Not found
            }
        }
        return false;  // Not found
    }

    bool remove(const KeyType& key) {
        size_t index = hash(key);
        size_t original_index = index;

        while (occupied[index]) {
            if (table[index].first == key) {
                occupied[index] = false;
                --current_size;
                return true;
            }
            index = (index + 1) % capacity;  // Linear probing
            if (index == original_index) {
                return false;  // Not found
            }
        }
        return false;  // Not found
    }

    size_t size() const {
        return current_size;
    }

    bool is_empty() const {
        return current_size == 0;
    }
};
void performance_test(HashTable<int, int>& ht, int num_operations) {
    using namespace std::chrono;

    // Random number generator for test
    std::srand(static_cast<unsigned int>(std::time(0)));

	//Measure time for each operation

    // Insert
    auto start = high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        ht.insert(std::rand(), std::rand());
    }
    auto end = high_resolution_clock::now();
    auto duration_insert = duration_cast<microseconds>(end - start);
    std::cout << "Insert " << num_operations << " operations took " << duration_insert.count() << " microseconds.\n";

    // Retrieve
    start = high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        int value;
        ht.retrieve(std::rand(), value);
    }
    end = high_resolution_clock::now();
    auto duration_retrieve = duration_cast<microseconds>(end - start);
    std::cout << "Retrieve " << num_operations << " operations took " << duration_retrieve.count() << " microseconds.\n";

    // Remove
    start = high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        ht.remove(std::rand());
    }
    end = high_resolution_clock::now();
    auto duration_remove = duration_cast<microseconds>(end - start);
    std::cout << "Remove " << num_operations << " operations took " << duration_remove.count() << " microseconds.\n";
}

int main() {
    // Test different op sizes
    for (int n : {100, 1000, 10000}) {
        std::cout << "Testing with " << n << " operations:\n";
        HashTable<int, int> ht(20000); // Initializing with a large enough capacity
        performance_test(ht, n);
    }
    return 0;
}
