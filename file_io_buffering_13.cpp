/*
 * file_io_buffering_13.cpp
 *
 *  Created on: Dec 9, 2017
 *      Author: yng
 */


#include "common.h"
#include <vector>
#include <fstream>

/*
 *without osync, the speed is ok, with higher buf size, better performance
==============================
without osync, 10m file size:
1024 buffer size

real	0m0.277s
user	0m0.004s
sys	0m0.248s


2048 buffer size

real	0m0.192s
user	0m0.000s
sys	0m0.172s


4096 buffer size

real	0m0.167s
user	0m0.000s
sys	0m0.128s


8192 buffer size

real	0m0.134s
user	0m0.000s
sys	0m0.108s


16384 buffer size

real	0m0.163s
user	0m0.000s
sys	0m0.140s


==============================
==============================
without osync, 100m file size:
1024 buffer size

real	0m0.283s
user	0m0.000s
sys	0m0.260s


2048 buffer size

real	0m0.218s
user	0m0.000s
sys	0m0.196s


4096 buffer size

real	0m0.151s
user	0m0.000s
sys	0m0.132s


8192 buffer size

real	0m0.137s
user	0m0.000s
sys	0m0.124s


16384 buffer size

real	0m0.173s
user	0m0.000s
sys	0m0.148s


==============================






with osync, the speed is much slower, with higher buf size, better performance
==============================
with osync, 10m file size:
1024 buffer size

real	0m47.759s
user	0m0.012s
sys	0m9.588s


2048 buffer size

real	0m28.501s
user	0m0.008s
sys	0m5.772s


4096 buffer size

real	0m14.047s
user	0m0.012s
sys	0m3.144s


8192 buffer size

real	0m7.123s
user	0m0.008s
sys	0m1.576s


16384 buffer size

real	0m3.564s
user	0m0.000s
sys	0m0.760s


==============================

*/














void time_stats_13_1(int argc, char *argv[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;

	if (argc != 4 || strcmp(argv[1], "--help") == 0)
		throw EUsage("%s old-file new-file\n", argv[0]);
	/* Open input and output files */
	inputFd = open(argv[1], O_RDONLY | O_SYNC);
	if (inputFd == -1)
		throw EUsage("Opening file %s failed.", argv[1]);

	openFlags = O_CREAT | O_WRONLY | O_TRUNC | O_SYNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |	S_IROTH | S_IWOTH ; /* rw-rw-rw- */
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1)
		throw EUsage("Opening file %s failed", argv[2]);

	int buf_size = atoi(argv[3]);
	std::vector<char> buf_vec(buf_size);
	auto buf = buf_vec.data();

	/* Transfer data until we encounter end of input or an error */
	while ((numRead = read(inputFd, buf, buf_size)) > 0)
		if (write(outputFd, buf, numRead) != numRead)
			throw std::runtime_error("Couldn't write whole buffer to output file.");
	if (numRead == -1)
		throw std::runtime_error("Read does not finish.");
	if (close(inputFd) == -1)
		throw std::runtime_error("Close input failed.");
	if (close(outputFd) == -1)
		throw std::runtime_error("Close output failed.");
	return;

}








/* result shows that larger buffer size, within a certain range, increases the performance
yng@ubuntu:~/eclipse-workspace/Practices_Linux_Programming/Release$ sudo ./compare_13_2.sh
Exercise 13-2 compare bytes writing with different buf size and different file size
==============================
10m file size:
1024 buffer size
./compare_13_2.sh: line 14: fg: no job control

real	0m0.027s
user	0m0.000s
sys		0m0.008s


2048 buffer size
./compare_13_2.sh: line 14: fg: no job control

real	0m0.013s
user	0m0.000s
sys		0m0.008s


4096 buffer size
./compare_13_2.sh: line 14: fg: no job control

real	0m0.025s
user	0m0.000s
sys		0m0.004s


8192 buffer size
./compare_13_2.sh: line 14: fg: no job control

real	0m0.019s
user	0m0.000s
sys		0m0.012s


16384 buffer size
./compare_13_2.sh: line 14: fg: no job control

real	0m0.023s
user	0m0.004s
sys		0m0.004s


==============================
==============================
100m file size:
1024 buffer size
./compare_13_2.sh: line 31: fg: no job control

real	0m0.030s
user	0m0.000s
sys	0m0.016s


2048 buffer size
./compare_13_2.sh: line 31: fg: no job control

real	0m0.016s
user	0m0.000s
sys	0m0.008s


4096 buffer size
./compare_13_2.sh: line 31: fg: no job control

real	0m0.035s
user	0m0.000s
sys	0m0.008s


8192 buffer size
./compare_13_2.sh: line 31: fg: no job control

real	0m0.028s
user	0m0.000s
sys	0m0.012s


16384 buffer size
./compare_13_2.sh: line 31: fg: no job control

real	0m0.034s
user	0m0.000s
sys	0m0.008s


 */









