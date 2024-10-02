# Online Shopping Cart Project

## Introduction
The Online Shopping Cart project is a software implementation of a typical e-commerce system, where customers can add products to their cart and apply additional services such as gift wrapping and express delivery. 

## Problem Statement
The objective of the project is to build a flexible shopping cart system where different kinds of products (e.g., books, electronics) can be purchased and additional features like Gift Wrapping and Express Delivery can be applied.

## Solution Design
The project is designed around the Decorator Pattern to add features like gift wrapping and express delivery in a clean and modular way. The core objects, such as Books and Electronics, implement common interfaces but can be extended using decorators that add features like GiftWrapperDecorator and ExpressDeliveryDecorator.

## UML Design

![Screenshot 2024-10-02 151248](https://github.com/user-attachments/assets/659591dd-bc0c-4ad1-9b92-344a8195b6f1)

```cpp
class IItem
{
public:    
    virtual int getId() = 0;
    virtual string getTitle() = 0;
    virtual double getPrice() = 0;
    virtual string getDescription() = 0;
    virtual ~IItem() = default;
};

class IBook
{
public:
    virtual string getISBN() = 0;
    virtual string getAuthor() = 0;
    virtual ~IBook() = default;
};

class IElectronics
{
public:
    virtual string getManufacturer() = 0;
    virtual ~IElectronics() = default;
};
```

```cpp
class Books : public IItem, public IBook
{
private:    
    int bookId;
    string bookTitle;
    double bookPrice;
    string bookAuthor;
    string bookISBN;
public:
    Books(int id, string name, double price,string author, string ISBN) 
        :bookId(id),bookTitle(name),bookPrice(price),bookAuthor(author), bookISBN(ISBN) {}

    int getId() override {
        return bookId;
    }

    string getTitle() override {
        return bookTitle;
    }

    double getPrice() override {
        return bookPrice;
    }

    string getDescription() override {
        return "Book Name: " + bookTitle + " Price: $" + to_string(bookPrice) + "\n\r";
    }

    string getISBN() override {
        return bookISBN;
    }

    string getAuthor() override {
        return bookAuthor;
    }
};

class Electronics : public IItem, public IElectronics
{
private:
    int itemId;
    string itemName;
    double itemPrice;
    string manufacturer;
public:
    Electronics(int id, string title, double price, string manufacturer)
        :itemId(id), itemName(title),itemPrice(price),manufacturer(manufacturer)
    {}

    int getId() override {
        return itemId;
    }

    string getTitle() override {
        return itemName;
    }

    double getPrice() override {
        return itemPrice;
    }

    string getDescription() override {
        return "Item Name: " + itemName + " Price: $" + to_string(itemPrice) + "\n\r";
    }

    string getManufacturer() { return manufacturer; }
};
```

```cpp
class ItemDecorator : public IItem
{
protected:
    shared_ptr<IItem> baseItem;

public:
    ItemDecorator(shared_ptr<IItem> item)
        :baseItem(std::move(item)){}

    int getId() override {
        return baseItem->getId();
    }

    string getTitle() override {
        return baseItem->getTitle();
    }

    double getPrice() override {
        return baseItem->getPrice();
    }

    string getDescription() override {
        return baseItem->getDescription();
    }
};

class GiftWrapperDecorator : public ItemDecorator
{
public:
    GiftWrapperDecorator(shared_ptr<IItem> newItem)
        :ItemDecorator(move(newItem)) {}
       

    double getPrice() override {
        return baseItem->getPrice() + 2.0;
    }

    string getDescription() override {
        return baseItem->getDescription() + " Selected Gift Wrapped: Additional Cost: $2.0" + "\n\r";
    }
};

class ExpressDeliveryDecorator : public ItemDecorator
{
public:
    ExpressDeliveryDecorator(shared_ptr<IItem> newItem)
        :ItemDecorator(move(newItem)) {}    

    double getPrice() override {
        return baseItem->getPrice() + 5.0;
    }

    string getDescription() override {
        return baseItem->getDescription() + " Selected Express Delivery: Additional Cost: $5.0" + "\n\r";
    }
};
```

```cpp
int main()
{
    shared_ptr<IItem> richDadPoorDad = make_shared<Books>(1, "Rich Dad Poor Dad", 20,"Robert K", "11223344");    
    shared_ptr<IItem> giftWrappedBook = make_shared<GiftWrapperDecorator>(richDadPoorDad);
    shared_ptr<IItem> expressDelivery = make_shared<ExpressDeliveryDecorator>(giftWrappedBook);
    cout << expressDelivery->getDescription()<< " Total Price: $"<< expressDelivery->getPrice() << endl;

    cout << endl;

    shared_ptr<IItem> iphone16Pro = make_shared<Electronics>(2, "iPhone 16 Pro", 300, "Apple");
    shared_ptr<IItem> iphoneExprDelivery = make_shared<ExpressDeliveryDecorator>(iphone16Pro);
    cout << iphoneExprDelivery->getDescription() << " Total Price: $" << iphoneExprDelivery->getPrice() << endl;

}
```