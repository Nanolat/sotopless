/*
 * StackDumper.cpp
 *
 *  Created on: 2013. 9. 16.
 *      Author: ilvsusie
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU
#define __USE_GNU
#endif

#define _XOPEN_SOURCE

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>

#include "Logger.h"
#include "StackDumper.h"

#include <map>

namespace nanolat {

#include <execinfo.h>

// The mapping from signal number to the original signal hander.
std::map<int, struct sigaction> original_signal_handlers;

// TODO : On windows, use StackWalker in as shonw in http://oroboro.com/stack-trace-on-crash/.
// This function is only for MAC OS X and Linux.
static inline void printStackTrace( FILE *out = stderr, unsigned int max_frames = 256 )
{
   fprintf(out, "stack trace:\n");

   // storage array for stack trace address data
   void* addrlist[max_frames+1];

   // retrieve current stack addresses
   unsigned int addrlen = backtrace( addrlist, sizeof( addrlist ) / sizeof( void* ));

   if ( addrlen == 0 )
   {
      fprintf( out, "  \n" );
      NL_LOG_FATAL( "stack trace: " << std::endl );
   } else {
	   // create readable strings to each frame.
	   char** symbollist = backtrace_symbols( addrlist, addrlen );

	   // print the stack trace.
	   for ( unsigned int i = 1; i < addrlen; i++ )
	      fprintf( out, "[%02d]%s\n", i, symbollist[i]);

	   for ( unsigned int i = 1; i < addrlen; i++ )
	      NL_LOG_FATAL( "["<< i <<"] "<< symbollist[i] );

	   free(symbollist);
   }
}

void abortHandler( int signum, siginfo_t* si, void* unused)
{
	// associate each signal with a signal name string.
	const char* name = NULL;
	switch( signum )
	{
		case SIGABRT: name = "SIGABRT";  break;
		case SIGSEGV: name = "SIGSEGV";  break;
		case SIGBUS:  name = "SIGBUS";   break;
		case SIGILL:  name = "SIGILL";   break;
		case SIGFPE:  name = "SIGFPE";   break;
		case SIGPIPE: name = "SIGPIPE";  break;
	}

	// Notify the user which signal was caught. We use printf, because this is the
	// most basic output function. Once you get a crash, it is possible that more
	// complex output systems like streams and the like may be corrupted. So we
	// make the most basic call possible to the lowest level, most
	// standard print function.
	if ( name )
		fprintf( stderr, "Caught signal %d (%s)\n", signum, name );
	else
		fprintf( stderr, "Caught signal %d\n", signum );

	// Dump a stack trace.
	// This is the function we will be implementing next.
	printStackTrace();

	// NL_LOG_FATAL itself may fail. use NL_LOG_FATAL at the end of the handler.
	if ( name )
		NL_LOG_FATAL( "Caught signal " << signum << "("<<name<<")" );
	else
		NL_LOG_FATAL( "Caught signal " << signum );

	// Call the original signal handler.
	struct sigaction sa = original_signal_handlers[signum];
	if (sa.sa_sigaction)
		sa.sa_sigaction(signum, si, unused);

	// If you caught one of the above signals, it is likely you just
	// want to quit your program right now.
	exit( signum );
}

void install_signal_handler( int signal ) {
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = abortHandler;
	sigemptyset( &sa.sa_mask );

	struct sigaction original_sa;

	if ( sigaction( SIGABRT, &sa, &original_sa ) != 0 ) {
		NL_LOG_FATAL("Failed to install signal handler. Signal : " << signal << ", errno : " << errno );
		exit(-1);
	}

	original_signal_handlers[signal] = original_sa;
}

StackDumper::StackDumper() {
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = abortHandler;
	sigemptyset( &sa.sa_mask );

	sigaction( SIGABRT, &sa, NULL );
	sigaction( SIGSEGV, &sa, NULL );
	sigaction( SIGBUS,  &sa, NULL );
	sigaction( SIGILL,  &sa, NULL );
	sigaction( SIGFPE,  &sa, NULL );
	sigaction( SIGPIPE, &sa, NULL );
}

StackDumper::~StackDumper() {
	// TODO Auto-generated destructor stub
}

} /* namespace nanolat */
