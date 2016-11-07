//
//  Message.h
//  PipelineExmpl
//
//  Created by grigorii on 27/10/16.
//  Copyright © 2016 grigorii. All rights reserved.
//

#ifndef Message_h
#define Message_h

#include <iostream>


enum class LastFlag {
    on,
    off
};

struct Message {
    std::vector<int> data;
    LastFlag flag;
    Message( int value = 0, int n = 500, LastFlag flag = LastFlag::off ) : data( n, value ), flag( flag ){ }
};

struct Query {
    LastFlag flag;
    
    Query( LastFlag f = LastFlag::off ) : flag( f ) { }
};


#endif /* Message_h */
