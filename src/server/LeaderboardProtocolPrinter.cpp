/*
 * ThriftProtocolPrinter.cpp
 *
 *  Created on: 2013. 9. 16.
 *      Author: ilvsusie
 */

#include "LeaderboardProtocolPrinter.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "LeaderboardService_types.h"


namespace nanolat {

std::string ScoreToString(const leaderboard::Score & score) {
	std::stringstream stream;
	stream << "(Score)["
		   << "value:"<< score.value
		   << ", date_epoch:" << score.date_epoch
		   << ", rank:" << score.rank
		   << ", user_alias:" << score.user_alias
		   << ", user_id:" << score.user_id
		   << ", vote_down_count:" << score.vote_down_count
		   << ", vote_up_count:" << score.vote_up_count
		   << "]";

	return stream.str();
}

std::string UserScoreAndTopScoresToString(const leaderboard::UserScoreAndTopScores &userScoreAndTopScores) {
	std::stringstream stream;

	stream << "(UserScoreAndTopScores)[" << std::endl;

	stream << "user_score:"<< ScoreToString( userScoreAndTopScores.user_score ) << std::endl;

	stream << "from_rank:"<<  userScoreAndTopScores.from_rank << ", count:" << userScoreAndTopScores.count << std::endl ;

	int i=0;
	for (std::vector<leaderboard::Score>::const_iterator it = userScoreAndTopScores.top_scores.begin();
		 it != userScoreAndTopScores.top_scores.end();
		 it++) {

		stream << "top_scores[" << i++ << "]:" << ScoreToString(*it) << std::endl;
	}

	stream << "]" << std::endl;

	return stream.str();
}

std::string ReplyStatusToString(const leaderboard::ReplyStatus & status) {
	std::stringstream stream;

	stream << "(ReplyStatus)[code:" << status.error_code
			            << ", error_message_format:" << status.error_message_format;

	int i=0;
	for (auto error_message_arg : status.error_message_args) {
		stream << ", error_message_args[" << i++ << "]:" << error_message_arg;
	}

	stream << "]";

	return stream.str();
}


} /* namespace nanolat */
