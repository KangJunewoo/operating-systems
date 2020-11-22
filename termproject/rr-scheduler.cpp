/**
 * 스케줄러 구현 연습 (Round Robin 방식)
 * Code Ref : https://shivammitra.com/operating%20system/roundrobin-scheduling-program/#
*/
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
using namespace std;

struct process {
    int pid; // 프로세스 고유번호
    int arrival_time; // 도착시각
    int burst_time; // 실행된시간
    int start_time; // 시작된시각
    int completion_time; // 완료된시각
    int turnaround_time; // arrive ~ finish
    int waiting_time; // 기다린시간
    int response_time; // 도착~ 첫 cpu할당
};

bool compare1(process p1, process p2){
    return p1.arrival_time < p2.arrival_time;
}

bool compare2(process p1, process p2){
    return p1.pid < p2.pid;
}

int main(){
    int n; // 프로세스수
    int tq; // 타임퀀텀
    struct process p[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilization;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    int burst_remaining[100];
    int idx;

    cout << setprecision(2) << fixed;

    cout << "Enter the # of processes : ";
    cin >> n;
    cout << "Enter time quantum : ";
    cin >> tq;

    for(int i=0;i<n;i++){
        cout << "Enter arrival time of process " << i+1 << " : ";
        cin >> p[i].arrival_time;
        cout << "Enter burst time of process " << i+1 << " : ";
        cin >> p[i].burst_time;
        burst_remaining[i] = p[i].burst_time;
        p[i].pid = i+1;
        cout << endl;
    }

    sort(p, p+n, compare1);

    queue<int> q;
    int current_time = 0;
    q.push(0);
    int completed = 0;
    int mark[100];
    memset(mark, 0, sizeof(mark));
    mark[0] = 1;

    while(completed != n){ // 프로세스들이 쌓인 큐를 RR방식으로 돌리며 처리
        idx = q.front();
        q.pop(); // pop 해서 idx에 저장

        if(burst_remaining[idx] == p[idx].burst_time) { // remaining == burst면
            p[idx].start_time = max(current_time,p[idx].arrival_time);
            total_idle_time += p[idx].start_time - current_time;
            current_time = p[idx].start_time;
        }

        if(burst_remaining[idx]-tq > 0) {
            burst_remaining[idx] -= tq;
            current_time += tq;
        }
        else {
            current_time += burst_remaining[idx];
            burst_remaining[idx] = 0;
            completed++;

            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            p[idx].response_time = p[idx].start_time - p[idx].arrival_time;

            total_turnaround_time += p[idx].turnaround_time;
            total_waiting_time += p[idx].waiting_time;
            total_response_time += p[idx].response_time;
        }

        for(int i = 1; i < n; i++) {
            if(burst_remaining[i] > 0 && p[i].arrival_time <= current_time && mark[i] == 0) {
                q.push(i);
                mark[i] = 1;
            }
        }
        if(burst_remaining[idx] > 0) {
            q.push(idx);
        }

        if(q.empty()) {
            for(int i = 1; i < n; i++) {
                if(burst_remaining[i] > 0) {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }
    }

    avg_turnaround_time = (float) total_turnaround_time / n;
    avg_waiting_time = (float) total_waiting_time / n;
    avg_response_time = (float) total_response_time / n;
    cpu_utilization = ((p[n-1].completion_time - total_idle_time) / (float) p[n-1].completion_time) * 100;
    throughput = float(n) / ((p[n-1].completion_time - p[0].arrival_time));

    sort(p, p+n, compare2);

    cout << endl;
    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<"\n"<<endl;

    for(int i = 0; i < n; i++) {
        cout<<p[i].pid<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\t"<<p[i].turnaround_time<<"\t"<<p[i].waiting_time<<"\t"<<p[i].response_time<<"\t"<<"\n"<<endl;
    }
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"CPU Utilization = "<<cpu_utilization<<"%"<<endl;
    cout<<"Throughput = "<<throughput<<" process/unit time"<<endl;
}