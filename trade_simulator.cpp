#include <iostream>
using namespace std;
#include <vector>
#include <cctype>

void fill_stock_prices(double array[], int array_len)
{
    for (int i = 0; i < array_len; i++)
    {
        cout << "Price of Stock " << i+1 << " - " ;
        cin >> array[i];
    }
}


void print_stock_prices(double stock_prices[], int num)
{
    for (int i = 0; i < num ; i++)
    {
        cout << " Price of Stock " << i + 1 << " is - " << stock_prices[i]  << endl;
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
                cout << "Enter stock price - " ;
                cin >> stock_price;
                
                stock_price_vector.push_back(stock_price);
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

    //fill_stock_prices(stock_prices, num);
    //print_stock_prices(stock_prices, num);
}
