#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;

const int JML_KOLOM = 6;

struct Tugas
{
    int idTugas;
    string namaTugas;
    string mataKuliah;
    string tenggatWaktu; // Format: DD/MM/YYYY
    int prioritas;       // 1 = tinggi, 2 = sedang, 3 = rendah
    bool selesai;        // true jika selesai, false jika belum
};

/* Begin Fungsi Pembantu */

// Fungsi untuk mengubah string prioritas seperti "Tinggi", "Sedang", dan "Rendah" menjadi 1, 2, dan 3
int prioritasTugasKeInt(string prioritas)
{
    if (prioritas == "Tinggi")
    {
        return 1;
    }
    else if (prioritas == "Sedang")
    {
        return 2;
    }
    else if (prioritas == "Rendah")
    {
        return 3;
    }
    else
    {
        return -1; // Nilai untuk input tidak valid
    }
}

// Fungsi untuk mengubah integer prioritas tugas seperti 1, 2, dan 3 menjadi "Tinggi", "Sedang", dan "Rendah"
string intKePrioritasTugas(int prioritas)
{
    string daftarPrioritas[3] = {"Tinggi", "Sedang", "Rendah"};
    return daftarPrioritas[prioritas - 1];
}

// Fungsi untuk mengubah status tugas menjadi boolean
bool statusTugasKeBool(string status)
{
    return (status == "Selesai");
}

string boolKeStatusTugas(bool selesai)
{
    if (selesai)
        return "Selesai";
    return "Belum Selesai";
}

// Fungsi untuk mengubah string menjadi array
// Contoh: "1|b|c|d|Tinggi|Belum Selesai" diubah menjadi {"1", "b", "c", "d", "Tinggi", "Belum Selesai"}
void lineKeArray(string line, string *arr)
{
    int insertIndex = 0;
    string temp;

    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == '|')
        {
            arr[insertIndex] = temp;
            temp = "";
            insertIndex++;
        }
        else
        {
            temp += line[i];
        }
    }

    arr[insertIndex] = temp;
}

// Fungsi yang mengubah array menjadi struct
// Contoh: {"1", "Project", "Pemrograman Dasar", "24/12/2024", "Tinggi", "Belum Selesai"} diubah menjadi
// {
//     int idTugas = 1;
//     string namaTugas = "Project";
//     string mataKuliah = "Pemrograman Dasar";
//     string tenggatWaktu = "23/12/2024";
//     int prioritas = 1;
//     bool selesai = false;
// };
void arrayKeStruct(string *arr, Tugas &tugas)
{
    tugas.idTugas = stoi(arr[0]); // stoi = string to integer
    tugas.namaTugas = arr[1];
    tugas.mataKuliah = arr[2];
    tugas.tenggatWaktu = arr[3];
    tugas.prioritas = prioritasTugasKeInt(arr[4]);
    tugas.selesai = statusTugasKeBool(arr[5]);
}

// Fungsi yang menghitung jumlah baris file
int hitungBarisFile(const string namaFile)
{
    ifstream fileData(namaFile);
    int jumlahBaris = 0;
    string line;

    // Menghitung jumlah baris pada file
    while (getline(fileData, line))
    {
        jumlahBaris++;
    }

    fileData.close();
    return jumlahBaris;
}

