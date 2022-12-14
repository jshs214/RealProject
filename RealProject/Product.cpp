#include "Product.h"

#include<iostream>

using namespace std;

Product::Product(string productID, string productName, int price, string producttype, int stock)
	: m_productID(productID), m_productName(productName), m_price(price), m_productType(producttype), m_stock(stock)
{
}

string Product::getProductID() const
{
	return m_productID;
}
void Product::setProductID(string& productID)
{
	m_productID = productID;
}
string Product::getProductName() const
{
	return m_productName;
}
void Product::setProductName(string& productName)
{
	m_productName = productName;
}
int Product::getPrice() const
{
	return m_price;
}
void Product::setPrice(int& price)
{
	m_price = price;
}
string Product::getProductType() const
{
	return m_productType;
}
int Product::getStock() const
{
	return m_stock;
}
void Product::setStock(int& stock)
{
	m_stock = stock;
}