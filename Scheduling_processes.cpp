#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <climits>
#include <queue>

using namespace std;

int dispatch = 0;
vector<int> processes, arrival, burst, priority;
vector<float> avgwt, avgtt;

int extract_int(string s)
{
    stringstream ss;
    ss << s;
    string total_time;
    int found;
    while (!ss.eof())
    {
        ss >> total_time;
        stringstream(total_time) >> found;
    }
    return found;
}

void input(fstream &f)
{
    string s;
    int ctr = 1;
    while (getline(f, s))
    {
        if (s == "EOF")
            break;
        else
        {
            if (s == "END")
                continue;
            if (s[0] == 'D')
            {
                dispatch = extract_int(s);
            }
            else if (s[0] == 'I')
            {
                processes.push_back(ctr);
                ctr++;
            }
            else if (s[0] == 'A')
            {
                arrival.push_back(extract_int(s));
            }
            else if (s[0] == 'E')
            {
                burst.push_back(extract_int(s));
            }
            else if (s[0] == 'P')
            {
                priority.push_back(extract_int(s));
            }
        }
    }
}

void display(vector<int> wt, vector<int> tt)
{
    int ctr = 0;
    cout << "\nProcesses  Turnaround Time  Waiting Time\n";
    int n = processes.size();
    while (n--)
    {
        cout << "p" << processes[ctr] << "\t\t" << tt[ctr] << "\t\t  " << wt[ctr] << "\n";
        ctr++;
    }
    cout << "\n";
}

void summary()
{
    int n = processes.size();
    cout << "SUMMARY\nAlgorithm\tAverage Turnaround Time\t  Average Waiting Time\n";
    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
            cout << "FCFS\t\t\t" << avgtt[i] << "\t\t\t" << avgwt[i] << "\n";
        else if (i == 1)
            cout << "SPN\t\t\t" << avgtt[i] << "\t\t\t" << avgwt[i] << "\n";
        else if (i == 2)
            cout << "SRPN\t\t\t" << avgtt[i] << "\t\t\t" << avgwt[i] << "\n";
        else if (i == 3)
            cout << "PP\t\t\t" << avgtt[i] << "\t\t\t" << avgwt[i] << "\n";
        else
            cout << "PRR\t\t\t" << avgtt[i] << "\t\t\t" << avgwt[i] << "\n";
    }
}

void FCFS()
{
    int n = processes.size();
    vector<int> wait_time(n), turnaround_time(n);
    wait_time[0] = dispatch;

    // calculating waiting time
    int total = 0;
    for (int i = 1; i < processes.size(); i++)
    {
        wait_time[i] = arrival[i - 1] + burst[i - 1] + wait_time[i - 1] + dispatch - arrival[i];
    }
    for (int i = 0; i < n; i++)
    {
        total += wait_time[i];
    }
    avgwt.push_back((float)total / n);

    total = 0;
    // calculating turnaround time
    for (int i = 0; i < n; i++)
    {
        turnaround_time[i] = burst[i] + wait_time[i];
        total += turnaround_time[i];
    }
    avgtt.push_back((float)total / n);

    cout << "FCFS:\n";
    int ctr = 0;
    while (n--)
    {
        cout << "T" << wait_time[ctr] + arrival[ctr] << ": p" << processes[ctr] << "(" << priority[ctr] << ")\n";
        ctr++;
    }
    display(wait_time, turnaround_time);
}

void SPN()
{
    int n = processes.size();
    vector<int> wait_time(n), turnaround_time(n);

    int rt[n] = {0};
    for (int i = 0; i < n; i++)
        rt[i] = burst[i];

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    bool check = false;
    cout << "SPN:\n";
    while (complete != n)
    {
        bool check_disp = false;
        for (int j = 0; j < n; j++)
        {
            if ((arrival[j] <= t) && (rt[j] < minm) && rt[j] > 0)
            {
                minm = rt[j];
                shortest = j;
                check = true;
                check_disp = true;
            }
        }
        if (check == false)
        {
            t++;
            continue;
        }
        if (check_disp)
        {
            t += dispatch;
        }

        cout << "T" << t << ": p" << processes[shortest] << "(" << priority[shortest] << ")" << endl;

        t += burst[shortest];

        rt[shortest] = 0;
        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        if (rt[shortest] == 0)
        {
            complete++;
            check = false;
            finish_time = t;
            // Calculate waiting time
            wait_time[shortest] = finish_time - burst[shortest] - arrival[shortest];
            if (wait_time[shortest] < 0)
                wait_time[shortest] = 0;
            // Calculate turnaround time
            turnaround_time[shortest] = wait_time[shortest] + burst[shortest];
        }
    }

    int totwt = 0, tottt = 0;
    for (int i = 0; i < n; i++)
    {
        totwt += wait_time[i];
        tottt += turnaround_time[i];
    }
    avgwt.push_back((float)totwt / n);
    avgtt.push_back((float)tottt / n);
    display(wait_time, turnaround_time);
}

