#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h> // Library untuk membuka GUI File Dialog di Windows

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    int completionTime;
    bool isCompleted;
};

// Fungsi untuk membuka File Explorer (GUI) dan mengambil path file
string openFileDialog() {
    OPENFILENAME ofn;
    char szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text/CSV Files\0*.txt;*.csv\0All Files\0*.*\0"; // Filter format file
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return string(ofn.lpstrFile);
    }
    return ""; // Return kosong jika user membatalkan (cancel)
}

void readInputFile(const string& filename, vector<Process>& processes) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Gagal membuka file " << filename << "!" << endl;
        return;
    }
    Process p;
    while (file >> p.id >> p.arrivalTime >> p.burstTime >> p.priority) {
        p.waitingTime = 0;
        p.turnaroundTime = 0;
        p.completionTime = 0;
        p.isCompleted = false;
        processes.push_back(p);
    }
    file.close();
}

void sortProcesses(vector<Process>& processes) {
    for (size_t i = 0; i < processes.size() - 1; i++) {
        for (size_t j = 0; j < processes.size() - i - 1; j++) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                swap(processes[j], processes[j + 1]);
            } else if (processes[j].arrivalTime == processes[j + 1].arrivalTime) {
                if (processes[j].priority > processes[j + 1].priority) {
                    swap(processes[j], processes[j + 1]);
                }
            }
        }
    }
}

void executeFCFS(vector<Process>& processes) {
    sortProcesses(processes);
    int currentTime = 0;

    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime; 
        }
        p.waitingTime = currentTime - p.arrivalTime;
        p.completionTime = currentTime + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        currentTime = p.completionTime;
    }
}

void executePriorityNonPreemptive(vector<Process>& processes) {
    int currentTime = 0;
    int completedCount = 0;
    int n = processes.size();

    while (completedCount < n) {
        int idx = -1;
        int highestPriority = 9999;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !processes[i].isCompleted) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    idx = i;
                } else if (processes[i].priority == highestPriority) {
                    if (idx == -1 || processes[i].arrivalTime < processes[idx].arrivalTime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            processes[idx].waitingTime = currentTime - processes[idx].arrivalTime;
            processes[idx].completionTime = currentTime + processes[idx].burstTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].isCompleted = true;
            currentTime = processes[idx].completionTime;
            completedCount++;
        } else {
            currentTime++; 
        }
    }
}

void calculateMetricsAndGanttChart(vector<Process>& processes, const string& algoName, ofstream& outFile) {
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;
    int n = processes.size();

    outFile << "=== HASIL SIMULASI: " << algoName << " ===\n";
    
    vector<Process> ganttProcesses = processes;
    for (size_t i = 0; i < ganttProcesses.size() - 1; i++) {
        for (size_t j = 0; j < ganttProcesses.size() - i - 1; j++) {
            if (ganttProcesses[j].completionTime > ganttProcesses[j + 1].completionTime) {
                swap(ganttProcesses[j], ganttProcesses[j + 1]);
            }
        }
    }

    outFile << "Gantt Chart (Visualisasi Linier): [ ";
    for (const auto& p : ganttProcesses) {
        outFile << "P" << p.id << " ";
    }
    outFile << "]\n\n";

    outFile << "ID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n";
    for (const auto& p : processes) {
        outFile << p.id << "\t" << p.arrivalTime << "\t" << p.burstTime << "\t" << p.priority
                << "\t\t" << p.waitingTime << "\t" << p.turnaroundTime << "\n";
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
    }

    outFile << "\nRata-rata Waiting Time    : " << (totalWaitingTime / n) << "\n";
    outFile << "Rata-rata Turnaround Time : " << (totalTurnaroundTime / n) << "\n";
    outFile << "--------------------------------------------------\n\n";
}

int main() {
    vector<Process> processList;
    
    cout << "Silakan pilih file input (.txt / .csv) melalui jendela direktori yang terbuka..." << endl;
    
    // Memanggil fungsi untuk membuka file explorer
    string filePath = openFileDialog();

    if (filePath.empty()) {
        cout << "Operasi dibatalkan. Tidak ada file yang dipilih." << endl;
        return 1;
    }

    cout << "File yang dipilih: " << filePath << endl;

    // Membaca file input dari path yang dipilih user
    readInputFile(filePath, processList);

    if (processList.empty()) {
        cout << "Gagal mengeksekusi: File kosong atau format salah." << endl;
        return 1;
    }

    ofstream outFile("output_log.txt");

    vector<Process> fcfsList = processList;
    executeFCFS(fcfsList);
    calculateMetricsAndGanttChart(fcfsList, "FCFS", outFile);

    vector<Process> priorityList = processList;
    executePriorityNonPreemptive(priorityList);
    calculateMetricsAndGanttChart(priorityList, "Priority Non-Preemptive", outFile);

    outFile.close();
    cout << "Eksekusi berhasil. Log simulasi tersimpan di 'output_log.txt'." << endl;

    return 0;
}