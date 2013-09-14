#ifndef _NANOLAT_SERVER_H_
#define _NANOLAT_SERVER_H_ (1)

// Nanonat Database service
namespace nanolat {
    namespace server {
    	int listen(int port);
    }
}

// SoTopless leaderboard service
namespace nanolat {
    namespace leaderboard {
    	int listen(int port);
    }
}

#endif /* _NANOLAT_SERVER_H_ */