void SRPN()
{
    int n = processes.size();
    vector<int> wait_time(n), turnaround_time(n);

    int rt[n] = {0};
    for (int i = 0; i < n; i++)
        rt[i] = burst[i];

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time, prev_shortest = -1;
    bool check = false;
    cout << "SRPN:\n";
    while (complete != n)
    {
        bool check_disp = false;
        for (int j = 0; j < n; j++)
        {
            if ((arrival[j] <= t) && (rt[j] < minm) && rt[j] > 0)
            {
                minm = rt[j];
                shortest = j;
                check = true;
                check_disp = true;
            }
        }

        if (check == false)
        {
            t++;
            continue;
        }
        if (check_disp)
        {
            t += dispatch;
        }
        if (prev_shortest != shortest)
        {
            cout << "T" << t << ": p" << processes[shortest] << "(" << priority[shortest] << ")" << endl;
        }
        prev_shortest = shortest;

        // Increment time
        t++;
        // decrementing the remaining time
        rt[shortest]--;
        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;
        // If a process gets completely
        // executed
        if (rt[shortest] == 0)
        {
            complete++;
            check = false;
            finish_time = t;
            // Calculate waiting time
            wait_time[shortest] = finish_time - burst[shortest] - arrival[shortest];
            if (wait_time[shortest] < 0)
                wait_time[shortest] = 0;
            // Calculate turnaround time
            turnaround_time[shortest] = wait_time[shortest] + burst[shortest];
        }
    }
    int totwt = 0, tottt = 0;
    for (int i = 0; i < n; i++)
    {
        totwt += wait_time[i];
        tottt += turnaround_time[i];
    }
    avgwt.push_back((float)totwt / n);
    avgtt.push_back((float)tottt / n);
    display(wait_time, turnaround_time);
}

void PP()
{
    int n = processes.size();
    vector<int> wait_time(n), turnaround_time(n);

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int left[n] = {0};

    for (int i = 0; i < n; i++)
        left[i] = burst[i];

    bool check = false;
    cout << "PP:\n";
    while (complete != n)
    {
        bool check_disp = false;
        for (int j = 0; j < n; j++)
        {
            if ((arrival[j] <= t) && (priority[j] < minm) && left[j] > 0)
            {
                minm = priority[j];
                shortest = j;
                check = true;
                check_disp = true;
            }
        }
        if (!check)
        {
            t++;
            continue;
        }
        if (check_disp)
        {
            t += dispatch;
            cout << "T" << t << ": p" << processes[shortest] << "(" << priority[shortest] << ")" << endl;
        }

        t++;
        left[shortest]--;

        if (left[shortest] == 0)
        {
            minm = INT_MAX;
            complete++;
            check = false;
            finish_time = t;
            // Calculate waiting time
            wait_time[shortest] = finish_time - burst[shortest] - arrival[shortest];
            if (wait_time[shortest] < 0)
                wait_time[shortest] = 0;
            // Calculate turnaround time
            turnaround_time[shortest] = wait_time[shortest] + burst[shortest];
        }
    }

    int totwt = 0, tottt = 0;
    for (int i = 0; i < n; i++)
    {
        totwt += wait_time[i];
        tottt += turnaround_time[i];
    }
    avgwt.push_back((float)totwt / n);
    avgtt.push_back((float)tottt / n);
    display(wait_time, turnaround_time);
}

void PRR()
{
    int n = processes.size();
    vector<int> wait_time(n), turnaround_time(n);

    int complete = 0, t = 0, minm = INT_MAX;
    int finish_time;
    int left[n] = {0};
    queue<int> q;
    int prev_check = -1;

    for (int i = 0; i < n; i++)
        left[i] = burst[i];

    bool check = false;
    cout << "PRR:\n";
    while (complete != n)
    {
        check = false;

        for (int i = 0; i < n; i++)
        {
            if (arrival[i] <= t && arrival[i] > prev_check)
            {
                q.push(i);
                check = true;
            }
        }
        prev_check = t;

        if (q.size() == 0)
        {
            t++;
            continue;
        }
        int quant = 2, cur = q.front();
        if (left[cur] == 0)
        {
            q.pop();
            if (q.size() > 0)
            {
                cur = q.front();
                t += dispatch;
            }
            else
            {
                t++;
                continue;
            }
        }
        else
        {
            if (t > 0)
            {
                q.pop();
                q.push(cur);
            }
            cur = q.front();
            t += dispatch;
        }
        if (priority[cur] <= 2)
        {
            quant = 4;
        }

        cout << "T" << t << ": p" << processes[cur] << "(" << priority[cur] << ")" << endl;

        int time_skip = min(quant, left[cur]);
        t += time_skip;
        left[cur] -= time_skip;

        if (left[cur] == 0)
        {
            complete++;
            finish_time = t;
            //Calculating wait time
            wait_time[cur] = finish_time - burst[cur] - arrival[cur];
            if (wait_time[cur] < 0)
                wait_time[cur] = 0;
            // Calculating turnaround time
            turnaround_time[cur] = wait_time[cur] + burst[cur];
        }
    }

    int totwt = 0, tottt = 0;
    for (int i = 0; i < n; i++)
    {
        totwt += wait_time[i];
        tottt += turnaround_time[i];
    }
    avgwt.push_back((float)totwt / n);
    avgtt.push_back((float)tottt / n);
    display(wait_time, turnaround_time);
}

int main()
{
    fstream f;
    f.open("Input.txt", ios::in);
    if (f.is_open())
        input(f);
    else
        cout << "File Not Found.......";
    FCFS();
    SPN();
    SRPN();
    PP();
    PRR();
    summary();
    f.close();
    return 0;
}