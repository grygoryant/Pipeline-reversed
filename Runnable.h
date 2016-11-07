//
//  Runnable.h
//  PipelineExmpl
//
//  Created by grigorii on 27/10/16.
//  Copyright © 2016 grigorii. All rights reserved.
//

#ifndef Runnable_h
#define Runnable_h

#include <iostream>
#include <list>
#include <future>

class Runnable {
    using Task = std::function<void()>;
    
public:
    Runnable() { }
    
    void Execute( Task task ) {
        futures.push_back( std::async( task ) );
    }
    
//    bool TasksCompleted() {
//        std::for_each( std::begin( futures ), std::end( futures ), [&] ( auto &f ) {
//            return f.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
//        } );
//        return true;
//    }
    
    virtual ~Runnable() {
        std::for_each( std::begin( futures ), std::end( futures ), [&] ( auto &f ) {
            f.get();
        } );
    }
    
    Runnable( const Runnable & ) = delete;
    void operator=( const Runnable & ) = delete;
    
protected:
    std::list<std::future<void>> futures;
};


#endif /* Runnable_h */
