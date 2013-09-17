/*
 * ThriftProtocolPrinter.h
 *
 *  Created on: 2013. 9. 16.
 *      Author: ilvsusie
 */

#ifndef _NANOLAT_LEADERBOARD_PROTOCOL_PRINTER_H_
#define _NANOLAT_LEADERBOARD_PROTOCOL_PRINTER_H_ (1)

#include <string>
#include "LeaderboardService_types.h"

namespace nanolat {

std::string ScoreToString(const leaderboard::Score & score) ;

std::string UserScoreAndTopScoresToString(const leaderboard::UserScoreAndTopScores &userScoreAndTopScores) ;

std::string ReplyStatusToString(const leaderboard::ReplyStatus & status) ;

} /* namespace nanolat */

#endif /* _NANOLAT_LEADERBOARD_PROTOCOL_PRINTER_H_ */
