Setup up enviroment:
cd lab2
export PATH="/home/min/a/ece695x/labs/common/dlxos/bin:$PATH"
export LD_LIBRARY_PATH=/home/min/a/ece695x/labs/common/gcc/lib

Make:
Make OS first before each app. Change os/Makefile to appropriate question before making and running it. 'make clean' OS before each remake it for a new app.
mainframer.sh 'cd os && make'
mainframer.sh 'cd apps/example && make'
mainframer.sh 'cd apps/q2 && make'
mainframer.sh 'cd apps/q3 && make'
mainframer.sh 'cd apps/q4 && make'
mainframer.sh 'cd apps/q5 && make'

Run:
cd os && make run && cd ..
