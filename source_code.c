#include <unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>

int num_of_items;
int no_items[1000]={0};
int cost_items[1000]= {0};
sem_t s;
int totexp=0,totinc=0;
void *produce(void* id)
{
    int x,t,choice=1;
    sem_wait(&s);
    int sum=0;
    printf("Producer id: %d\n",rand()%100);
    //printf("\n");
    while(choice==1)
    {
        printf("Enter id of item to produce: ");
        scanf("%d", &t);
        if(t>=num_of_items||t<0){
          printf("Item id does not exist\n");
        }
        else{
            int c=0;
            printf("At what unit cost will you produce this item: ");
            scanf("%d", &c);
            while(c<=0){
                printf("Enter positive cost :");
                scanf("%d", &c);
            }
            printf("Enter number of items to produce: ");
            scanf("%d", &x);
            while(x<=0){
                printf("Enter positive number of items :");
                scanf("%d", &x);
            }
            sleep(1);
            no_items[t]+=x;
            printf("Number of items produced: %d of id %d \n", x,t);
            sum+= x*c;
        }
        printf("Press 1 to produce more: ");
        scanf("%d",&choice);
        fflush(stdin);
    }
    printf("\n");
    printf("Inventory expenditure: Rs%d\n", sum);
    totexp+=sum;
    printf("\n");
    printf("Number of items of each type in inventory: \n");
    int i=0;
    for(i=0;i<num_of_items;i++){
        printf("id %d = %d \n",i,no_items[i]);
    }
    printf("\n\n");
    sem_post(&s);
}
void *consume(void* id)
{
    int y,t,choice=1,sum=0;
    sem_wait(&s);
    printf("Consumer id: %d\n",rand()%100);
    while(choice==1)
    {
        printf("Enter id of item to consume: ");
        scanf("%d", &t);
        if(t>=num_of_items||t<0){
            printf("Item id does not exist\n");
        }
        else if(no_items[t]==0)
        {
            printf("Sorry market is out of stock for this item\n");
        }
        else
        {
            printf("Enter number of items to consume: ");
            scanf("%d", &y);
            while(y<=0){
                printf("Enter positive number of items :");
                scanf("%d", &y);
            }
            sleep(1);
            if(y>no_items[t])
            {
                printf("Consuming all of the items of this type though demand cant be satisfied\n");
                y=no_items[t];
            }
            no_items[t]-=y;
            printf("Number of items consumed: %d of id %d \n", y,t);
            sum+=cost_items[t]*y;
        }

        printf("Press 1 to consume: ");
        scanf("%d",&choice);
        fflush(stdin);
    }
    printf("Inventory income: Rs%d\n",sum);
    totinc+=sum;
    printf("Number of items of each type in inventory: \n");
    int i=0;
    for(i=0;i<num_of_items;i++){
      printf("id %d = %d \n",i,no_items[i]);
    }
    printf("\n\n");
    sem_post(&s);
}

int main()
{
    printf("INVENTORY MANAGEMENT SYSTEM\nThis program manages all the transactions of the inventory!!\n\n");
    sem_init(&s, 0, 1);
    
    
    printf("Enter number of types of items to be available for marketing: ");
    scanf("%d",&num_of_items);
    while(num_of_items<=0){
        printf("Enter valid number of types of items : ");
        scanf("%d",&num_of_items);
    }
    printf("\n\n");
    
    
    printf("Set unit price of each type of item: \n");
    int i=0;
    for(i=0; i<num_of_items; i++) {
        printf("Unit price of item id %d : ",i);
        scanf("%d",&cost_items[i]);
        while(cost_items[i]<=0){
            printf("Enter valid Unit price of item id %d : ",i);
            scanf("%d",&cost_items[i]);
        }
    }
    printf("\n\n");
    
    
    int no_prod=0,no_cons=0;
    printf("Enter number of producers: ");
    scanf("%d",&no_prod);
    while(no_prod<=0){
        printf("Enter valid  number of producers: ");
        scanf("%d",&no_prod);
    }
    printf("Enter number of consumers: ");
    scanf("%d",&no_cons);
     while(no_cons<=0){
        printf("Enter valid  number of consumers: ");
        scanf("%d",&no_cons);
    }

    pthread_t consumer[no_cons], producer[no_prod];
    int m= 0,n= 0;
    printf("\n\n");


    printf("Items with their ids in inventory: \n");
    for(i=0;i<num_of_items;i++){
        printf("id %d = %d \tcost = Rs%d\n",i,no_items[i],cost_items[i]);
    }
    printf("\n\n");
   

    while(m<no_prod || n<no_cons)
    {
        if(m==0)
        {
               pthread_create(&producer[m], NULL, produce, &m);
               pthread_join(producer[m], NULL);
                m++;
        }
        if(m==no_prod)
        {
            pthread_create(&consumer[n], NULL, consume, &n);
            n++;
        }
        else if (n==no_cons)
        {
            pthread_create(&producer[m], NULL, produce, &m);
            m++;
        }
        else
        {
            int choice= rand()%2;
            if(choice==0)
            {
                pthread_create(&producer[m], NULL, produce, &m);
                m++;
            }
            else
            {
                pthread_create(&consumer[n], NULL, consume, &n);
                n++;
            }
        }
    }
    

    for(i=1; i<no_prod; i++) pthread_join(producer[i], NULL);
    for(i=0; i<no_cons; i++) pthread_join(consumer[i], NULL);
    

    printf("Final list of items in the inventory: \n");
    for(i=0; i<num_of_items; i++)
    {
        printf("id %d = %d \n",i,no_items[i]);
    }
    printf("\n\n");
    
    printf("Total expenditure of inventory: %d\n",totexp);
    printf("Total income of inventory: %d\n", totinc);
    if(totexp>totinc) printf("Loss: %d\n",totexp-totinc);
    else if(totexp<totinc) printf("Profit: %d\n", totinc- totexp);
    else printf("Net income = 0\n");
    return 0;
}
