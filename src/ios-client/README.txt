2nd implementation of iOS Leaderboard Client
--------------------------------------------

1. Uses LeaderboardService.thrift instead of DatabaseService.thrift
2. Reduces the number of request/reply for posting score, and getting top score by using single RPC.
3. Only three RPC calls for the leaderboard feature. 
