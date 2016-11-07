//
//  main.cpp
//  PipelineExmpl
//
//  Created by grigorii on 25/10/16.
//  Copyright © 2016 grigorii. All rights reserved.
//


#include <iostream>
#include <unistd.h>

#include "InStage.h"
#include "OutStage.h"
#include "InOutStage.h"
#include "timer.hpp"

const int IN_PACKAGE_SIZE = 1024*768;
const int IN_PACKAGE_NUM = 1000;

class ConcreteInStage : public InStage {
    int pckgCounter{0};
public:
	void StageTask( ) override {
		for( int i = 0; i < IN_PACKAGE_NUM; ++i ) {
            auto query = qbmq->Pop();
			auto msg = std::make_unique<Message>( pckgCounter, IN_PACKAGE_SIZE,
                                                 ( pckgCounter == IN_PACKAGE_NUM - 1 ) ? LastFlag::on : LastFlag::off );
			//std::cout << "Push it to the limit " << i << "\n";
			bmq->Push( std::move( msg ) );
            ++pckgCounter;
		}
	}
};

class ConcreteOutStage : public OutStage {
public:

	void StageTask( std::unique_ptr<Message> msg ) override {
		std::string m( "Read " );
		m += std::to_string( msg->data[0] ) + "\n";
		std::cerr << m;
	}
};
        
class FirstConcreteInOutStage : public InOutStage {
public:
    std::unique_ptr<Message> StageTask( std::unique_ptr<Message> msg ) override {
        std::for_each( std::begin( msg->data ) , std::end( msg->data ), [] ( int &n ) {
            n *= 10;
        });
        return msg;
    }
};



int main(int argc, const char * argv[]) {
    //is <qbmq1 mbmq1> ios1 <qbmq2 mbmq2> os
    auto mbmq1 = std::make_shared<MessageBMQType>( );
    auto qbmq1 = std::make_shared<QueryBMQType>( );
    auto mbmq2 = std::make_shared<MessageBMQType>( );
    auto qbmq2 = std::make_shared<QueryBMQType>( );
    
    auto is = std::make_shared<ConcreteInStage>( );
    auto os = std::make_shared<ConcreteOutStage>( );
    auto ios1 = std::make_shared<FirstConcreteInOutStage>( );
    
    is->SetOutBMQ( mbmq1 );
    ios1->SetInBMQ( mbmq1 );
    ios1->SetOutBMQ( mbmq2 );
    os->SetInBMQ( mbmq2 );
    
    os->SetOutQBMQ( qbmq2 );
    ios1->SetInQBMQ( qbmq2 );
    ios1->SetOutQBMQ( qbmq1 );
    is->SetInQBMQ( qbmq1 );

    is->Run();
    ios1->Run();
    for( int i = 0; i < IN_PACKAGE_NUM; ++i ) {
        os->Run();
    }
    
    std::cout << "After getting all tasks\n";

    while( !os->TaskCompleted() );
    
    std::cout << "After finishing all tasks\n";

	
	return 0;
}
