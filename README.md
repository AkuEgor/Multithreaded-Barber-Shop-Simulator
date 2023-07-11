**Project Description: Barber Shop Simulator**

Zaawansowane systemy operacyjne / Advanced Operating Systems

#The barber shop simulation project aims to create a safe and multithreaded program in C or C++ that emulates the operations of a barber shop.

**Scenario:**
- The barber shop has three seats, three barbers, a sofa for four people, and a waiting room.
- The facility can accommodate a maximum of 20 people at a time, and clients will not enter if the facility is full.
- Clients either occupy a free seat on the sofa or wait in the waiting room if the sofa is occupied.
- When a barber becomes available, the client who has been sitting on the sofa for the longest time is chosen for a haircut, along with the client who has been waiting in the waiting room for the longest time.
- After receiving a haircut, the client needs to pay the fee at the cashier's office, which is operated by the currently free barber.
- The barbers divide their time between cutting clients, sleeping (in case of no clients), and cashier's service.
- The program will simulate the operations until 50 people have been serviced.

**Program Requirements:**
- Develop a safe and multithreaded program in C or C++.
- Implement parameterization and testing capabilities, with relevant messages logged to the console.
- Test the program using the Valgrind tool to check for memory leaks, thread blocking, and other issues.
- Utilize synchronization mechanisms, mainly focusing on mutexes and condition variables.
- Provide options to enable/disable console logging and artificial delays for clear visibility of the logging output.
