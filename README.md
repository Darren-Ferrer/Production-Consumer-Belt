# Production-Consumer-Belt

Uses semaphores to create a conveyor belt that produces and consumes 100 items.

Required Arguments:
  * only needs one of the required arguments to run. All others will be initialized to 0 milliseconds*
    -E N: sets milliseconds N for the consumer Ethel to put a candy in a box
    -L N: sets milliseconds N for the consumer Lucy to put a candy in a box
    -f N: sets milliseconds N to produce a crunchy frog bite item
    -e N sets milliseconds N to produce an escargot sucker item

 Example:
  ./mizzo -f 5 -e 15 -E 35 -L 20  
