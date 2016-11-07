//
//  InOutStage.h
//  PipelineExmpl
//
//  Created by grigorii on 28/10/16.
//  Copyright © 2016 grigorii. All rights reserved.
//

#ifndef InOutStage_h
#define InOutStage_h

#include "Runnable.h"
#include "BlockingMessageQueue.h"
#include "Message.h"

class InOutStage {
protected:
    Runnable rnbl;
    std::shared_ptr<MessageBMQType> inBmq;
    std::shared_ptr<MessageBMQType> outBmq;
    
    std::shared_ptr<QueryBMQType> inQBmq;
    std::shared_ptr<QueryBMQType> outQBmq;
    bool completed;
public:
    InOutStage() : completed(false) { }
    
    virtual ~InOutStage() { }
    
    void SetInBMQ( std::shared_ptr<MessageBMQType> bmq ) {
        this->inBmq = bmq;
    }
    void SetOutBMQ( std::shared_ptr<MessageBMQType> bmq ) {
        this->outBmq = bmq;
    }
    
    
    void SetInQBMQ( std::shared_ptr<QueryBMQType> qbmq ) {
        this->inQBmq = qbmq;
    }
    void SetOutQBMQ( std::shared_ptr<QueryBMQType> qbmq ) {
        this->outQBmq = qbmq;
    }
    
    bool TaskCompleted() {
        return completed;
    }
    
    virtual std::unique_ptr<Message> StageTask( std::unique_ptr<Message> msg ) = 0;
    
    void Run() {
        completed = false;
        rnbl.Execute( [&] {
            while( !completed ) {
                auto query = inQBmq->Pop();
                outQBmq->Push( std::move( query ) );
                
                auto msg = inBmq->Pop();
                LastFlag f = msg->flag;
                auto procMsg = StageTask( std::move( msg ) );
                outBmq->Push( std::move( procMsg ) );
                if( f == LastFlag::on )
                    completed = true;
            }
        } );
    }
};

#endif /* InOutStage_h */
