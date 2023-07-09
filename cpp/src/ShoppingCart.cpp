#include "ShoppingCart.h"

void addItemQuantity(const Product& product, double quantity);

std::vector<ProductQuantity> ShoppingCart::getItems() const {
    return items;
}

std::map<Product, double> ShoppingCart::getProductQuantities() const {
    return productQuantities;
}

void ShoppingCart::addItem(const Product& product) {
    addItemQuantity(product, 1.0);
}

void ShoppingCart::addItemQuantity(const Product& product, double quantity) {
    items.emplace_back(product, quantity);
    if (productQuantities.find(product) != productQuantities.end()) {
        productQuantities[product] += quantity;
    } else {
        productQuantities[product] = quantity;
    }
}

Discount* ShoppingCart::calculateDiscount(int unitPrice, double quantity, Offer offer, Product product){
    
    int quantityAsInt = int(quantity);
    double discountAmount = calculateDiscountAmount(offer, quantity, unitPrice);
    Discount* returnVal = nullptr;

    if (offer.getOfferType() == SpecialOfferType::TwoForAmount && quantityAsInt >= 2) {
        returnVal = new Discount("2 for " + std::to_string(offer.getArgument()), discountAmount, product);
    }
    if (offer.getOfferType() == SpecialOfferType::ThreeForTwo && quantityAsInt > 2) {
        returnVal =  new Discount("3 for 2", discountAmount, product);
    }
    if (offer.getOfferType() == SpecialOfferType::TenPercentDiscount) {
        returnVal = new Discount(std::to_string(offer.getArgument()) + "% off", discountAmount, product);
    }
    if (offer.getOfferType() == SpecialOfferType::FiveForAmount && quantityAsInt >= 5) {
        returnVal = new Discount(std::to_string(5) + " for " + std::to_string(offer.getArgument()), discountAmount, product);
    }
    return returnVal;
}

void ShoppingCart::handleOffers(Receipt& receipt, std::map<Product, Offer> offers, SupermarketCatalog* catalog) {
    for (const auto& productQuantity : productQuantities) {
        Product product = productQuantity.first;
        double quantity = productQuantity.second;
        if (offers.find(product) != offers.end()) {
            auto offer = offers[product];
            double unitPrice = catalog->getUnitPrice(product);
            Discount* discount = nullptr;

            discount = calculateDiscount(unitPrice, quantity, offer, product);

            if (discount != nullptr)
                receipt.addDiscount(*discount);
        }
    }
}

double ShoppingCart::calculateDiscountAmount(Offer offer, double quantity, double unitPrice){
    int quantityAsInt = int (quantity);
    switch (offer.getOfferType()){
    case SpecialOfferType::TwoForAmount:
        return -(unitPrice * quantity - (offer.getArgument() * (quantityAsInt / 2) + quantityAsInt % 2 * unitPrice));
    case SpecialOfferType::ThreeForTwo:
        return -(quantity * unitPrice - ((quantityAsInt / 3 * 2 * unitPrice) + quantityAsInt % 3 * unitPrice));
    case SpecialOfferType::TenPercentDiscount:
        return -quantity * unitPrice * offer.getArgument() / 100.0;
    case SpecialOfferType::FiveForAmount:
        return -(unitPrice * quantity - (offer.getArgument() * quantityAsInt / 5 + quantityAsInt % 5 * unitPrice));
    default:
        return 0;
    }
}