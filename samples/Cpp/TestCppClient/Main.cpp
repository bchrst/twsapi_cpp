/* Copyright (C) 2019 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "StdAfx.h"

#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <thread>

#include "TestCppClient.h"

const unsigned MAX_ATTEMPTS = 50;
const unsigned SLEEP_TIME = 10;

void printUsageExit() {
    std::cout<<"UsageExit: "<<std::endl;
    std::cout<<" - Stocks / Bonds / CFDs / etc: TestCppClient <tws host> <tws port> <connect options> <symbol> <sectype> <currency> <exchange>"<<std::endl;
    std::cout<<" - Options: TestCppClient <tws host> <tws port> <connect options> <symbol> <sectype> <currency> <exchange> <last trade date or contract month> <right> <strike> <multiplier>"<<std::endl;
    exit(0);
}

/* IMPORTANT: always use your paper trading account. The code below will submit orders as part of the demonstration. */
/* IB will not be responsible for accidental executions on your live account. */
/* Any stock or option symbols displayed are for illustrative purposes only and are not intended to portray a recommendation. */
/* Before contacting our API support team please refer to the available documentation. */
int main(int argc, char** argv)
{
        if(argc < 7) {
            printUsageExit();
        }

	const char* host = argv[1];

	int port = atoi(argv[2]);

        Contract myContract;

        if(std::string(argv[6]) == std::string("OPT")) {
            if(argc < 13) {
                printUsageExit();
            }
            myContract.lastTradeDateOrContractMonth = std::string(argv[9]);
            myContract.right = std::string(argv[10]);
            myContract.strike = atof(argv[11]);
            myContract.multiplier = std::string(argv[12]);
        }
        else if(argc < 9) {
            printUsageExit();
        }

        // Connect options is now mandatory. Type "" as the third argument to use default values
	const char* connectOptions = argv[3];

        myContract.delay = atoi(argv[4]);
        myContract.symbol = std::string(argv[5]);
        myContract.secType = std::string(argv[6]);
        myContract.currency = std::string(argv[7]);
        myContract.exchange = std::string(argv[8]);

	int clientId = 0;

	unsigned attempt = 0;
	printf( "Start of C++ Socket Client Test %u\n", attempt);

        TestCppClient client(myContract);

	for (;;) {
		++attempt;
		printf( "Attempt %u of %u\n", attempt, MAX_ATTEMPTS);

		// Run time error will occur (here) if TestCppClient.exe is compiled in debug mode but TwsSocketClient.dll is compiled in Release mode
		// TwsSocketClient.dll (in Release Mode) is copied by API installer into SysWOW64 folder within Windows directory 
		
		if( connectOptions) {
			client.setConnectOptions( connectOptions);
		}
		
		client.connect( host, port, clientId);
		
		while( client.isConnected()) {
			client.processMessages();
		}
		if( attempt >= MAX_ATTEMPTS) {
			break;
		}

		printf( "Sleeping %u seconds before next attempt\n", SLEEP_TIME);
		std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME));
	}

	printf ( "End of C++ Socket Client Test\n");
}

