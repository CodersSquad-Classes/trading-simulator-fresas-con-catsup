//the main file the runs 3 theads 

#include "orderbook.h"
#include "order.h"

#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <atomic>
#include <iomanip>

using namespace std::chrono_literals;

//id counter for new orders
std::atomic<OrderId> global_order_id{1};

//controls of threads
std::atomic<bool> running{true};

//generate  trader name from the id
std::string trader_name_from_id(uint64_t id) {
    return "Trader_" + std::to_string(id);
}

//#1 random order generator that creates the buy or sell orders and simulates the traders 
void generator_thread(OrderBook& ob, int orders_per_sec) {
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<int> side(0, 1);//verify the buy or sell
    std::uniform_real_distribution<double> delta(-1, 1);//the price variation
    std::uniform_int_distribution<int> qty(1, 100);//the quantity

    double mid_price = 100.0; //the average price

    while (running.load()) {
        for (int i = 0; i < orders_per_sec; i++) {
            bool is_buy = (side(rng) == 0);
            double price = std::max(1.0, mid_price + delta(rng));
            uint64_t quantity = qty(rng);

            OrderId id = global_order_id.fetch_add(1);

            Order o(
                id,
                trader_name_from_id(id),
                is_buy,
                price,
                quantity,
                std::chrono::steady_clock::now()
            );

            ob.add_order(o);

            std::this_thread::sleep_for(10ms);
        }
        std::this_thread::sleep_for(100ms);
    }
}

//#2 calls the match dfunction to execute the trades 
void matcher_thread(OrderBook& ob) {
    while (running.load()) {
        ob.match();
        std::this_thread::sleep_for(50ms);
    }
}

//#3 displays the order book in real time and uses green fot buy and red for sell
void ui_thread(OrderBook& ob, size_t rows) {
    while (running.load()) {
        // Clear screen
        std::cout << "\033[2J\033[H";

        std::cout << "==== REAL-TIME ORDER BOOK ====\n\n";

        auto buys = ob.top_buys(rows);
        auto sells = ob.top_sells(rows);

        std::cout << "     BUY ORDERS          SELL ORDERS\n";
        std::cout << "-------------------------------------------\n";

        for (size_t i = 0; i < rows; i++) {
            //buy display (green)
            if (i < buys.size()) {
                std::cout << "\033[32m";
                std::cout << std::setw(6) << buys[i].price
                          << " x " << std::setw(4) << buys[i].qty;
                std::cout << "\033[0m";
            } else {
                std::cout << "              ";
            }

            std::cout << "     ";

            //sell display (red)
            if (i < sells.size()) {
                std::cout << "\033[31m"; 
                std::cout << std::setw(6) << sells[i].price
                          << " x " << std::setw(4) << sells[i].qty;
                std::cout << "\033[0m";
            }

            std::cout << "\n";
        }

        std::cout << "\nTrades executed: " << ob.total_trades_executed() << "\n";

        std::this_thread::sleep_for(500ms);
    }
}

//main 
int main() {
    OrderBook ob("ABC");

    std::thread gen(generator_thread, std::ref(ob), 10);
    std::thread matcher(matcher_thread, std::ref(ob));
    std::thread ui(ui_thread, std::ref(ob), 10);

    std::cout << "Simulation running. Press ENTER to exit.\n";
    std::cin.get();

    running.store(false);

    gen.join();
    matcher.join();
    ui.join();

    std::cout << "Simulation stopped.\n";
    return 0;
}

