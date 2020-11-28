#include <stdio.h>
#include <stdlib.h>

typedef struct personal_info {
	char genre;			//1 Byte (+3 alinhamento)
	int age;			//4 Bytes
	const char *name;	//8 Bytes
} PersonalInfo;

typedef struct check_point {
	long entry_time;	//8 Bytes
	long exit_time;		//8 Bytes
} CheckPoint;

typedef struct worker {
	PersonalInfo identity;		//16 Bytes
	size_t check_points_len;	//8 Bytes
	CheckPoint check_points[];	//Dynamic
} Worker;

long get_total_work_time_after(Worker *worker, long start_time);/*{
	long totalTime=0;
	CheckPoint *aux = worker->check_points;
	for(int i=0; i<worker->check_points_len; i++){
		if(aux[i].entry_time >= start_time) totalTime+= aux[i].exit_time - aux[i].entry_time;
		else if(aux[i].exit_time > start_time) totalTime+= aux[i].exit_time - start_time;
	}
	return totalTime;
}*/

int main(){
	
	
	char *name = "joao";
	PersonalInfo info = {'m', 19, name};
	
	CheckPoint ck[]= {{10, 20}, {5, 505}, {50, 120}};

	Worker *work = malloc(sizeof(Worker) + 3*sizeof(CheckPoint));
	work->identity = info;
	work->check_points_len=3;
	work->check_points[0]= ck[0];
	work->check_points[1]= ck[1];
	work->check_points[2]= ck[2];
	
	
	printf("%ld\n", get_total_work_time_after(work, 10));
}
