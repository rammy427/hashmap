// Implementation of a hash table with singly linked lists
// using the "separate chaining" method for hash collisions.
// Hash table will map from single characters to strings.
// References:
// https://aozturk.medium.com/simple-hash-map-hash-table-implementation-in-c-931965904250

#include <iostream>
#include <string>

const int TABLE_SIZE = 20;

// Simple hash function.
int hash_function(char key)
{
    return static_cast<int>(key) % TABLE_SIZE;
}

// Class for hash node (as linked list).
class Node
{
public:
    Node(char key, const std::string& val)
        :
        key(key),
        val(val),
        next(nullptr)
    {
    }
    void setVal(const std::string& val)
    {
        this->val = val;
    }
    void setNext(Node* next)
    {
        this->next = next;
    }
    char getKey() const
    {
        return key;
    }
    std::string getVal() const
    {
        return val;
    }
    Node* getNext() const
    {
        return next;
    }
private:
    char key;
    std::string val;
    Node* next;
};

class HashMap
{
public:
    HashMap()
    {
        buckets = new Node*[TABLE_SIZE]();
    }
    ~HashMap()
    {
        // Destroy all the buckets one by one.
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            // Clear the list from front to back.
            Node* cur = buckets[i];
            while (cur != nullptr)
            {
                Node* prev = cur;
                cur = cur->getNext();
                delete prev;
            }
            buckets[i] = nullptr;
        }
        delete[] buckets;
    }
    void insert(char key, const std::string& val)
    {
        // Hash the key and get array index.
        int hash_index = hash_function(key);
        // Create two pointers to iterate the bucket corresponding to the key.
        Node* prev = nullptr;
        Node* cur = buckets[hash_index];

        // Elements with the same key will be stored consecutively.
        // Iterate through the list until we reach an appropriate spot
        // for the new element.
        // (We stop iterating if we reach the end of the bucket or if
        // we reach the same key we're inserting.)
        while (cur != nullptr && cur->getKey() != key)
        {
            prev = cur;
            cur = cur->getNext();
        }

        if (cur == nullptr)
        {
            // Key does not exist already. Insert it.
            if (prev == nullptr)
                // This means this is the first element in this bucket.
                // So we update the head of the list.
                buckets[hash_index] = new Node(key, val);
            else
                prev->setNext(new Node(key, val));
            
            std::cout << "Insertion successful.\n";
        }
        else // Key already exists. Simply update the value.
        {
            std::cout << "Key already exists. Value updated.\n";
            cur->setVal(val);
        }
    }
    std::string get(char key) const
    {
        // Hash key and get array index.
        int hash_index = hash_function(key);
        Node* cur = buckets[hash_index];

        // Search through the bucket.
        while (cur != nullptr)
        {
            // If we find the corresponding key, return its mapped value.
            if (cur->getKey() == key)
                return cur->getVal();
            // Otherwise, continue.
            cur = cur->getNext();
        }

        // If the item was not found, print a message
        // and return a default-constructed value.
        std::cout << "Item not found. Returning empty string.";
        return "";
    }
    void remove(char key)
    {
        // Hash key and get array index.
        int hash_index = hash_function(key);
        Node* prev = nullptr;
        Node* kod = buckets[hash_index];

        // Iterate through the entire bucket until
        // we reach the end or find the desired key.
        while (kod != nullptr && kod->getKey() != key)
        {
            prev = kod;
            kod = kod->getNext();
        }

        if (kod == nullptr)
        {
            // Key was not found.
            std::cout << "Item not found. No deletion occurred.\n";
            return;
        }
        else
        {
            if (prev == nullptr)
                // Node to delete is the first node. Adjust head.
                buckets[hash_index] = kod->getNext();
            else
                prev->setNext(kod->getNext());
            
            delete kod;

            std::cout << "Deletion successful.\n";
        }
    }
private:
    // Array of "heads" (linked lists). Double pointer.
    Node** buckets;
};

int main()
{
    HashMap map;

    // Insert some pre-determined values.
    map.insert('R', "Rom");
    map.insert('L', "Lenny");
    map.insert('R', "Rammy overwrote Rom");
    map.insert('r', "Case-sensitive!");
    map.insert('D', "The derivative of the integral.");
    map.insert('X', "This should cause a hash collision with D.");
    map.insert('X', "But we may never know for sure. ;)");
    map.insert('G', "Gaby");

    // Let the user insert, get and remove at will.
    int option = 0;
    while (option != 4)
    {
        std::cout << "\nOPTIONS:\n1. Insert\n2. Get\n3. Remove\n4. Quit\n";
        std::cout << "Enter option: ";
        std::cin >> option;

        char key;
        std::string val;

        switch (option)
        {
        case 1:
            std::cout << "Enter key: ";
            std::cin >> key;
            std::cout << "Enter value: ";
            std::cin.ignore();
            std::getline(std::cin, val);
            map.insert(key, val);
            break;
        case 2:
            std::cout << "Enter key: ";
            std::cin >> key;
            std::cout << "Retrieved value: " << map.get(key) << "\n";
            break;
        case 3:
            std::cout << "Enter key: ";
            std::cin >> key;
            map.remove(key);
            break;
        }
    }

    return 0;
}