//
//  ViewController.m
//  objc
//
//  Created by Kangmo Kim on 13. 9. 7..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//


#import "ViewController.h"

#import "NLSoTopless.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    ////////////////////////////////////////////////////////////////////////////////
	// Login
    
    NSString * uuid = [ NSString stringWithFormat:@"abcde"];
    
    NLLocalPlayer * localPlayer = [ NLLocalPlayer localPlayer:uuid playerAlias:@"test alias" password:uuid ];
    
    localPlayer.authenticateHandler = ^(NSString * connectedServerName, NSError * error) {
        if (error != NULL) {
            if ( error.code == NLErrorIncompatibleClientVersion) {
                NSLog( @"Error : Client upgrade required. : URL => %@", error.localizedDescription );
            }
            
            NSLog( @"Error : while authenticating : %@", error.description );
            return;
        }
        
        NSLog( @"Success : Connected to %@. User authenticated.", connectedServerName);
        
        ////////////////////////////////////////////////////////////////////////////////
        // Report Score
        NLScore * myScore = [[NLScore alloc] initWithCategory:@"all"];
        [myScore increaseValue:1 reason:@"Got apple"];  // increase score with reason of increment
        [myScore increaseValue:3 reason:@"Got orange"]; // increase score with reason of increment
        [myScore increaseValue:2 reason:@"Got water"];  // increase score with reason of increment
        
        [myScore reportScoreWithCompletionHandler:^(NSError * error) {
            if (error != NULL) {
                NSLog( @"Error : while reporting score : %@", error.description );
                return;
            }
            NSLog( @"Success : Score reported.");
            
            ////////////////////////////////////////////////////////////////////////////////
            // List top 10 Scores
            NLLeaderboard * leaderboard = [[NLLeaderboard alloc] init];
            leaderboard.category = @"all";
            leaderboard.range = NSMakeRange(1,10); // 10 users from the 1st user.
            
            [leaderboard loadScoresWithCompletionHandler:^(NSArray* scores, NSError* error) {
                if (error != NULL) {
                    NSLog( @"Error : while loading scores : %@", error.description );
                    return;
                }
                NSLog( @"Success : Scores loaded.");
                
                NSLog( @"User Score -> Rank: %d, Display Alias : %@, Score : %lld",
                      leaderboard.localPlayerScore.rank, leaderboard.localPlayerScore.playerAlias, leaderboard.localPlayerScore.value );
                
                NSLog( @"Listing scores." );
                
                for (NLScore * score in scores) {
                    NSString *dateString = [NSDateFormatter localizedStringFromDate:score.date
                                                                          dateStyle:NSDateFormatterShortStyle
                                                                          timeStyle:NSDateFormatterFullStyle];
                    
                    NSLog( @"Rank: %d, Display Alias : %@, Score : %lld, Time : %@",
                          score.rank, score.playerAlias, score.value, dateString );
                }
            }];
            
        }];

    };
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
