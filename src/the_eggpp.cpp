#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>
#include <cmath>
using namespace std;

unsigned long long money = 0;
unsigned long long clicks = 0;

uint64_t upgrade_price = 10;
uint64_t jeremiasz_price = 1000;
uint64_t jeremiasz_upgrade_price = 100;
uint64_t slave_price = 10;
uint64_t pawel_price = 100;
uint64_t egg_price = 1000000000000;

bool jeremiasz = 0;
uint16_t jeremiasz_level = 0;
float multiplier = 1;
uint64_t pawels = 0;
uint64_t slaves = 0;
uint64_t eggs = 0;

short i;
string input;

bool store_status = 0; //0 - home || 1 - store

mutex game_mutex;

void wait(uint32_t ms){
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void clear(){
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void change_status(bool new_status){
    lock_guard<mutex> lock(game_mutex);
    store_status = new_status;
}

void jeremiasz_worker(){
    while (true){
        uint16_t level_copy;{
            std::lock_guard<std::mutex> lock(game_mutex);
            level_copy = jeremiasz_level;
        }        
        if (level_copy > 0){
            wait(static_cast<uint32_t>(ceil(1000.0 / level_copy)));
            unsigned long long money_copy;{
                std::lock_guard<std::mutex> lock(game_mutex);
                money_copy = money;
            }
            bool status_copy;{
                lock_guard<mutex> lock(game_mutex);
                status_copy = store_status;
            }
            float multiplier_copy;{
                lock_guard<std::mutex> lock(game_mutex);
                multiplier_copy = multiplier;
            }

            lock_guard<std::mutex> lock(game_mutex);
            money += static_cast<unsigned long long>((10 * multiplier_copy) * level_copy);
            if (!status_copy){
                clear();
                cout << "=============================\n\n[1] ENTER THE STORE.\n[2] EXIT.\n\n=============================\n\nBALANCE: "
                << money_copy
                << "$\nCURRENT MULTIPLIER: "
                << multiplier_copy
                << "x\nPRESS OR HOLD ENTER TO GAIN MONEY.";
            }
        }
        else {
            wait(100);
        }
    }   
}



void store(){
    clear();
    while (true) {
        unsigned long long money_copy;{
            std::lock_guard<std::mutex> lock(game_mutex);
            money_copy = money;
        }

        cout << "===================================\n\n" << "[1] UPGRADE (" << upgrade_price << " ZWD)\n";
        if (jeremiasz == false) {
            cout << "[2] HIRE JEREMIASZ (" << jeremiasz_price << " ZWD)\n";
        }   
        cout << "[3] UPGRADE JEREMIASZ (" << jeremiasz_upgrade_price << " ZWD)\n";
        cout << "[4] BUY A SLAVE THREAD (" << slave_price << " ZWD)\n";
        cout << "[5] BUY PAWEL (" << pawel_price << " ZWD)\n";
        cout << "[6] BUY EGG (" << egg_price << " ZWD)\n";
        cout << "[7] EXIT\n";
        cout << "\n===================================\nBALANCE: " << money_copy << " ZIMBABWE DOLLARS\n" << flush;
        cout << "\nINPUT THE INDEX OF THE ITEM YOU PRAISE: ";
        cin >> i;
        if (cin.fail()){
            cout << "\nTHE INDEX IS SUPPOSED TO BE AN INTEGER, TRY AGAIN.\n";
            cin.clear();
            cin.ignore();
        }
        else{
            switch (i){
                case 1: {
                    lock_guard<std::mutex> lock(game_mutex);
                    if (money >= upgrade_price){
                        money -= upgrade_price;
                        if (multiplier < 2) {
                            multiplier = 2;
                        }
                        else {
                            multiplier *= 1.4;
                        }
                        upgrade_price *=2;
                        clear();
                        cout << "\nSUCCESSFULLY BOUGHT AN UPGRADE | CURRENT MULTIPLIER: " << multiplier <<"x\n";
                    }
                    else {
                        clear();
                        cout << "\nNOT ENOUGH MONEY. YOU NEED " << upgrade_price - money << " ZIMBABWE DOLLARS MORE TO BUY AN UPGRADE.\n";
                    }
                    break;
                }
                case 2: {
                    lock_guard<std::mutex> lock(game_mutex);
                    if (money >= jeremiasz_price && !jeremiasz){
                        money -= jeremiasz_price;
                        jeremiasz = true;
                        jeremiasz_level = 1;
                        clear();
                        cout << "\nSUCCESSFULLY HIRED JEREMIASZ | CURRENT JEREMIASZ LEVEL: " << jeremiasz_level <<"x\n";
                    }
                    else if (money >= jeremiasz_price && jeremiasz){
                        clear();
                        cout << "\nJEREMIASZ IS ALREADY EXISTANT, TRY AGAIN LATER.";
                    }
                    else {
                        clear();
                        cout << "\nNOT ENOUGH MONEY TO HIRE THE LORD... HOW DARE YOU. YOU NEED " << jeremiasz_price - money << " ZIMBABWE DOLLARS MORE TO ACQUIRE THE KING'S WORK.\n";
                    }
                    break;
                }
                case 3: {
                    lock_guard<std::mutex> lock(game_mutex);
                    if (money >= jeremiasz_upgrade_price && jeremiasz && jeremiasz_level <= 250){
                        money -= jeremiasz_upgrade_price;
                        jeremiasz_level += 1;
                        jeremiasz_upgrade_price *= 2;
                        clear();
                        cout << "\nSUCCESSFULLY UPGRADED JEREMIASZ. | CURRENT JEREMIASZ LEVEL: " << jeremiasz_level <<".\n";
                    }
                    else if (money >= jeremiasz_upgrade_price && !jeremiasz){
                        clear();
                        cout << "\nYOU NEED THE KING FIRST\n";
                    }
                    else if (money >= jeremiasz_upgrade_price && jeremiasz_level == 250) {
                        clear();
                        cout << "\nYOU CANNOT UPGRADE JEREMIASZ UP TO HIGHER LEVELS THAN 250\n";
                    }
                    else {
                        clear();
                        cout << "\nNOT ENOUGH MONEY TO UPGRADE THE MIGHT JEREMIASZ. YOU REQUIRE " << jeremiasz_upgrade_price - money << " ZIMBABWE DOLLARS MORE TO ACQUIRE THE UPGRADE.\n";
                        /*cout << "\nDEBUG:"
                            << "\nmoney = " << money
                            << "\nprice = " << jeremiasz_upgrade_price
                            << "\njeremiasz = " << jeremiasz
                            << "\nlevel = " << jeremiasz_level << endl;*/
                    }
                    break;
                }   
                case 4: {
                    lock_guard<std::mutex> lock(game_mutex);
                    if (money >= slave_price) {
                        money -= slave_price;
                        slaves += 1;
                        slave_price *=2;
                        clear();
                        cout << "\nSUCCESSFULLY BOUGHT A SLAVE THREAD. | CURRENT AMOUNT OF SLAVE THREADS: " << slaves <<".\n";
                    }
                    else {
                        clear();
                        cout << "\nNOT ENOUGH MONEY TO BUY A SLAVE THREAD.. WHAT A SHAME. | YOU NEED " << slave_price - money << " ZIMBABWE DOLLARS MORE.\n";
                    }
                    break;
                }
                case 5: {
                    lock_guard<std::mutex> lock(game_mutex);
                    if (money >= pawel_price) {
                        money -= pawel_price;
                        pawel_price *= 2;
                        pawels += 1;
                        clear();
                        cout << "\nSUCCESSFULLY ACQUIRED A PAWEL. | CURRENT AMOUNT OF PAWELS: " << pawels <<".\n";
                    }
                    else {
                        clear();
                        cout << "\nNOT ENOUGHT MONEY TO ACQUIRE A PAWEL. | YOU NEED " << pawel_price - money <<" ZIMBABWE DOLLARS MORE TO ACQUIRE ONE.\n";
                    }
                    break;
                }
                case 6: {
                    lock_guard<std::mutex> lock(game_mutex);
                    if (money >= egg_price) {
                        money -= egg_price;
                        egg_price *= 2;
                        eggs += 1;
                        clear();
                        cout << "\nSUCCESSFULLY BOUGHT AN EGG. | CURRENT AMOUNT OF EGGS IN YOUR SACK: " << eggs <<".\n";
                    }
                    else {
                        clear();
                        cout << "\nNOT ENOUGH MONEY TO BUY AN EGG. | YOU NEED " << egg_price - money <<" ZIMBABWE DOLLARS MORE TO BUY ONE.\n";
                    }
                    break;
                }
                case 7:
                    change_status(0);
                    return;
                    
                default:
                    clear();
                    cout << "\nNO OPTION CHOOSEN, TRY AGAIN.\n";
                    wait(400);
                    break;
            }
        }
    }
}

void home() {
    while (true) {
        clear();
        unsigned long long money_copy;
        float multiplier_copy;{
            lock_guard<std::mutex> lock(game_mutex);
            money_copy = money;
            multiplier_copy = multiplier;
        }

        cout << "=============================\n\n[1] ENTER THE STORE.\n[2] EXIT.\n\n=============================\n\nBALANCE: "
             << money_copy
             << "$\nCURRENT MULTIPLIER: "
             << multiplier_copy
             << "x\nPRESS OR HOLD ENTER TO GAIN MONEY.";

        getline(cin, input);
        if (input == "1"){
            change_status(1);
            store();
        }
        else if (input == "2"){
            exit(1);
        }
        else if (input.empty()){
            lock_guard<std::mutex> lock(game_mutex);
            money += static_cast<unsigned long long>(1 * multiplier);
        }
    }
}

int main() {
    thread jeremiasz_t(jeremiasz_worker);

    while (true) {
        home();
        clear();
        cout << "\n\nexited the home loop...\n\n";
        wait(1000);
    }

    jeremiasz_t.join();
}