/* write_bytes.c

   Write bytes to a file. (A simple program for file I/O benchmarking.)

   Usage: write_bytes file num-bytes buf-size

   Writes 'num-bytes' bytes to 'file', using a buffer size of 'buf-size'
   for each write().

   If compiled with -DUSE_O_SYNC, open the file with the O_SYNC flag,
   so that all data and metadata changes are flushed to the disk.

   If compiled with -DUSE_FDATASYNC, perform an fdatasync() after each write,
   so that data--and possibly metadata--changes are flushed to the disk.

   If compiled with -DUSE_FSYNC, perform an fsync() after each write, so that
   data and metadata are flushed to the disk.
*/

void write_bytes_13_2(int argc, char *argv[])
{
    size_t bufSize, numBytes, thisWrite, totWritten;
    char *buf;
    int fd, openFlags;

    if (argc != 4 || strcmp(argv[1], "--help") == 0)
        throw EUsage("%s file num-bytes buf-size\n", argv[0]);

    numBytes = atoi(argv[2]);
    bufSize = atoi(argv[3]);
    std::vector<char> buf_vec(bufSize);
    buf = buf_vec.data();

    if (buf == NULL)
        throw std::runtime_error("malloc");

    openFlags = O_CREAT | O_WRONLY;

#if defined(USE_O_SYNC) && defined(O_SYNC)
    openFlags |= O_SYNC;
#endif

    fd = open(argv[1], openFlags, S_IRUSR | S_IWUSR);
    if (fd == -1)
        throw std::runtime_error("open");

    for (totWritten = 0; totWritten < numBytes; totWritten += thisWrite)
    {
        thisWrite = std::min(bufSize, numBytes - totWritten);

        if (write(fd, buf, thisWrite) != thisWrite)
        	std::runtime_error("partial/failed write");

#ifdef USE_FSYNC
        if (fsync(fd))
            errExit("fsync");
#endif
#ifdef USE_FDATASYNC
        if (fdatasync(fd))
            errExit("fdatasync");
#endif
    }

    if (close(fd) == -1)
        throw std::runtime_error("Could not close.");
    return;
}



/*
 *
 *
Exercise 13-3
What is the effect of the following statements?
fflush(fp);
fsync(fileno(fp));

Answer:
fflush works on the stream buffer in user space, it transfer the buffer in user space to the system space
fsync works on a lower level, it does not know the stream. It makes sure that the content in system space goes inside the physical media.
For a further discussion: see  https://stackoverflow.com/questions/2340610/difference-between-fflush-and-fsync

Therefore:
 fflush(fp)
 fsync(fileno(fp))
working together makes sure that the file stream goes to the physical media.

*/



/*
 *
so if when run to stdout

yng@ubuntu:~/eclipse-workspace/Practices_Linux_Programming/Release$ ./Practices_Linux_Programming
If I had more time,
I would have written you a shorter letter.


so if run to file
yng@ubuntu:~/eclipse-workspace/Practices_Linux_Programming/Release$ cat res.txt
I would have written you a shorter letter.
If I had more time,


The reason for this is that the printf operation:
When going to stdout, default to mode:
	_IOLBF
	Employ line-buffered I/O. This flag is the default for streams referring to
	terminal devices. For output streams, data is buffered until a newline character is output (unless the buffer fills first). For input streams, data is read
	a line at a time.
So it will be printed out the terminal after it sees the line break;

when going to file, default to mode:
	_IOFBF
	Employ fully buffered I/O. Data is read or written (via calls to read() or
	write()) in units equal to the size of the buffer. This mode is the default for
	streams referring to disk files.

	So it will only be printed to file when the input buffer is exhausted or when the program exits.

 *
 *
 */

void different_effect_redirection_13_4()
{
	printf("If I had more time, \n");
	write(STDOUT_FILENO, "I would have written you a shorter letter.\n", 43);
}




void tail_13_5(int argc, char *argv[])
{
   EUsage err = EUsage{"exe [-n num] file"};
   auto params_pack = Params{1, argc, argv};
   if(params_pack.IfHelp())
	   throw err;


   std::string file_name;
   int nlines_to_read{10};
   if(params_pack.IfOption("-n"))
   {
	   if(params_pack.args.size() < 3)
		   throw err;
	   file_name = params_pack.args[2];
	   auto nlines_to_read_tuple = params_pack.FindIntWithOption("-n");
	   if(!std::get<0>(nlines_to_read_tuple) || !std::get<1>(nlines_to_read_tuple))
		   throw err;
	   nlines_to_read = std::get<2>(nlines_to_read_tuple);
   }
   else
   {
	   if(params_pack.args.size() < 1)
		   throw err;
	   file_name = params_pack.args[0];
   }

   //open the file
   std::ifstream read_file{file_name};
   if(!read_file.is_open())
	   throw std::runtime_error{"Could not open file" + file_name};

   //seek directly to the end of file

   read_file.seekg(-1, read_file.end);
   int size = read_file.tellg();
   std::vector<char> buf;
   buf.reserve(5000);
   int nlines{};
   for(int i = 1; i < size; i++)
   {
	   char ch;
	   read_file.seekg(-i,read_file.end);
	   read_file.get(ch);
	   if(ch == '\n')
		   ++nlines;
	   if(nlines > nlines_to_read)
		   break;
	   buf.emplace_back(ch);
   }

   for(auto it = buf.rbegin(); it != buf.rend(); ++it)
	   std::cout << *it;
}
