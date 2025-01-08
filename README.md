# encrypted-chat-application

This encrypted chat application is written in C/C++.

There are two ways to approaching this application with regards to using threads:
1. One thread per connection
   - This approach would create one thread per user connection. This approach is good when the server expects low number of user connections. If there were a significant number of people connecting and using the server, the high number of threads could use up all of the CPU resources, making the server significantly slower or crashing the server.

2. Threading for tasks
   - In this approach, instead of creating a new thread each time a user connects to the server, we use threads to manage different tasks at the same time. This approach is more scalable than the one thread per connection approach, since a high number of connection doesn't cause the server to slow down or crash the server. 

So far, this project is programmed on the idea of one thread per connection. While the server is listening for requests, a new thread will be created for a new connection each time. I decided to go with this approach since it is easier to implement. This might change in the future if I decide to implement another approach. 

In this chat application, once a user connects, this user can broadcast messages across the server. It's like a public chat room. Features I eventually want to implement in the future:
- User is allowed to have private conversations with another user
- Make this application end-to-end encrypted, so the server is unable to see messages the users send (increse privacy)
