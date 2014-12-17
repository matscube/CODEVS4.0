//
//  main.cpp
//  CODEVS4.0
//
//  Created by TakanoriMatsumoto on 2014/12/16.
//  Copyright (c) 2014年 TakanoriMatsumoto. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include "IOManager.h"

using namespace std;



int main(int argc, const char * argv[]) {
    ifstream arq(getenv("MYARQ"));
    cin.rdbuf(arq.rdbuf());
    
    int n;
    cin >> n;
    cout << n;
    cin >> n;
    cout << n;
    cin >> n;
    cout << n;
    
    // insert code here...
//    cout << "Hello, World!\n";
    
    IOManager iOManager;
    iOManager.input();

    
/*    for (int i = 0; i < 10; i++) {
        cout << i << endl;
    }
    
    int n;
    cout << "put n:";
    cin >> n;
    cout << n << endl;*/
    
    return 0;
}
