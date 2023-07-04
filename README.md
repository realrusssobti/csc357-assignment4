# 357 Assignment 4
Russ Sobti,  <rsobti@calpoly.edu>
# Functional Overview
Your program will process a file containing the URLs for the files to be downloaded (an example file is provided in the assignment repository). The actual downloading will be handled by the curl program which be installed in your development environment with the command:
```sudo apt-get install curl```
###  curl
The curl program is a command-line tool that can be used to download files specified by URLs. This program allows for configuration using a truly incredible number of command-line options, but we will not consider the vast majority of these options. Instead, your program will use curl in the following manner (with the options described below).

curl -m <seconds> -o <filename> -s <url>
-m <seconds> — This option limits the download time to the specified number of seconds.
-o <filename> — This option specifies the name of the output file to which the contents of the download file will be written.
-s — This option silences the progress meter typically printed by curl.
<url> — This is the URL of the file to download.


##  Requirement 1
Your program must be named a4download take two command-line arguments, in this order:

The name of the file (discussed below) containing the downloads to be processed.
An integer indicating the maximum number of downloading processes that may be executing at one time (this is so that the user can control the load from this program). Your program must verify that the arguments represent a valid file and a valid positive integer.
For example, your program should run as follows:

./a4download test_urls.txt 3



## Requirement 2
Each line of the provided file will contain the information relevant to a file to download, in this order:

Output file name,
URL
An optional maximum number of seconds for the download.
These values will be separated by a space character. You may assume that the output file name is valid and does not include any directory separators. You may also assume that the URL is a valid HTTP(s) URL. The maximum number of seconds, if provided, will be an integer value > 0.

For example, the line:

file1 https://csc.calpoly.edu/wp-content/uploads/2020/11/cal-poly-logo-reverse.svg 2
Should cause your program to download the content at URL https://csc.calpoly.edu/wp-content/uploads/2020/11/cal-poly-logo-reverse.svg, into a local file named file1, waiting a maximum of 2 seconds

## Requirement 3
Your program must spawn a single process per download request, but only up to the (command-line argument) specified maximum number of (child) processes that may exist at one time. Each of these processes will execute the appropriate curl command with the specified output file, url, and, if appropriate, maximum time.

If there are more download requests than the maximum number of allowed processes, then additional requests must be satisfied as each process completes. It is not sufficient to spawn only a single process at a time; multiple processes (up to the specified limit) must be used.



## Requirement 4
The primary (parent) process should print when a download request is starting and when it ends. The output (when starting and when ending) must refer to each download request by the line number in the provided file and the child process id (e.g., process 3287 processing line #1). If one of the curl processes terminates abnormally, then the primary process must print an error message indicating such.



## Requirement 5
Your program must properly manage resources to avoid running out of processes.



## Deliverables
Source Code (and Makefile) - Push all relevant source code and an appropriate Makefile to your repository.
During testing, the autograder creates files/directories with the prefix: ag_ Please make sure your repository does not include files or directories named with this prefix with these names.
Please do not commit compiled/executable files to git (ie. do not add: a.out, a4download (the executable file, without an extension), main.o, etc.)
For full credit, your git commit history must be sufficiently detailed to illustrate how you approached the assignment, addressed bugs, etc. Commit often, with descriptive commit labels. For example: "initial data structures and function prototypes", "implemented the -a switch to display hidden files", "solved resource leak in function xyz()"  There is no fixed minimum number of commits. As one example guideline, you will not be eligible for full credit if your submitted git repository includes just 2-3 commits where large blocks of code are added in each commit.
Rubric