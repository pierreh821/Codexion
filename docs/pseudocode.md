# Pseudocode idea

```C
id = 1
while (id <= nb_coders):
    pthread_t coder // on cree un codeur (thread)
    int res = pthread_create(&coder, NULL, coder_activity, NULL)
```