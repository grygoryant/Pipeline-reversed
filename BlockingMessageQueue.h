//
//  BlockingQueue.h
//  PipelineExmpl
//
//  Created by grigorii on 27/10/16.
//  Copyright © 2016 grigorii. All rights reserved.
//

#ifndef BlockingMessageQueue_h
#define BlockingMessageQueue_h

#include <iostream>
#include <thread>
#include <queue>

#include "Message.h"


template<class T>
class BlockingMessageQueue {
    std::mutex qMutex;
    std::condition_variable qCv;
    std::queue<T> queue;
    
public:
    
    void Push( T msg ) {
        std::unique_lock<std::mutex> lock( qMutex );
        queue.push( std::move( msg ) );
        qCv.notify_one();
    }
    
    T Pop() {
        std::unique_lock<std::mutex> lock( qMutex );
        qCv.wait( lock, [&] {
            return !queue.empty();
        });
        T retMsg( std::move( queue.front() ) );
        queue.pop();
        return std::move( retMsg );
    }
};

using MessageBMQType = BlockingMessageQueue<std::unique_ptr<Message>>;
using QueryBMQType = BlockingMessageQueue<std::unique_ptr<Query>>;

#endif /* BlockingQueue_h */
