#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

using namespace std;

//Exeption classes
class DuplicateItemException : public runtime_error {
public:
    DuplicateItemException(const string& msg) : runtime_error(msg) {}
};

class ItemNotFoundException : public runtime_error {
public:
    ItemNotFoundException(const string& msg) : runtime_error(msg) {}
};

//stored item class
class StoredItem {
private:
    string id;
    string description;
    string location;

public:
    StoredItem(string id, string desc, string loc)
        : id(id), description(desc), location(loc) {}

    string getId() const { return id; }
    string getDescription() const { return description; }
    string getLocation() const { return location; }
};

//storage manager class
class StorageManager {
private:
    unordered_map<string, shared_ptr<StoredItem>> itemById;
    map<string, shared_ptr<StoredItem>> itemByDescription;

public:
    void addItem(const shared_ptr<StoredItem>& item) 
    {
        // Add item to both maps, throw if ID already exists
        //check if item already exists
        if (itemById.count(item->getId()))
        {
            throw DuplicateItemException("Item with ID " + item->getId() + " already exists!");
        }

        //ID and description of item in maps
        itemById[item->getId()] = item;
        itemByDescription[item->getDescription()] = item;
    }

    shared_ptr<StoredItem> findById(const string& id) const
    {
        auto it = itemById.find(id);
        //if no item with id is found, throw exception
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        return it->second;
    }

    void removeItem(const string& id)
    {
        auto it = itemById.find(id);
        //if no item with id is found, throw exception
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        string desc = it->second->getDescription();
        itemById.erase(it);
        itemByDescription.erase(desc);
    }

    void listItemsByDescription() const
    {
        //Iterate over itemByDescription and print info
        cout << "\nItems in Description Order: " << endl;
        
        for (const auto& pair : itemByDescription)
        {
            cout << "- " << pair.second->getDescription() << ": " << pair.second->getLocation() << "\n";
        }
        cout << endl;
    }
};

//void testDuplicateAddition();
//void testItemNotFound();
//These functions are effectively acheived by the user menu
void userMenu();

int main()
{
    StorageManager manager;
    int choice;

    do 
    {
        userMenu(); //display the input menu
        cin >> choice;
        cin.ignore();

        switch (choice) 
        {
        case 1: //add an item
        {
            string id, description, location;
            cout << "Enter item ID: ";
            getline(cin, id);
            cout << "Enter item description: ";
            getline(cin, description);
            cout << "Enter item location: ";
            getline(cin, location);

            auto item = make_shared<StoredItem>(id, description, location);
            try {
                manager.addItem(item);
                cout << "Item added successfully." << endl;
            }
            catch (const DuplicateItemException& e) {
                cerr << "Error: " << e.what() << endl;
            }
            break;
        }

        case 2: //finding an item
        {
            string id;
            cout << "Enter item ID to find: ";
            getline(cin, id);
            try 
            {
                auto found = manager.findById(id);
                cout << "Found: " << found->getDescription() << " at " << found->getLocation() << endl;
            }
            catch (const ItemNotFoundException& e) 
            {
                cerr << "Error: " << e.what() << endl;
            }
            break;
        }

        case 3: //remove an item
        {
            string id;
            cout << "Enter item ID to remove: ";
            getline(cin, id);
            try 
            {
                manager.removeItem(id);
                cout << "Item removed successfully." << endl;
            }
            catch (const ItemNotFoundException& e) 
            {
                cerr << "Error: " << e.what() << endl;
            }
            break;
        }

        case 4: //list items in order bt description
        {
            manager.listItemsByDescription();
            break;
        }

        case 5:
        {
            cout << "Thanks for using the storage management system" << endl;
            break;
        }

        default:
            cout << "Invalid choice." << endl;
        }

    } while (choice != 5);

    return 0;
}

void userMenu()
{
    cout << "\nUser Menu for Congo Storage Management" << endl;
    cout << "1. Add Item" << endl;
    cout << "2. Find Item by ID" << endl;
    cout << "3. Remove Item" << endl;
    cout << "4. List Items by Description" << endl;
    cout << "5. Exit Program" << endl;
    cout << "\nEnter choice: ";
}