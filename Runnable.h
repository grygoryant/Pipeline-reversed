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
#include <thread>
#include <queue>

class Runnable {
    using Task = std::function<void()>;
    
public:
    Runnable() {
        thread = std::unique_ptr<std::thread>( new std::thread( [&] {
            this->Run();
        } ) );
    }
    
    void Execute( Task task ) {
        waitingTasks.push( task );
    }
    
    bool TaskCompleted() {
        return !thread->joinable();
    }
    
    virtual ~Runnable() {
        Execute( [&] {
            done = true;
            std::cout << "Finishing\n";
        } );
        if( thread->joinable() ) {
            thread->join();
        }
    }
    
    void Run() {
        done = false;
        while( !done ) {
            if( !waitingTasks.empty() ) {
                auto t = waitingTasks.front();
                t();
            }
        }
    }
    
    Runnable( const Runnable & ) = delete;
    void operator=( const Runnable & ) = delete;
    
protected:
    std::unique_ptr<std::thread> thread;
    std::queue<Task> waitingTasks;
    bool done{false};
};

//class Runnable {
//    using Task = std::function<void()>;
//    
//public:
//    Runnable() {
//        thread = std::unique_ptr<std::thread>( new std::thread(  ) );
//    }
//    
//    void Execute( Task task ) {
//        if( thread->joinable() ) {
//            thread->join();
//        }
//        thread = std::make_unique<std::thread>( task );
//    }
//    
//    bool TaskCompleted() {
//        return !thread->joinable();
//    }
//    
//    virtual ~Runnable() {
//        if( thread->joinable() )
//            thread->join();
//    }
//    
//    Runnable( const Runnable & ) = delete;
//    void operator=( const Runnable & ) = delete;
//    
//protected:
//    std::unique_ptr<std::thread> thread;
//    
//};

#endif /* Runnable_h */
