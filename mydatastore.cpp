#include "mydatastore.h"
#include "util.h"
#include <iostream>

using namespace std;

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for (vector<Product*>::iterator it = products_.begin();
         it != products_.end(); ++it) {
        delete *it;
    }

    for (map<string, User*>::iterator it = users_.begin();
         it != users_.end(); ++it) {
        delete it->second;
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.push_back(p);

    set<string> productKeywords = p->keywords();

    for (set<string>::iterator it = productKeywords.begin();
         it != productKeywords.end(); ++it) {
        string keyword = convToLower(*it);
        keywordIndex_[keyword].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    string username = convToLower(u->getName());

    users_[username] = u;
    carts_[username] = vector<Product*>();
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
    vector<Product*> hits;

    if (terms.size() == 0) {
        return hits;
    }

    set<Product*> results;

    for (size_t i = 0; i < terms.size(); i++) {
        string term = convToLower(terms[i]);
        set<Product*> currentMatches;

        map<string, set<Product*> >::iterator found =
            keywordIndex_.find(term);

        if (found != keywordIndex_.end()) {
            currentMatches = found->second;
        }

        if (i == 0) {
            results = currentMatches;
        }
        else if (type == 0) {
            results = setIntersection(results, currentMatches);
        }
        else {
            results = setUnion(results, currentMatches);
        }
    }

    for (set<Product*>::iterator it = results.begin();
         it != results.end(); ++it) {
        hits.push_back(*it);
    }

    return hits;
}

bool MyDataStore::addToCart(const string& username, Product* product)
{
    string lowercaseUsername = convToLower(username);

    map<string, User*>::iterator user =
        users_.find(lowercaseUsername);

    if (user == users_.end() || product == NULL) {
        return false;
    }

    carts_[lowercaseUsername].push_back(product);
    return true;
}

bool MyDataStore::viewCart(const string& username) const
{
    string lowercaseUsername = convToLower(username);

    map<string, User*>::const_iterator user =
        users_.find(lowercaseUsername);

    if (user == users_.end()) {
        return false;
    }

    map<string, vector<Product*> >::const_iterator cart =
        carts_.find(lowercaseUsername);

    if (cart == carts_.end()) {
        return true;
    }

    for (size_t i = 0; i < cart->second.size(); i++) {
        cout << "Item " << i + 1 << endl;
        cout << cart->second[i]->displayString() << endl;
    }

    return true;
}

bool MyDataStore::buyCart(const string& username)
{
    string lowercaseUsername = convToLower(username);

    map<string, User*>::iterator user =
        users_.find(lowercaseUsername);

    if (user == users_.end()) {
        return false;
    }

    vector<Product*>& cart = carts_[lowercaseUsername];
    vector<Product*> remainingProducts;

    for (vector<Product*>::iterator it = cart.begin();
         it != cart.end(); ++it) {
        Product* product = *it;

        if (product->getQty() > 0 &&
            user->second->getBalance() >= product->getPrice()) {
            product->subtractQty(1);
            user->second->deductAmount(product->getPrice());
        }
        else {
            remainingProducts.push_back(product);
        }
    }

    cart = remainingProducts;
    return true;
}

void MyDataStore::dump(ostream& ofile)
{
    ofile << "<products>" << endl;

    for (vector<Product*>::iterator it = products_.begin();
         it != products_.end(); ++it) {
        (*it)->dump(ofile);
    }

    ofile << "</products>" << endl;
    ofile << "<users>" << endl;

    for (map<string, User*>::iterator it = users_.begin();
         it != users_.end(); ++it) {
        it->second->dump(ofile);
    }

    ofile << "</users>" << endl;
}