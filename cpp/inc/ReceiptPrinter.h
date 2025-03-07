#ifndef CPP_RECEIPTPRINTER_H
#define CPP_RECEIPTPRINTER_H


#include "Receipt.h"

#include <iomanip>
#include <sstream>

#define NumberOfReceiptPrinters 40
#define NormalPrecision 2
#define MinimumPrecision 0
#define MaximumPrecision 3

class ReceiptPrinter
{

public:
    ReceiptPrinter();

    ReceiptPrinter(int columns);

    std::string printReceipt(const Receipt &receipt);

    std::string presentReceiptItem(const ReceiptItem &item) const;

    std::string presentDiscount(const Discount &discount) const;

    std::string presentTotal(const Receipt &receipt) const;

    std::string formatLineWithWhitespace(const std::string &name, const std::string &value) const;

    std::string presentPrice(double price) const;

    static std::string presentQuantity(const ReceiptItem &item);

private:

    static std::string getFormattedNumberAsString(double number, int precision);

    const int columns;


};


#endif //CPP_RECEIPTPRINTER_H
