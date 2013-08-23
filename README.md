sotopless
=========

A ranking server supporting 100M users per server. REST service is also provided.
For the details, see http://www.sotopless.com.


Prerequisites
-------------
*  libevent is required to include non-blocking server into thrift.
*  wget is required to download boost library to build it in deps/build.sh script.
*  gcc 4.7.x is required to build and test SoTopless server and client.
*  OSX users using MacPorts need to upgrade gcc to 4.7.3_2 otherwise xUnit will crash at __once_proxy function. For the details see following link.
http://comments.gmane.org/gmane.os.apple.macports.user/32617

License
-------
SoTopless is an open source software with AGPL v3 license. A commercial license which includes an Apache v2 license is available for project owners not willing to open their source codes.

