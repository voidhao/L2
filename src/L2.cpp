//============================================================================
// Name        : L2.cpp
// Author      : vincent
// Version     :
// Copyright   : Copyright 2016 Lonlife Inc. All rights reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "netio/netio.h"
using namespace std;

int main(int argc, char** argv) {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	int id = 0;
	id = netio::open_dev("eth1");
	id = netio::open_dev("eth2");
	id = netio::open_dev("eth3");
	id = netio::open_dev("eth4");
	return 0;
}
