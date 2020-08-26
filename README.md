# Producer_Consumer_Problem
The producer-consumer problem is a classic whose solutions make it possible to resolve the cooperation of processes, within the scope of concurrent programming.
Two tasks share a limited size buffer. One of the tasks places data in the buffer (producer) and another one removes the data from the buffer (consumer). This problem can 
be generalized, considering several producers and consumers. The problems that arise, in addition to concurrent access to the shared buffer, are:
• The placement of elements when the buffer is full, which implies that the producer falls asleep and is awakened only when there is space
• The withdrawal of elements, when the buffer is empty, which implies that the consumer falls asleep and is awake when elements are available.
In this project, it is interesting to focus on solutions based on the support of the operating system, specifically on the study of the weight (overhead) of the facilities
implemented in the user space vs the core space.
