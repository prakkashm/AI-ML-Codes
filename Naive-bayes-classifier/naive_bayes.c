#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int row=6670,rowt=3333;
int calc_P(int class,int *test_set,int **training_set)
{
        // int i,j=0;
        // while(j!=193){
        //     printf("%d", test_set[i][j]);
        //     j++;
        // }
        // printf("\n");
    float tot=0,ind_cnt[192];
    int i,j;
    for(i=0;i<192;i++)
    {
        ind_cnt[i]=0;
    }
    for(i=0;i<row;i++)
    {
        // printf("%d,%d\n",training_set[i][192],class );
        if(training_set[i][192] == class)
        {
            tot+=1;
            for(j=0;j<192;j++)
            {
                if(training_set[i][j]==test_set[j])
                {
                    ind_cnt[j]+=1;
                }
            }
        }
    }
    // printf("%f\n",tot);
    // for(i=0;i<192;i++)
    // {
    //     printf("%f ",ind_cnt[i]);
    // }
    // printf("\n");
    float prob = 0;
    for(i=0;i<192;i++)
    {
        prob += log(ind_cnt[i]/tot);
    }
    // printf("%f\n",  prob );
    return prob;
}
void bayes(int **training_set, int **test_set)
{
    int i,j,z;
    float errcnt = 0;
    // calc_P(0,test_set[0],training_set);
    for(z=0;z<rowt;z++)
    {
        int max = log(0);
        float class;
        for(i=0;i<=9;i++)
        {
            int prob = calc_P(i,test_set[z],training_set);
            if(prob>max)
            {
                max = prob;
                class = i;
            }
        }
        if(class != test_set[z][192])
        {
                errcnt+=1;
        }
        // printf("CLASS %d\n",class)
    }
    printf("Number of wrongly classified data: %f\n", errcnt);
    printf("ACCURACY : %f %%\n", (1-errcnt/rowt)*100);
}
int main()
{
    FILE* train = fopen("pp_tra.dat","r");
    FILE* test = fopen("pp_tes.dat","r");
    int i=0,j=0,cls=10,feature=5,col=193;
    int **training_set;
    int **test_set;
    training_set = (int**)malloc(row*sizeof(int*));
    test_set = (int**)malloc(row*sizeof(int*));
    for(i=0;i<row;i++)
        training_set[i] = (int*)malloc(col*sizeof(int));
    for(i=0;i<rowt;i++)
        test_set[i] = (int*)malloc(col*sizeof(int));
    for(i=0;i<row;i++)
    {
        j=0;
        while(j!=193){
            fscanf(train,"%d",&training_set[i][j]);
            j++;
        }
    }
    for(i=0;i<rowt;i++)
    {
        j=0;
        while(j!=193){
            fscanf(test,"%d",&test_set[i][j]);
            j++;
        }
    }
    bayes(training_set,test_set);
}
