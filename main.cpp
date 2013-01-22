#include <iostream>
#include <mutex>
#include <thread>
#include <stdlib.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>

using namespace std;

const int USERS = 2; //the number of users

mutex isBusy;
int balance = 0;
int debt = 0;


void user(){
    while(true){
        sleep(rand() % 5 + 1);
        int money = rand() % 100 - 30;
        if ( money > 0 ){
            isBusy.lock();
            balance += money;
            isBusy.unlock();
            cout << money << "$ successfully enrolled. Now : " << balance << " $ " << endl;
            if( debt > 0 && balance > debt){
                isBusy.lock();
                debt = 0;
                balance -= debt;
                isBusy.unlock();
                cout << debt << "$ have been removed for debt. Now : " << balance << " $ " << endl;
            }
        } else {
            if ( balance > money*(-1)){ // we have more than need.
                isBusy.lock();
                balance -= money*(-1);
                isBusy.unlock();
                cout << money*(-1) << "$ successfully removed. Now :" << balance << " $ " << endl;
            } else { // we'll have debt
                isBusy.lock();
                debt += money*(-1);
                isBusy.unlock();
                cout << "There is not enough money. Debt now : " << debt << " $ " << endl;
            }
        }
        sleep(rand() % 5 + 1);
    }
}


int main()
{
    vector<thread> th;

    for( int i = 0; i < USERS; i++ ){
        th.push_back( thread(&user) );
    }
    for( int i = 0; i < USERS; i++ ){
         th[i].join();
    }
    th.clear();
    return 0;
}
