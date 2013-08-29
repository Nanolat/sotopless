SoTopless
=========
A leaderboard is a useful tool for games and gamified apps providing rankings of users based on score of each user.
The main goal of SoTopless leaderboard server is to support 100M users per server. 

For the details, see http://www.sotopless.com.

Supported Platforms
-------------------
*  OSX
*  Ubuntu

Prerequisites
-------------
*  wget is required to download boost library to build it in deps/build.sh script.
*  gcc 4.7.x is required to build and test SoTopless server and client.
*  OSX users using MacPorts need to upgrade gcc to 4.7.3_2 otherwise xUnit will crash at __once_proxy function. For the details see following link.
http://comments.gmane.org/gmane.os.apple.macports.user/32617

List of Features
----------------
Followings are list of leaderboard features to implement.

*  Post a new score of a user.
*  Get top N users with scores from a leaderboard.
*  Get rank and score of a user from a leaderboard.
*  Add different kinds of leaderboards such as weekly, monthly, asia, europe, etc.

License
-------
SoTopless is an open source software with AGPL v3 license. A commercial license which includes an Apache v2 license is available for project owners not willing to open their source codes.
