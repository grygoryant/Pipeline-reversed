//
//  InStage.h
//  PipelineExmpl
//
//  Created by grigorii on 28/10/16.
//  Copyright © 2016 grigorii. All rights reserved.
//

#ifndef InStage_h
#define InStage_h


#include "Runnable.h"
#include "BlockingMessageQueue.h"
#include "Message.h"


class InStage {
protected:
    Runnable rnbl;
    std::shared_ptr<MessageBMQType> bmq;
    std::shared_ptr<QueryBMQType> qbmq;
    bool completed;
    
public:
    
    void SetOutBMQ( std::shared_ptr<MessageBMQType> bmq ) {
        this->bmq = bmq;
    }
    
    void SetInQBMQ( std::shared_ptr<QueryBMQType> qbmq ) {
        this->qbmq = qbmq;
    }
    
    bool TaskCompleted() {
        return completed;
    }

    virtual void StageTask() = 0;
    
    void Run() {
        completed = false;
        rnbl.Execute( [&] {
            //auto query = qbmq->Pop();
            StageTask();
            completed = true;
        } );
    }
};

#endif /* InStage_h */
