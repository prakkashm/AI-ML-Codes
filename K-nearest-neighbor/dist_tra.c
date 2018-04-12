#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

/******************* R E A D I N G   F R O M   F I L E ***************************/

int trainTotal = 6670;
int testTotal = 3333;
int count = 193;
float distanceBetween[6670][6670] = {0};

int **readFromDAT(char filename[],int totalLines)
{
	int **ar = (int**)malloc(totalLines*sizeof(int*));
	FILE *f = fopen(filename,"r");
	if(f){
		char buf[500];
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

void findDistance(int **a, int i, int j){
    int c;
    float d = 0;
    for(c=0;c<count-1;c++){
        d+=pow((a[i][c]-a[j][c]),2);
    }
    d = sqrt(d);
    distanceBetween[i][j] = d;
    distanceBetween[j][i] = d;
    //printf("distanceBetween %d & %d = %f\n",i,j,distanceBetween[i][j]);
}

/**************************************************************************/

int main(){
    int **ar1,i,j;
    ar1 = readFromDAT("shuffled.dat",trainTotal);

    for(i=0;i<6670;i++){
        for(j=0;j<6670;j++){
            if(i<=j)
                findDistance(ar1,i,j);
        }
    }

    FILE *fp = fopen("dist_tra.txt","w");

    for(i=0;i<6670;i++){
        for(j=0;j<6670;j++){
            if(j != 6669)
                fprintf(fp,"%f ",distanceBetween[i][j]);
            else
                fprintf(fp,"%f",distanceBetween[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

    return 0;
}
