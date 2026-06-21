#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h> 

using namespace std;

// struktur buat nyimpen data proses
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    bool isCompleted; 
};

// fungsi bantu buat sorting FCFS
bool sortByArrival(Process a, Process b) {
    return a.arrivalTime < b.arrivalTime;
}

// manggil pop up windows explorer
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
void jalaninFCFS(vector<Process> pList, ofstream& out) {
    sort(pList.begin(), pList.end(), sortByArrival);

    int waktuSekarang = 0;
    float totalNunggu = 0;
    int jmlProses = pList.size();

    out << "=== HASIL SIMULASI: FCFS ===\n";
    out << "Riwayat Panggilan Antrean:\n";

    for (int i = 0; i < jmlProses; i++) {
        // kalo cpu nganggur nungguin proses dateng
        if (waktuSekarang < pList[i].arrivalTime) {   
            waktuSekarang = pList[i].arrivalTime;
        }

        pList[i].waitingTime = waktuSekarang - pList[i].arrivalTime;
        totalNunggu += pList[i].waitingTime;

        out << i + 1 << ". Proses P" << pList[i].id 
            << " -> Waktu Masuk: " << waktuSekarang << " menit"
            << " | Waktu Tunggu: " << pList[i].waitingTime << " menit"
            << " | Durasi: " << pList[i].burstTime << " menit"
            << " | Waktu Selesai: " << (waktuSekarang + pList[i].burstTime) << " menit\n";

        waktuSekarang += pList[i].burstTime;
    }

    out << "--------------------------------------------------\n";
    out << "Rata-rata Waiting Time: " << (totalNunggu / jmlProses) << " menit\n\n\n";
}

// =================================================================
// 2. METODE PRIORITY SCHEDULING
// =================================================================
void jalaninPriority(vector<Process> pList, ofstream& out) {
    int waktuSekarang = 0; 
    float totalNunggu = 0;
    int jmlProses = pList.size();
    int ygUdahSelesai = 0;
    int noUrut = 1;

    out << "=== HASIL SIMULASI: PRIORITY NON-PREEMPTIVE ===\n";
    out << "Riwayat Panggilan Antrean:\n";

    while (ygUdahSelesai < jmlProses) {
        int indexKepilih = -1;
        int prioritasTertinggi = 99999;       
        int datengPalingAwal = 99999;

        for (int i = 0; i < jmlProses; i++) {
            // cek yg udah dateng & blm kelar
            if (!pList[i].isCompleted && pList[i].arrivalTime <= waktuSekarang) {
                if (pList[i].priority < prioritasTertinggi) {
                    prioritasTertinggi = pList[i].priority;
                    datengPalingAwal = pList[i].arrivalTime;
                    indexKepilih = i;
                } 
                else if (pList[i].priority == prioritasTertinggi) {
                    // tie breaker kalo prioritas sama
                    if (pList[i].arrivalTime < datengPalingAwal) {
                        datengPalingAwal = pList[i].arrivalTime;
                        indexKepilih = i;
                    }
                }
            }
        }

        if (indexKepilih == -1) {
            waktuSekarang++; 
        } 
        else {
            int i = indexKepilih;
            pList[i].waitingTime = waktuSekarang - pList[i].arrivalTime;
            totalNunggu += pList[i].waitingTime;

            out << noUrut++ << ". Proses P" << pList[i].id 
                << " [Prioritas " << pList[i].priority << "]"
                << " -> Waktu Masuk: " << waktuSekarang << " menit"
                << " | Waktu Tunggu: " << pList[i].waitingTime << " menit"
                << " | Durasi: " << pList[i].burstTime << " menit"
                << " | Waktu Selesai: " << (waktuSekarang + pList[i].burstTime) << " menit\n";

            waktuSekarang += pList[i].burstTime;
            pList[i].isCompleted = true; 
            ygUdahSelesai++;
        }
    }

    out << "--------------------------------------------------\n";
    out << "Rata-rata Waiting Time: " << (totalNunggu / jmlProses) << " menit\n\n\n";
}

// =================================================================
// 3. METODE SJF (Shortest Job First)
// =================================================================
void jalaninSJF(vector<Process> pList, ofstream& out) {
    int waktuSekarang = 0; 
    float totalNunggu = 0;
    int jmlProses = pList.size();
    int ygUdahSelesai = 0;
    int noUrut = 1;

    out << "=== HASIL SIMULASI: SJF NON-PREEMPTIVE ===\n";
    out << "Riwayat Panggilan Antrean:\n";

    while (ygUdahSelesai < jmlProses) {
        int indexKepilih = -1;
        int durasiTercepat = 99999; // buat nyari burst time paling kecil
        int datengPalingAwal = 99999;

        for (int i = 0; i < jmlProses; i++) {
            if (!pList[i].isCompleted && pList[i].arrivalTime <= waktuSekarang) {
                // cek durasi paling cepet
                if (pList[i].burstTime < durasiTercepat) {
                    durasiTercepat = pList[i].burstTime;
                    datengPalingAwal = pList[i].arrivalTime;
                    indexKepilih = i;
                } 
                else if (pList[i].burstTime == durasiTercepat) {
                    // tie breaker kalo durasi sama, pilih yg dtg duluan
                    if (pList[i].arrivalTime < datengPalingAwal) {
                        datengPalingAwal = pList[i].arrivalTime;
                        indexKepilih = i;
                    }
                }
            }
        }

        if (indexKepilih == -1) {
            waktuSekarang++; 
        } 
        else {
            int i = indexKepilih;
            pList[i].waitingTime = waktuSekarang - pList[i].arrivalTime;
            totalNunggu += pList[i].waitingTime;

            out << noUrut++ << ". Proses P" << pList[i].id 
                << " -> Waktu Masuk: " << waktuSekarang << " menit"
                << " | Waktu Tunggu: " << pList[i].waitingTime << " menit"
                << " | Durasi: " << pList[i].burstTime << " menit"
                << " | Waktu Selesai: " << (waktuSekarang + pList[i].burstTime) << " menit\n";

            waktuSekarang += pList[i].burstTime;
            pList[i].isCompleted = true; 
            ygUdahSelesai++;
        }
    }

    out << "--------------------------------------------------\n";
    out << "Rata-rata Waiting Time: " << (totalNunggu / jmlProses) << " menit\n";
}

int main() {
    cout << "Pilih file input.txt nya di window yg muncul ya..." << endl;
    
    string pathFile = bukaFileExplorer();

    if (pathFile == "") {
        cout << "Gak ada file yg dipilih. Program udahan." << endl;
        return 1;
    }

    cout << "File yg kepilih: " << pathFile << endl;

    ifstream fileIn(pathFile);
    ofstream fileOut("output_log.txt");

    if (!fileIn) {
        cout << "Duh file nya error ga bisa dibaca!" << endl;
        return 1;
    }

    vector<Process> daftarProses;
    Process p;

    // baca isi file
    while (fileIn >> p.id >> p.arrivalTime >> p.burstTime >> p.priority) {
        p.waitingTime = 0;
        p.isCompleted = false;
        daftarProses.push_back(p);
    }

    // adu ketiga metode
    jalaninFCFS(daftarProses, fileOut);
    jalaninPriority(daftarProses, fileOut);
    jalaninSJF(daftarProses, fileOut);

    fileIn.close();
    fileOut.close();

    cout << "Beres! Langsung buka output_log.txt..." << endl;

    system("start output_log.txt");

    return 0;
}