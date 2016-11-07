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
const int IN_PACKAGE_NUM = 10;

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
    //for( int i = 0; i < IN_PACKAGE_NUM; ++i ) {
        os->Run();
    //}
    
    while( !os->TaskCompleted() );

//	//is -> bmq1 -> ios1 -> bmq2 -> ios2 -> bmq3 -> os
//    auto mbmq1 = std::make_shared<MessageBMQType>( );
//	auto mbmq2 = std::make_shared<MessageBMQType>( );
//	auto mbmq3 = std::make_shared<MessageBMQType>( );
//    
//    auto qbmq1 = std::make_shared<QueryBMQType>( );
//    auto qbmq2 = std::make_shared<QueryBMQType>( );
//    auto qbmq3 = std::make_shared<QueryBMQType>( );
//	
//	auto is = std::make_shared<ConcreteInStage>( );
//	auto os = std::make_shared<ConcreteOutStage>( );
//	auto ios1 = std::make_shared<FirstConcreteInOutStage>( );
//	auto ios2 = std::make_shared<SecondConcreteInOutStage>( );
//	
//	is->SetOutBMQ( mbmq1 );
//	ios1->SetInBMQ( mbmq1 );
//	ios1->SetOutBMQ( mbmq2 );
//	ios2->SetInBMQ( mbmq2 );
//	ios2->SetOutBMQ( mbmq3 );
//	os->SetInBMQ( mbmq3 );
//	
//	TimePoint t1 = currentTime();
//    
//	is->Run();
//	ios1->Run();
//	ios2->Run();
//	os->Run();
//
//    while( !os->TaskCompleted() );
//    
//	TimePoint t2 = currentTime();
//	
//	std::cout << "Elapsed time: " << elapsedTime( t1, t2 ) << "\n";
	
//	t1 = currentTime();
//	for( int i = 0; i < IN_PACKAGE_NUM; ++i ) {
//		std::vector<int> v( IN_PACKAGE_SIZE, i );
//		std::for_each( std::begin( v ) , std::end( v ), [] ( int &n ) {
//			n *= 10;
//			n /= 10;
//		});
//		std::for_each( std::begin( v ) , std::end( v ), [] ( int &n ) {
//			n *= 10;
//			n /= 10;
//		});
//		std::string m( "Read " );
//		m += std::to_string( v[0] ) + "\n";
//		std::cerr << m;
//	}
//	t2 = currentTime();
//	
//	std::cout << "Elapsed time: " << elapsedTime( t1, t2 ) << "\n";
	
	return 0;
}
        
        //
        //        //
        //class SecondConcreteInOutStage : public InOutStage {
        //public:
        //	std::unique_ptr<Message> StageTask( std::unique_ptr<Message> msg ) override {
        //		std::for_each( std::begin( msg->data ) , std::end( msg->data ), [] ( int &n ) {
        //			n /= 10;
        //		});
        //		return msg;
        //	}
        //};

