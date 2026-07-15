# Pseudocode idea

```C
int id = 1;
int res;
*pthread_t coders;

coders = ft_calloc(nb_coders, sizeof(t_coder))


while (id <= nb_coders):
    pthread_t coder; // on cree un codeur (thread)

    res = pthread_create(&coder, NULL, coder_activity, NULL);

    coders[len(coders)] = coder

    if res:
        // handle error

    id++;

id = 1;
bool running = True;

while (running && id <= nb_coders):
    running = pthread_join(coders[id - 1]);
    id++;

free(coders)

```
