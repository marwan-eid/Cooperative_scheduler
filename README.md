# Cooperative_scheduler
This is a repo for the first project of the Embedded Systems course. In addition to the cooperative scheduler, two small applications are developed to demonstrate it.
## Queue Implementation
### Data Structures
#### Struct task
This struct contains type definition pointer to a function and two unsigned integer priority and delay.
#### Struct Task Queue
This struct has two unsigned integers one indexing the task to be dequeued (ptr) and the other is the maximum size of the queue. The third attribute is a pointer to an array of tasks. 
### Functions
- #### enqueueReady()
This function checks if the first queue has any elements and if it has, it checks whether their delay is zero or not and then dequeue them and enqueuer them to the other queue (ready queue) in case the delay is zero. 
- #### decrementDelay()
This function loops over the tasks in the delay queue to decrement them by one.
- #### Comp()
This function compare the delay and the priority of the two passed tasked and return one if the first task should be placed before the second task.
- #### Swap()
This function swaps two tasks.
- #### initQueue()
This function initializes the queue by setting its size to the wanted size, the task index to zero and initializes the array of tasks to point to dummy tasks.
- #### enqueueFun()
This function takes a queue, a function pointer, a priority and a delay. It declares a variable of type task and gives it the last three inputs (multiplying the delay by 50) and then passes the task along with queue to the function enqueue.
- #### enqueue()
This function returns if the queue is full otherwise, initializes the next empty space in the array and increment the index (ptr) by one. It then calls the function compare and checks if the recently added task should be swapped with the task added before it or not.
- #### Rearranging()
This function deletes the task on the top of the queue by moving the next task to its place. It also loops over the rest of the tasks and does the same thing making the queue shifts forward by one.
- #### Dequeue()
This function checks if the queue is not empty, it decrements the array index (ptr) by one, calls rearrange and then returns the task on the top of the queue.

#### The next functions are queue related but were placed in the main.

- #### Dispatch()
This function calls decrement delayed queue and then enqueue ready tasks to the main queue. It also checks if there are tasks if the ready queue. If there are, it dequeue them and calls the function of the task to be implemented. 
- #### reRunME()
This function takes function pointer, priority and delay checks if the delay if there, it calls the function enqueueFun and pass to it the ready queue.  If the delay is more than zero, it calls the same function but passes the delay queue.
- #### Init()
This function calls the function initQueue and passes to it the ready queue and the delay queue along with the size.
## Queue Testing
Three functions were implemented first, second and third. They all transmit a message to the UART containing their names. The first and the third function have reRunME inside them. Two queues were declared (readyQueue & delayedQueue). In the main, init was called then the three functions were enqueued by passing their pointer, priority and delay to enqueueFun. Inside the while loop, dispatch was called.
## Ambient temperature monitor
This is an application that was developed on NUCLEO-L432KC MCU board to demonstrate the scheduler through reading temperature via a temperature sensor and toggling a LED if the temperature exceeds a certain threshold set by the user.
### Functions
- ## read_temp()
This function reads the temperature via I2C and forwards it to a UART interface to be displayed on TeraTerm.
- ## check_threshold()
This function reads a threshold temperature from the user through a text command sent over TeraTerm, checks the if the current temperature is above this threshold, and, if so, signals to toggle a LED.
- ## toggleLED()
This function toggles an on board LED.

## Parking Sensor
This is an application that was developed on NUCLEO-L432KC MCU board to demonstrate the scheduler through calculating the distance between the sensor and the nearest object and producing peeps from a buzzer reflecting this distance.
[App 2 Testing Video](https://drive.google.com/file/d/1HjxjuKk-F1VdVskpovXKFzAauXyghymI/view?usp=sharing)
### Functions
- ## buzzer()
This function checks if the measured distance is within a certain threshold. If so, it toggles the buzzer pin and dispatches itslef using ReRunMe() to run again after a certain delay relative to the distance; if not, the function stops the buzzer and reruns again after 10 ticks to repeat the process with the new distance.
- ## HCSR04_read()
This function pulls the TRIG pin of the ultrasound sensor HIGH for 10us, then it pulls it low and recieves data from the ECHO pin. The function waits till the ECHO pin is set high and signals a flag to start counting the time till the next falling edge; time is the difference between both timestamps, and the distance is calculated by multiplying the time by the speed of sound and dividing it by 2. The function reruns itself to continuously read from the sensor and update the distance.
- ## timer()
This function increments the time to be used for the HCSR04_read() function.

https://drive.google.com/file/d/1HjxjuKk-F1VdVskpovXKFzAauXyghymI/view?usp=sharing
## License
MIT
