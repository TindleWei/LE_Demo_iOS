//
//  Melodify.m
//  LE_Demo_iOS
//
//  Created by tindle on 15/10/27.
//  Copyright (c) 2015年 tindle. All rights reserved.
//

#import "Melodify.h"

@implementation Melodify

- (id)init {
    if (self = [super init]) {
        melodify = new LE_Melodify();
    }
    return self;
}

- (void)startMelodify {
    melodify->startMelodify();
}

- (void)stopMelodify {
    melodify->stopMelodify();
}


@end