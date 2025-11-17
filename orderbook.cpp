//This file Implements the OrderBook logicwhere the priority queues determine which order has highest priorityand the match() function executes trades when a buy price is >= SELL price

#include "orderbook.h"
#include <algorithm>
#include <chrono>


//higher price = higher priority
struct OrderBook::BuyCmp { 
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price)
            return a.price < b.price;// here highest price first
        return a.id > b.id; //here earliest order first
    }
};


//lower proce = higher prpiority 
struct OrderBook::SellCmp {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price)
            return a.price > b.price; //lowest price first
        return a.id > b.id; //earliest first
    }
};

OrderBook::OrderBook(const std::string& symbol)
    : symbol_(symbol), trades_executed_(0) {}

OrderBook::~OrderBook() = default;

//sort the order into the correct queue.
void OrderBook::add_order(const Order& o) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (o.is_buy)
        buys_.push(o);
    else
        sells_.push(o);
}

// Match BUY and SELL orders while best BUY price >= best SELL price
void OrderBook::match() {
    std::lock_guard<std::mutex> lock(mtx_);

    while (!buys_.empty() && !sells_.empty()) {
        Order buy = buys_.top();
        Order sell = sells_.top();

        //If prices cross then trade occurs
        if (buy.price >= sell.price) {

            uint64_t trade_qty = std::min(buy.qty, sell.qty);
            trades_executed_++;

            //removes them
            buys_.pop();
            sells_.pop();

            buy.qty -= trade_qty;
            sell.qty -= trade_qty;

            if (buy.qty > 0) buys_.push(buy);
            if (sell.qty > 0) sells_.push(sell);

        } else {
            break; //where orders no longer match
        }
    }
}

//top N rows for  display
template<typename Cmp>
std::vector<TopRow> OrderBook::snapshot_pq(
    std::priority_queue<Order, std::vector<Order>, Cmp> pq, size_t n)
{
    std::vector<TopRow> rows;
    rows.reserve(n);

    while (!pq.empty() && rows.size() < n) {
        Order o = pq.top();
        pq.pop();
        rows.push_back({o.price, o.qty, o.id});
    }

    return rows;
}

std::vector<TopRow> OrderBook::top_buys(size_t n) {
    std::lock_guard<std::mutex> lock(mtx_);
    return snapshot_pq(buys_, n);
}

std::vector<TopRow> OrderBook::top_sells(size_t n) {
    std::lock_guard<std::mutex> lock(mtx_);
    return snapshot_pq(sells_, n);
}

uint64_t OrderBook::total_trades_executed() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return trades_executed_;
}

