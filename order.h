//This file is based in the order structure used by the order book where the orders represent a request to buy or sell a stock at a specific  limit for the price 


#ifndef ORDER_H
#define ORDER_H

#include <cstdint>
#include <chrono>
#include <string>

//ordering the id´s
using OrderId = uint64_t;

//timestamps
using TimePoint = std::chrono::steady_clock::time_point;


struct Order {
    OrderId id; //the id to identify the order
    std::string trader; //name of the person who placed it
    bool is_buy; //buy or sell
    double price; //limit price
    uint64_t qty;
    TimePoint time; //timestamp

    Order() = default;

    Order(OrderId _id, const std::string& _trader, bool _is_buy,
          double _price, uint64_t _qty, TimePoint _time)
        : id(_id), trader(_trader), is_buy(_is_buy),
          price(_price), qty(_qty), time(_time) {}
};

#endif // ORDER_H

