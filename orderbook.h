// This file defines the OrderBook class where it stores buy or sell orders using priority queues and executes when the price cross

#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "order.h"
#include <queue>
#include <vector>
#include <mutex>
#include <string>

struct TopRow {
    double price; //best price at this level
    uint64_t qty; //quantity of top order
    OrderId earliest_order_id;
};

class OrderBook {
public:
    explicit OrderBook(const std::string& symbol);
    ~OrderBook();

    //add a new order to the appropriate queue.
    void add_order(const Order& o);

    //match BUY and SELL orders 
    void match();

    //get top n levels
    std::vector<TopRow> top_buys(size_t n);
    std::vector<TopRow> top_sells(size_t n);

    uint64_t total_trades_executed() const;

private:
    //comparator for buy priority queue
    struct BuyCmp {
        bool operator()(const Order& a, const Order& b) const {
            if (a.price != b.price)
                return a.price < b.price; 
            return a.id > b.id; 
        }
    };
    //comparator for sell priority queue
    struct SellCmp {
        bool operator()(const Order& a, const Order& b) const {
            if (a.price != b.price)
                return a.price > b.price; 
            return a.id > b.id; 
        }
    };
    std::string symbol_;

    //priority queues storing orders
    std::priority_queue<Order, std::vector<Order>, BuyCmp> buys_;
    std::priority_queue<Order, std::vector<Order>, SellCmp> sells_;

    mutable std::mutex mtx_;   
    uint64_t trades_executed_; 

    //for top N values
    template<typename Cmp>
    static std::vector<TopRow> snapshot_pq(
        std::priority_queue<Order, std::vector<Order>, Cmp> pq, size_t n);
};

#endif // ORDERBOOK_H

