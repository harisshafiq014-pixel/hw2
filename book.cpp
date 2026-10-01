#include "book.h"
#include "util.h"
#include <sstream>

using namespace std;

Book::Book(string category, string name, double price, int qty, string isbn, string& author)
    : Product(category, name, price, qty),
      isbn_(isbn),
      author_(author)
{

}

Book::~Book()
{

}

set<string> Book::keywords() const
{
    set<string> name_Word = parseStringToWords(getName());
    set<string> author_Word = parseStringToWords(author_);

    set<string> result = setUnion(name_Word, author_Word);

    result.insert(convToLower(isbn_));

    return result;
}

string Book::displayString() const
{
    stringstream ss;

    ss << getName() << "\n";
    ss << "Author: " << author_ << " ISBN: " << isbn_ << "\n";
    ss << getPrice() << " " << getQty() << " left.";

    return ss.str();
}

void Book::dump(ostream& os) const
{
    Product::dump(os);
    os << isbn_ << "\n";
    os << author_ << "\n";
}