// Fungsi untuk mengecek apakah sebuah string adalah format tanggal yang valid (DD/MM/YYYY)
bool formatTanggalValid(string tgl)
{
    // Panjang string harus 10 (DD/MM/YYYY)
    if (tgl.length() != 10)
        return false;

    // Memeriksa apakah karakter pada posisi 3 dan 6 adalah '/'
    if (tgl[2] != '/' || tgl[5] != '/')
        return false;

    // Mengubah bagian string menjadi integer
    int hari = stoi(tgl.substr(0, 2));  // DD
    int bulan = stoi(tgl.substr(3, 2)); // MM
    int tahun = stoi(tgl.substr(6, 4)); // YYYY

    // Jika ada kesalahan konversi (return -1), maka format tidak valid
    if (hari == -1 || bulan == -1 || tahun == -1)
        return false;

    // Memeriksa validitas bulan
    if (bulan < 1 || bulan > 12)
        return false;

    // Memeriksa validitas hari berdasarkan bulan
    int hariMax;
    if (bulan == 1 || bulan == 3 || bulan == 5 || bulan == 7 || bulan == 8 || bulan == 10 || bulan == 12)
    {
        hariMax = 31; // Bulan dengan 31 hari
    }
    else if (bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11)
    {
        hariMax = 30; // Bulan dengan 30 hari
    }
    else if (bulan == 2)
    {
        // Februari: Periksa tahun kabisat
        if ((tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0))
        {
            hariMax = 29; // Tahun kabisat
        }
        else
        {
            hariMax = 28; // Bukan tahun kabisat
        }
    }

    // Memeriksa validitas hari
    if (hari < 1 || hari > hariMax)
        return false;

    return true;
}

// Fungsi yang mengecek apakah string merupakan numerik
// Fungsi untuk mengecek apakah string adalah numerik
bool cekNumerik(string str)
{
    // Jika string kosong, tidak valid
    if (str.empty())
        return false;

    // Memeriksa setiap karakter dalam string
    for (int i = 0; i < str.length(); i++)
    {
        // Karakter harus berada dalam rentang '0' hingga '9'
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }

    return true;
}

// Fungsi untuk mengecek apakah pilihan ada pada opsi
bool cekPilihan(string pilihan, int jumlahOpsi)
{
    if (!cekNumerik(pilihan))
        return false;

    int pil = stoi(pilihan);

    return (pil > 0 && pil <= jumlahOpsi);
}

// Fungsi untuk mencari index sebuah tugas berdasarkan ID tugas
int cariIndexTugas(Tugas *daftarTugas, int jumlahTugas, int idTugas)
{
    for (int i = 0; i < jumlahTugas; i++)
    {
        if (daftarTugas[i].idTugas == idTugas)
        {
            return i; // Kembalikan indeks jika ditemukan
        }
    }
    return -1; // Kembalikan -1 jika tidak ditemukan
}

// Fungsi untuk menambahkan spasi agar teks sesuai dengan lebar kolom tabel
string tambahkanSpasi(string teks, int lebar)
{
    string hasil = teks;
    while (hasil.length() < lebar)
    {
        hasil += " ";
    }
    return hasil;
}

/* End Fungsi Pembantu */

void menuUtama()
{
    cout << "########################################" << endl
         << "###     PENGELOLAAN TUGAS KULIAH     ###" << endl
         << "########################################" << endl
         << endl
         << "1. Tambah Tugas" << endl
         << "2. Lihat Daftar Tugas" << endl
         << "3. Edit Tugas" << endl
         << "4. Hapus Tugas" << endl
         << "5. Tandai Tugas Selesai" << endl
         << "6. Simpan dan Keluar" << endl
         << endl
         << "Pilih opsi (1-6): ";
}

Tugas *muatData()
{
    // Membuka file dan deklarasi variabel
    ifstream fileData("data.txt");
    int panjangArr = hitungBarisFile("data.txt");
    string line;

    // Array 2D yang akan diisi daftar tugas
    string daftarTugasStr[panjangArr][JML_KOLOM];

    // Mengisi array daftarTugasStr dengan tugas
    int lineIndex = 0;
    while (getline(fileData, line))
    {
        lineKeArray(line, daftarTugasStr[lineIndex]);
        lineIndex++;
    }

    // Array yang akan diisi struct Tugas
    Tugas *daftarTugas = new Tugas[panjangArr];

    // Mengisi array daftarTugas dari array daftarTugasStr yang telah diubah menjadi array of struct Tugas
    for (int i = 0; i < panjangArr; i++)
    {
        arrayKeStruct(daftarTugasStr[i], daftarTugas[i]);
    }

    // Menutup file
    fileData.close();

    return daftarTugas;
}

