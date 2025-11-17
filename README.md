[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/yqZxj4OI)
# Stock Exchange Matching Engine

Traders continuously post bids to buy or sell stock. A limit order means that a buyer (seller) places an order to buy (sell) a specified amount of a given stock at or below (at or above) a given price. The order book displays buy and sell orders, and ranks them by price and then by time. Matching engine matches compatible buyers and sellers; if there are multiple possible buyers, break ties by choosing the buyer that placed the bid earliest. Use two priority queues for each stock, one for buyers and one for sellers.

A continuous limit order book (CLOB) is a trading system used by exchanges to match buy and sell orders based on both price and time priority, ensuring a transparent and real-time market. It operates as a central, dynamic list of all open orders, with bids on one side and asks on the other, continuously updated as new orders are added or existing ones are executed or canceled. CLOBs provide transparency into market depth and facilitate fair pricing by allowing traders to see and compete for the best available prices.

On this challenge, you're implementing a Continuos Limit Order Book (CLOB). Buyers and sellers will be **dynamically** created and then displayed in a **real-time dashboard**. Consider that your implementation should look **as realist as possible** (you can use colors). You are free to choose the number of rows to display. Take a look in the following [document](https://faculty.haas.berkeley.edu/hender/ITpro.pdf), the Island's limit order book can be a good example of what is expected, but in terminal.


## How to build and run

_ADD YOUR INSTRUCTIONS HERE_


## Deliverables

- Source code must be implemented in `*.cpp` and `*.h` files
- Instructions at [How to build and run](#how-to-build-and-run) section
- [DOCS.md](./DOCS.md)
    - Introduction
    - Research on stocks exchange 
    - Software Design and Implementation (diagrams, charts, key functions, code snippets, etc)
    - Conclusions and Learnings (one per teammate)
 
## Grading Policy

| Rubric                           | Points |
|----------------------------------|--------|
| Use of Priority Queues           |   40   |
| Terminal-based real-time UI      |   30   |
| Documentation                    |   30   |
| Total                            |   100  |