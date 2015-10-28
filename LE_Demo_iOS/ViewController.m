//
//  ViewController.m
//  LE_Demo_iOS
//
//  Created by tindle on 15/10/27.
//  Copyright (c) 2015年 tindle. All rights reserved.
//

#import "ViewController.h"
#import <MediaPlayer/MediaPlayer.h>

@interface ViewController (){
    Melodify *melodify;
    NSThread *myThread;
}

@property (assign,nonatomic) BOOL isStarting;

@end

@implementation ViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.isStarting = NO;
    melodify = [[Melodify alloc] init];
    myThread = [[NSThread alloc] initWithTarget:self selector:@selector(startMelodify) object:nil];
}

- (void)startMelodify{
    [melodify startMelodify];
}

- (IBAction)startClick:(id)sender event:(UIEvent *)event {
    NSLog(@"start");
    if (self.isStarting) {
        return;
    }
    self.isStarting = YES;
    if ([myThread isCancelled]) {
        myThread = [[NSThread alloc] initWithTarget:self selector:@selector(startMelodify) object:nil];
    }
    [myThread start];

}

- (IBAction)stopClick:(id)sender event:(UIEvent *)event {
    NSLog(@"stop");
    self.isStarting = NO;
    
    [melodify stopMelodify];
    [myThread cancel];
}

- (IBAction)restartClick:(id)sender event:(UIEvent *)event {
    NSLog(@"restart");
    self.isStarting = NO;
}

- (IBAction)playAction:(id)sender {
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString *imageDir = [[paths objectAtIndex:0] stringByAppendingPathComponent:@"MelodifyedSpeech.wav"];
    NSLog(@"%@",imageDir);
    
    MPMoviePlayerViewController *controller = [[MPMoviePlayerViewController alloc] initWithContentURL:[NSURL fileURLWithPath:imageDir]];
    [self presentMoviePlayerViewControllerAnimated:controller];
}

@end