void simpanData(Tugas *daftarTugas, int jumlahTugas)
{
    ofstream fileData("data.txt");
    string data, temp;

    for (int i = 0; i < jumlahTugas; i++)
    {
        temp = "";
        temp += to_string(daftarTugas[i].idTugas) + "|";
        temp += daftarTugas[i].namaTugas + "|";
        temp += daftarTugas[i].mataKuliah + "|";
        temp += daftarTugas[i].tenggatWaktu + "|";
        temp += to_string(daftarTugas[i].prioritas) + "|";
        temp += to_string(daftarTugas[i].selesai) + "\n";
        data += temp;
    }

    fileData << data;
}

// Fungsi untuk mengecek apakah suatu string legal atau tidak terdapat karakter "|"
bool inputLegal(string input)
{
    return (input.find('|') == string::npos);
}

// Vasya
void tambahTugas()
{
    string idTugas, namaTugas, matkul, tenggat, prioritas, selesai;

    // input nama tugas
    while (true)
    {
        cout << "input nama tugas: ";
        getline(cin, namaTugas);

        // Apabila terdapat karakter "|" maka continue statement akan dijalankan dan loop mulai lagi dari awal
        if (!inputLegal(namaTugas))
        {
            cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
            continue;
        }
        break;
    }

    // input mata kuliah
    while (true)
    {
        cout << "input mata kuliah: ";
        getline(cin, matkul);

        if (!inputLegal(matkul))
        {
            cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
            continue;
        }
        break;
    }

    // input tenggat waktu
    while (true)
    {
        cout << "input tenggat waktu (DD/MM/YYYY): ";
        getline(cin, tenggat);

        // Apabila terdapat karakter "|" atau format tanggal tidak valid maka continue statement akan dijalankan dan loop mulai lagi dari awal
        if (!inputLegal(tenggat))
        {
            cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
            continue;
        }
        if (!formatTanggalValid(tenggat))
        {
            cout << "Format tanggal tidak valid!" << endl;
            continue;
        }
        break;
    }

    // input prioritas
    while (true)
    {
        cout << "input prioritas (1:tinggi/2:sedang/3:rendah): ";
        getline(cin, prioritas);
        int jumlahOpsi = 3;

        if (!cekPilihan(prioritas, jumlahOpsi))
        {
            cout << "Pilihan tidak valid!" << endl;
            continue;
        }

        prioritas = intKePrioritasTugas(stoi(prioritas));
        break;
    }

    idTugas = to_string(hitungBarisFile("data.txt") + 1);
    selesai = "Belum Selesai";

    // Menggabungkan semua input menjadi satu string
    string hasilGabung = idTugas + "|" + namaTugas + "|" + matkul + "|" + tenggat + "|" + prioritas + "|" + selesai;

    // Menyimpan hasil ke file
    ofstream fileTugas("data.txt", ios::app);
    fileTugas << hasilGabung << endl;
    fileTugas.close();

    cout << endl
         << "tugas berhasil ditambahkan ke dalam file!" << endl
         << endl;
}

// Vasya
// Fungsi untuk menampilkan daftar tugas dalam format tabel
void lihatDaftarTugas()
{
    Tugas *daftarTugas = muatData();
    int jumlahTugas = hitungBarisFile("data.txt");

    // Header tabel
    cout << tambahkanSpasi("ID", 5)
         << tambahkanSpasi("Nama Tugas", 25)
         << tambahkanSpasi("Mata Kuliah", 20)
         << tambahkanSpasi("Tenggat Waktu", 15)
         << tambahkanSpasi("Prioritas", 10)
         << tambahkanSpasi("Status", 15) << endl;

    cout << string(90, '-') << endl;

    // Isi tabel
    for (int i = 0; i < jumlahTugas; i++)
    {
        cout << tambahkanSpasi(to_string(daftarTugas[i].idTugas), 5)
             << tambahkanSpasi(daftarTugas[i].namaTugas, 25)
             << tambahkanSpasi(daftarTugas[i].mataKuliah, 20)
             << tambahkanSpasi(daftarTugas[i].tenggatWaktu, 15)
             << tambahkanSpasi(intKePrioritasTugas(daftarTugas[i].prioritas), 10)
             << tambahkanSpasi(boolKeStatusTugas(daftarTugas[i].selesai), 15) << endl;
    }

    cout << endl;
}

