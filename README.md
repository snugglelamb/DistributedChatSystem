### Low level communication demo
#### Zhi Li

=======
#### Update Log
  - 04/12/15
    - update stub_send
      - now stub_send could choose to send infinite times, send at most once and at least once
      - enable msg send queue by first enqueue msg before sending, and dequeue on receiving "OK" ack back
      - if not receiving "OK" (timeout), then will continue to resend SENDMAX(3) times iteratively and return "ERROR"
      - if receiving "RESEND" then keep sending until receiving "OK"
      - every time stub_send tries to resend, it first dequeue the current msg then enqueue a second time on reinitializing the same msg, however the id of the same msg may change.
    - update stub_receive
      - stub_receive could now check the size of packet received and compare with packet header
      - send "RESEND" request on receiving unintact msg
      - send "OK" ack on receiving full & intact msg
    - update stub_connect
      - now stub_connect use stub_send(Tip, Tport, msg, request = 1) to check if destination available by sending msg only once
      - will receive "ERROR" on not being able to connect
    - update stub_create
      - initialize msg send queue in stub_create
       
      
    - further update
      - handle duplicate msg
        - might need to maintain vector clock? 
      - enforce msg ordering on receiver side (possible?)
        - don't care what sequence the sender send (2,3,1 is OK), but receiver needs to get (1,2,3) then call high-level parser. 
  - 04/14/15
  - finished fault tolerance
    - cleanup the transmission between char*, const char* and string
    - datastructure change
      - add in struct clock_send
        - use targetIP:targetPORT as key to store how many valid messages have been sent to the target   
        - num will only update on sending non-resend messages(resend < 1)
        - use value of num(different among targets) as sequence number as part of msg header
      - add in struct msg_monitor
        - use receivedIP:receivedPORT as key mapping to sequence number received
        - a vector<int> holdback is used to store those sequence num that are way ahead of it's supposed to receive
        - latest is used as a classifier that seq received below latest will be defined as duplicate
        - latest only updates on receiving the msg with correct seq number(current latest+1)
        - on receving msg with correct seq number, it will then try to clear the member in holdback vector one by one incrementaly (e.g. if 3,4 are in holdback vector, now 2 arrives, it will then trigger the erase of 3 and 4, and update value of "latest" before erase), keep latest always the seq of latest valid msg received.
      -  update stub_receive to adapt the msg_monitor datastructure
        -  first check if data is intact
          -  if intact: reply "OK", then if sender already in monitor list, update seq num and holdback vector accordingly.
          -  if not intact: then simply reply "RESEND" as ack.
      -  update stub_send to adapt the clock_send datastructure
        -  first check if it's a resend request
        -  (either triggered by not intact resend request "RESEND" or resource temporarily unavailable)
          -  if resend (request >= 1) use same msg without adding new header to it
          -  if not resend (request == 0) 
            -  update sequence number accordingly, use the same sequence number for sending that msg in the header
            -  adding new header to it, format:   data size(fix length == 5) + IP:PORT + "STA" + sequence number(target specific) + "END@" (@ is for parser). Overall something like: 00045192.168.1.100:26411STA00002END@what's up
    - fix RESEND bug
      - used to send infinite times when data is not intact
      - now just send additional three times and return "ERROR" on the last run
      - Then there won't be any BUS ERROR
