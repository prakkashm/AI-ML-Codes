#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

/******************* R E A D I N G   F R O M   F I L E ***************************/

int trainTotal = 6670;
int testTotal = 3333;
int count = 193;
float **distanceBetween = NULL;
float **distanceBetween2 = NULL;

int **readFromDAT(char filename[],int totalLines)
{
	int **ar = (int**)malloc(totalLines*sizeof(int*));
	FILE *f = fopen(filename,"r");
	if(f){
		char buf[500];
		char *str;

		int i=0;
		while(fgets(buf, sizeof(buf), f)){          //reads a single row in the file
			str = (char *)strtok(buf," ");          //reads first space separated token of the row
			ar[i]=(int *)malloc(count*sizeof(int));
			int j = 0;
			while(str!=NULL){                       //reads all space separated tokens of the first line
				ar[i][j++]=atoi(str);
				str = (char *)strtok(NULL," ");
			}
			i++;
		}
	}
	return ar;
}

void readFromTXT(char filename[],int totalLines, int columns,float ***abc)
{
	*abc = (float**)malloc(totalLines*sizeof(float*));
	FILE *f = fopen(filename,"r");
	if(f){
		char buf[100000];
		char *str;
		/*int count = 0;
		fgets(buf, sizeof(buf), f);
		str = (char *)strtok(buf," ");
		while(str!=NULL){
			count++;
			str = (char *)strtok(NULL," ");
		}*/

		int i=0;
		while(fgets(buf, sizeof(buf), f)){          //reads a single row in the file
			str = (char *)strtok(buf," ");          //reads first space separated token of the row
            (*abc)[i]=(float *)malloc(columns*sizeof(float));
			int j = 0;
			while(str!=NULL){                       //reads all space separated tokens of the first line
				(*abc)[i][j++]=atof(str);
				str = (char *)strtok(NULL," ");
			}
			i++;
		}
	}
}

/**************************************************************************/

typedef struct{
    float distance;
    int index;
}distanceNode;

float knn1(int **arr, int l, int h, int k){
    int i,j,p,z,count1=0,totalExamples,max;
    float error,d;
    distanceNode array[k];
    totalExamples = h-l+1;
    for(i=l;i<=h;i++){
        for(p=0;p<k;p++){
            array[p].distance = -1;
            array[p].index = -1;
        }
        z=0;
        for(j=0;j<trainTotal;j++){
            if(j==l)                    // even j==l is sufficient
                j = h+1;
            if(j<trainTotal){                   // it's possible that j = h+1 = trainTotal
                d = distanceBetween[i][j];
                if(z>=0 && z<=k-1){
                    array[z].distance = d;
                    array[z++].index = j;
                }
                else{
                    for(p=1,max=0;p<k;p++){
                        if(array[max].distance<array[p].distance)
                            max = p;
                    }
                    if(d<array[max].distance){
                        array[max].distance = d;
                        array[max].index = j;
                    }
                }
            }
        }


        int count2[10] = {0};
        for(p=0;p<k;p++)
            (count2[arr[array[p].index][192]])++;

        for(p=1,max=0;p<10;p++){
            if(count2[max]<count2[p])
                max=p;
        }

        // max now contains the label occurring maximum no. of times among all the k-nearest neighbors
        if(max==arr[i][192])
            count1++;
    }

    error = 100.0 - ((float)(count1*100)/(totalExamples));
    return error;
}

int threeFoldValidation(int **arr){
    int i=1,j=0,k,low,high,m=25;
    float error[4][m+1];

    // 1st partition till 6670/3 = 2223
    low = 0;
    high = (trainTotal/3) - 1;
    printf("\nP R O C E S S I N G   V A L I D A T I O N   S E T   %d . . .\n",i);
    for(k=1;k<=m;k++){
        error[i][k] = knn1(arr,low,high,k);
        //printf("knn completed for partition %d, k = %d\n",i,k);
        //printf("Error Calculated for partition %d, k = %d is : %f\n\n",i,k,error[i][k]);
    }

    // 2nd partition till 2*6670/3 = 4446
    i++;
    low = high + 1;
    high = (2*trainTotal/3) - 1;
    printf("\nP R O C E S S I N G   V A L I D A T I O N   S E T   %d . . .\n",i);
    for(k=1;k<=m;k++){
        error[i][k] = knn1(arr,low,high,k);
        //printf("knn completed for partition %d, k = %d\n",i,k);
        //printf("Error Calculated for partition %d, k = %d is : %f\n\n",i,k,error[i][k]);
    }

    // 3rd partition till 6670
    i++;
    low = high + 1;
    high = trainTotal - 1;
    printf("\nP R O C E S S I N G   V A L I D A T I O N   S E T   %d . . .\n",i);
    for(k=1;k<=m;k++){
        error[i][k] = knn1(arr,low,high,k);
        //printf("knn completed for partition %d, k = %d\n",i,k);
        //printf("Error Calculated for partition %d, k = %d is : %f\n\n",i,k,error[i][k]);
    }

    float e[26];
    for(k=1;k<=m;k++){
        e[k] = (error[1][k]+error[2][k]+error[3][k])/3.0;
        printf("\nAverage Error for k = %d is : %f %\n",k,e[k]);
    }

    for(i=2,k=1;i<=m;i++){
        if(e[i]<e[k])
            k=i;
    }
    return k;
}

float knn2(int **test, int **train, int k){
    int i,j,p,z,count1=0,max;
    float error,d;
    distanceNode array[k];
    for(i=0;i<testTotal;i++){
        for(p=0;p<k;p++){
            array[p].distance = -1;
            array[p].index = -1;
        }
        z=0;
        for(j=0;j<trainTotal;j++){
            d=distanceBetween2[i][j];
            if(z>=0 && z<=k-1){
                array[z].distance = d;
                array[z++].index = j;
            }
            else{
                for(p=1,max=0;p<k;p++){
                    if(array[max].distance<array[p].distance)
                        max = p;
                }
                if(d<array[max].distance){
                    array[max].distance = d;
                    array[max].index = j;
                }
            }
        }

        int count2[10] = {0};
        for(p=0;p<k;p++)
            (count2[train[array[p].index][192]])++;

        for(p=1,max=0;p<10;p++){
            if(count2[max]<count2[p])
                max=p;
        }

        // max now contains the label occurring maximum no. of times among all the k-nearest neighbors
        if(max==test[i][192])
            count1++;
    }

    error = 100.0 - ((float)(count1*100)/(testTotal));
    return error;
}

/***************** M A I N ************************/

int main(){
    int **ar1,**ar2,i,j,k;
    float error;
    ar1 = readFromDAT("shuffled.dat",trainTotal);
    readFromTXT("dist_tra.txt",trainTotal,trainTotal,&distanceBetween);

    k = threeFoldValidation(ar1);
    printf("\nChosen k = %d\n\n",k);

    ar2 = readFromDAT("pp_tes_3333x193.dat",testTotal);
    readFromTXT("dist_tes.txt",testTotal,trainTotal,&distanceBetween2);

    error = knn2(ar2, ar1, k);
    printf("\nERROR ON THE TEST SET IS : %f %\n",error);
    printf("-----------------------------------------\nSo, ACCURACY ON THE TEST SET IS : %f %\n",100.0-error);

    return 0;
}
