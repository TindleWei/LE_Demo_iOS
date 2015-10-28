//
//  Melodify.h
//  LE_Demo_iOS
//
//  Created by tindle on 15/10/27.
//  Copyright (c) 2015年 tindle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LE_Melodify.mm"


@interface Melodify: NSObject {
@private
    LE_Melodify *melodify;
}

- (id)init;
- (void)startMelodify;
- (void)stopMelodify;

@end