// Hilmi
void editTugas()
{
}

// Fungsi untuk menghapus elemen dari array berdasarkan indeks
void hapusTugasByIndex(Tugas *daftarTugas, int jumlahTugas, int index)
{
    // Geser elemen setelah indeks ke depan agar elemen pada indeks tertimpa
    for (int i = index; i < jumlahTugas - 1; i++)
    {
        daftarTugas[i] = daftarTugas[i + 1];
    }

    // Kurangi panjang array karena satu tugas sudah ditimpa/dihapus
    jumlahTugas--;

    // Update ID tugas agar kembali berurutan
    for (int i = 0; i < jumlahTugas; i++)
    {
        daftarTugas[i].idTugas = i + 1;
    }
}

void hapusTugas()
{
    string idTugasStr, konfirmasi;
    int idTugas, indexTugas = -1, jumlahTugas = hitungBarisFile("data.txt");
    Tugas *daftarTugas;

    if (jumlahTugas > 0)
    {
        while (indexTugas == -1)
        {
            cout << "Masukkan ID tugas: ";
            getline(cin, idTugasStr);

            if (!cekNumerik(idTugasStr))
            {
                cout << "ID tugas harus berupa bilangan bulat!" << endl;
                continue;
            }

            idTugas = stoi(idTugasStr);
            daftarTugas = muatData();
            indexTugas = cariIndexTugas(daftarTugas, jumlahTugas, idTugas);

            if (indexTugas == -1)
                cout << "Tugas dengan ID " << idTugas << " tidak ditemukan!" << endl;
        }

        cout << "Apakah Anda yakin ingin menghapus tugas dengan ID " << idTugas << "?" << endl;
        cout << "Ketik YA bila yakin: ";
        getline(cin, konfirmasi);

        if (konfirmasi == "YA")
        {
            hapusTugasByIndex(daftarTugas, jumlahTugas, indexTugas);
            simpanData(daftarTugas, jumlahTugas - 1);
            cout << "Tugas berhasil dihapus!" << endl;
        }
        else
        {
            cout << "Penghapusan tugas berhasil dibatalkan!" << endl;
        }
    }
    else
    {
        cout << "Daftar tugas masih kosong!" << endl;
    }
}

// Hilmi
void tandaiSelesai() {}

void simpanDanKeluar()
{
    cout << "Berhasil menyimpan dan keluar!" << endl;
    cout << "Terima kasih telah menggunakan program Pengelolaan Tugas Kuliah!" << endl;
}

void pesanPilihanError(int &pilihan)
{
    cout << "Error: Nomor " << pilihan << " tidak ada pada opsi, mohon pilih ulang!" << endl;
}

int main()
{
    string pilihanStr;
    int pilihan;

    do
    {
        menuUtama();
        getline(cin, pilihanStr);
        pilihan = stoi(pilihanStr); // stoi = string to integer

        switch (pilihan)
        {
        case 1:
            tambahTugas();
            break;
        case 2:
            lihatDaftarTugas();
            break;
        case 3:
            editTugas();
            break;
        case 4:
            hapusTugas();
            break;
        case 5:
            tandaiSelesai();
            break;
        case 6:
            simpanDanKeluar();
            break;
        default:
            pesanPilihanError(pilihan);
            break;
        }
    } while (pilihan != 6);

    return 0;
}
