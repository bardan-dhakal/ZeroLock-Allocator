#include <iostream>
using namespace std;
#include <vector>
#include <cctype>

void fill_stock_prices(vector<double> &price)
{   
    double stock_price;
    cout << "Enter stock price - ";
    cin >> stock_price;

    price.push_back(stock_price);

}


void print_stock_prices(vector<double> price, int size)
{   
    cout << " Here are the stock prices you have provided" << endl;
    for (int i = 0; i < size ; i++)
    {
        cout << " Price of Stock " << i + 1 << " is - " << price[i]  << endl;
    }
}


int main()
{
    double stock_price;
    vector<double> stock_price_vector; 


    bool flag  = true;
    char input;

        while (flag) 
        { 
            cout << "Do you want to enter a stock price? Y/N " << endl;
            cin >>  input;
            input = (char) toupper(input);

            if (input == 'Y' )
            {

                fill_stock_prices(stock_price_vector);
            }


            else if (input == 'N')
            {
                flag = false;
            }

            else
            {
                cout << "Invalid keyword entered, try again." << endl;
            }
            
        }
    
    int size = stock_price_vector.size();

    print_stock_prices(stock_price_vector, size);

    

}
