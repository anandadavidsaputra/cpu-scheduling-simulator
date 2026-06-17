#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h> 

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    bool isCompleted; 
};

bool sortByArrivalTime(Process a, Process b) {
    return a.arrivalTime < b.arrivalTime;
}

string bukaFileExplorer() {
    OPENFILENAME ofn;
    char namaFile[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = namaFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";

    if (GetOpenFileName(&ofn)) {
        return string(namaFile);
    }
    return "";
}

// =================================================================
// 1. METODE FCFS
// =================================================================
void executeFCFS(vector<Process> queueList, ofstream& fileOutput) {
    sort(queueList.begin(), queueList.end(), sortByArrivalTime);

    int currentTime = 0;
    float totalWaitingTime = 0;
    int totalProcess = queueList.size();

    fileOutput << "=== HASIL SIMULASI: FCFS ===\n";
    fileOutput << "Riwayat Panggilan Antrean:\n";

    for (int i = 0; i < totalProcess; i++) {
        if (currentTime < queueList[i].arrivalTime) {
            currentTime = queueList[i].arrivalTime;
        }

        queueList[i].waitingTime = currentTime - queueList[i].arrivalTime;
        totalWaitingTime += queueList[i].waitingTime;

        // Output dibikin kaya struk/nomor antrean ke bawah
        fileOutput << i + 1 << ". Proses P" << queueList[i].id 
                   << " -> Waktu Masuk: " << currentTime << " menit"
                   << " | Waktu Tunggu: " << queueList[i].waitingTime << " menit"
                   << " | Durasi: " << queueList[i].burstTime << " menit"
                   << " | Waktu Selesai: " << (currentTime + queueList[i].burstTime) << " menit\n";

        currentTime += queueList[i].burstTime;
    }

    fileOutput << "--------------------------------------------------\n";
    fileOutput << "Rata-rata Waiting Time: " << (totalWaitingTime / totalProcess) << " menit\n\n\n";
}

// =================================================================
// 2. METODE PRIORITY SCHEDULING
// =================================================================
void executePriorityNonPreemptive(vector<Process> queueList, ofstream& fileOutput) {
    int currentTime = 0; 
    float totalWaitingTime = 0;
    int totalProcess = queueList.size();
    int processDone = 0;
    int urutanPanggil = 1;

    fileOutput << "=== HASIL SIMULASI: PRIORITY NON-PREEMPTIVE ===\n";
    fileOutput << "Riwayat Panggilan Antrean:\n";

    while (processDone < totalProcess) {
        int selectedIndex = -1;
        int highestPriority = 99999;       
        int earliestArrival = 99999;

        for (int i = 0; i < totalProcess; i++) {
            if (!queueList[i].isCompleted && queueList[i].arrivalTime <= currentTime) {
                if (queueList[i].priority < highestPriority) {
                    highestPriority = queueList[i].priority;
                    earliestArrival = queueList[i].arrivalTime;
                    selectedIndex = i;
                } 
                else if (queueList[i].priority == highestPriority) {
                    if (queueList[i].arrivalTime < earliestArrival) {
                        earliestArrival = queueList[i].arrivalTime;
                        selectedIndex = i;
                    }
                }
            }
        }

        if (selectedIndex == -1) {
            currentTime++; 
        } 
        else {
            int i = selectedIndex;
            queueList[i].waitingTime = currentTime - queueList[i].arrivalTime;
            totalWaitingTime += queueList[i].waitingTime;

            // Output dibikin kaya struk/nomor antrean ke bawah, ditambah info kasta
            fileOutput << urutanPanggil++ << ". Proses P" << queueList[i].id 
                       << " [Prioritas " << queueList[i].priority << "]"
                       << " -> Waktu Masuk: " << currentTime << " menit"
                       << " | Waktu Tunggu: " << queueList[i].waitingTime << " menit"
                       << " | Durasi: " << queueList[i].burstTime << " menit"
                       << " | Waktu Selesai: " << (currentTime + queueList[i].burstTime) << " menit\n";

            currentTime += queueList[i].burstTime;
            queueList[i].isCompleted = true; 
            processDone++;
        }
    }

    fileOutput << "--------------------------------------------------\n";
    fileOutput << "Rata-rata Waiting Time: " << (totalWaitingTime / totalProcess) << " menit\n";
}

int main() {
    cout << "Silakan pilih file input.txt dari File Explorer yang muncul..." << endl;
    
    string pathFile = bukaFileExplorer();

    if (pathFile == "") {
        cout << "Batal milih file. Program berhenti." << endl;
        return 1;
    }

    cout << "File yang dipilih: " << pathFile << endl;

    ifstream fileInput(pathFile);
    ofstream fileOutput("output_log.txt");

    if (!fileInput) {
        cout << "Waduh file gagal dibaca!" << endl;
        return 1;
    }

    vector<Process> dataAntrean;
    Process p;

    while (fileInput >> p.id >> p.arrivalTime >> p.burstTime >> p.priority) {
        p.waitingTime = 0;
        p.isCompleted = false;
        dataAntrean.push_back(p);
    }

    executeFCFS(dataAntrean, fileOutput);
    executePriorityNonPreemptive(dataAntrean, fileOutput);

    fileInput.close();
    fileOutput.close();

    cout << "Simulasi kelar! Otomatis buka log hasil..." << endl;

    system("start output_log.txt");

    return 0;
}