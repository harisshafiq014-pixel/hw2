#include "clothing.h"
#include "util.h"
#include <sstream>

using namespace std;

Clothing::Clothing(string category, string name, double price, int qty, string size, string& brand)
    : Product(category, name, price, qty), size_(size), brand_(brand)
{
}

Clothing::~Clothing()
{
}

set<string> Clothing::keywords() const
{
    set<string> name_Word = parseStringToWords(getName());
    set<string> brand_Word = parseStringToWords(brand_);

    return setUnion(name_Word, brand_Word);
}

string Clothing::displayString() const
{
    stringstream ss;

    ss << getName() << "\n";
    ss << "Size: " << size_ << " Brand: " << brand_ << "\n";
    ss << getPrice() << " " << getQty() << " left.";

    return ss.str();
}

void Clothing::dump(ostream& os) const
{
    Product::dump(os);
    os << size_ << "\n";
    os << brand_ << "\n";
}