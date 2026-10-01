#include "movie.h"
#include "util.h"
#include <sstream>

using namespace std;

Movie::Movie(string category, string name, double price, int qty, string genre, string rating)
    : Product(category, name, price, qty), genre_(genre), rating_(rating)
{
}

Movie::~Movie()
{
}

set<string> Movie::keywords() const
{
    set<string> result = parseStringToWords(getName());

    result.insert(convToLower(genre_));

    return result;
}

string Movie::displayString() const
{
    stringstream ss;

    ss << getName() << "\n";
    ss << "Genre: " << genre_ << " Rating: " << rating_ << "\n";
    ss << getPrice() << " " << getQty() << " left.";

    return ss.str();
}

void Movie::dump(ostream& os) const
{
    Product::dump(os);
    os << genre_ << "\n";
    os << rating_ << "\n";
}