//
//  OutStage.h
//  PipelineExmpl
//
//  Created by grigorii on 28/10/16.
//  Copyright © 2016 grigorii. All rights reserved.
//

#ifndef OutStage_h
#define OutStage_h

#include "Runnable.h"
#include "BlockingMessageQueue.h"
#include "Message.h"

class OutStage {
protected:
    Runnable rnbl;
    std::shared_ptr<MessageBMQType> bmq;
    std::shared_ptr<QueryBMQType> qbmq;
    bool completed;
    
public:
    void SetInBMQ( std::shared_ptr<MessageBMQType> bmq ) {
        this->bmq = bmq;
    }
    
    void SetOutQBMQ( std::shared_ptr<QueryBMQType> qbmq ) {
        this->qbmq = qbmq;
        if( !this->qbmq )
            std::cout << "lol\n";
    }
    
    bool TaskCompleted() {
        return completed;
    }

    virtual void StageTask( std::unique_ptr<Message> msg ) = 0;
    
    void Run() {
        completed = false;
        rnbl.Execute( [&] {
            auto query = std::make_unique<Query>( );
            qbmq->Push( std::move( query ) );
            
            auto msg = bmq->Pop();
            LastFlag f = msg->flag;
            StageTask( std::move( msg ) );
            if( f == LastFlag::on ) {
                completed = true;
            }
//            while( !completed ) {
//                auto msg = bmq->Pop();
//                LastFlag f = msg->flag;
//                StageTask( std::move( msg ) );
//                if( f == LastFlag::on )
//                    completed = true;
//            }
        } );
    }
};

#endif /* OutStage_h */
