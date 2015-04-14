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
