#include <iostream>
#include <string>
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
    if (prioritas < 1 || prioritas > 3)
        return "Tidak Diketahui";
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
bool cekPilihan(string pilihan, int opsiMin, int opsiMax)
{
    if (!cekNumerik(pilihan))
        return false;

    int pil = stoi(pilihan);

    return (pil >= opsiMin && pil <= opsiMax);
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
        temp += intKePrioritasTugas(daftarTugas[i].prioritas) + "|";
        temp += boolKeStatusTugas(daftarTugas[i].selesai) + "\n";
        data += temp;
    }

    fileData << data;
}

// Fungsi untuk mengecek apakah suatu string legal atau tidak terdapat karakter "|"
bool inputLegal(string input)
{
    return (input.find('|') == string::npos);
}

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
        int opsiMin = 1, opsiMax = 3;

        if (!cekPilihan(prioritas, opsiMin, opsiMax))
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

// Fungsi untuk menyortir array Tugas berdasarkan ID
void sortirTugasBerdasarkanID(Tugas *daftarTugas, int jumlahTugas)
{
    // Bubble Sort untuk menyortir berdasarkan ID
    for (int i = 0; i < jumlahTugas - 1; i++)
    {
        for (int j = 0; j < jumlahTugas - i - 1; j++)
        {
            if (daftarTugas[j].idTugas > daftarTugas[j + 1].idTugas)
            {
                // Tukar elemen jika ID j lebih besar daripada j+1
                Tugas temp = daftarTugas[j];
                daftarTugas[j] = daftarTugas[j + 1];
                daftarTugas[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk memecah string tanggal menjadi hari, bulan, dan tahun
void pecahTanggal(const string tanggal, int &hari, int &bulan, int &tahun)
{
    hari = stoi(tanggal.substr(0, 2));  // Ambil DD
    bulan = stoi(tanggal.substr(3, 2)); // Ambil MM
    tahun = stoi(tanggal.substr(6, 4)); // Ambil YYYY
}

// Fungsi untuk membandingkan dua tanggal dalam format DD/MM/YYYY
bool tglLebihAwal(const string tanggal1, const string tanggal2)
{
    int hari1, bulan1, tahun1;
    int hari2, bulan2, tahun2;

    // Pecah kedua tanggal menjadi komponen hari, bulan, dan tahun
    pecahTanggal(tanggal1, hari1, bulan1, tahun1);
    pecahTanggal(tanggal2, hari2, bulan2, tahun2);

    // Perbandingan hierarkis
    if (tahun1 != tahun2)
    {
        return tahun1 < tahun2; // Tahun lebih kecil lebih awal
    }
    else if (bulan1 != bulan2)
    {
        return bulan1 < bulan2; // Bulan lebih kecil lebih awal
    }
    else
    {
        return hari1 < hari2; // Hari lebih kecil lebih awal
    }
}

// Fungsi untuk menyortir array Tugas berdasarkan tenggat waktu
void sortirTugasBerdasarkanTanggal(Tugas *daftarTugas, int jumlahTugas)
{
    // Bubble Sort untuk menyortir berdasarkan tanggal
    for (int i = 0; i < jumlahTugas - 1; i++)
    {
        for (int j = 0; j < jumlahTugas - i - 1; j++)
        {
            if (!tglLebihAwal(daftarTugas[j].tenggatWaktu, daftarTugas[j + 1].tenggatWaktu))
            {
                // Tukar elemen jika tanggal j lebih lambat daripada j+1
                Tugas temp = daftarTugas[j];
                daftarTugas[j] = daftarTugas[j + 1];
                daftarTugas[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk menyortir array Tugas berdasarkan prioritas
void sortirTugasBerdasarkanPrioritas(Tugas *daftarTugas, int jumlahTugas)
{
    // Bubble Sort untuk menyortir berdasarkan prioritas
    for (int i = 0; i < jumlahTugas - 1; i++)
    {
        for (int j = 0; j < jumlahTugas - i - 1; j++)
        {
            if (daftarTugas[j].prioritas > daftarTugas[j + 1].prioritas)
            {
                // Tukar elemen jika prioritas j lebih besar daripada j+1
                Tugas temp = daftarTugas[j];
                daftarTugas[j] = daftarTugas[j + 1];
                daftarTugas[j + 1] = temp;
            }
        }
    }
}

void sortirTugas(Tugas *daftarTugas, int jumlahTugas, int sortir)
{
    switch (sortir)
    {
    case 1:
        sortirTugasBerdasarkanID(daftarTugas, jumlahTugas);
        break;
    case 2:
        sortirTugasBerdasarkanTanggal(daftarTugas, jumlahTugas);
        break;
    case 3:
        sortirTugasBerdasarkanPrioritas(daftarTugas, jumlahTugas);
        break;
    }
}

void menuSortirTugas(int sortir)
{
    cout << "Sortir tugas berdasarkan:" << endl
         << "1. ID (default)" << endl
         << "2. Tenggat Waktu" << endl
         << "3. Prioritas" << endl
         << "Pilihan saat ini: " << sortir << endl
         << endl
         << "Pilih 0 untuk kembali ke menu utama" << endl
         << endl
         << "Pilih opsi (1/2/3/0): ";
}

void tabelTugas(Tugas *daftarTugas, int jumlahTugas, int sortir = 1)
{
    // Sortir urutan tugas sebelum ditampilkan
    sortirTugas(daftarTugas, jumlahTugas, sortir);

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

void tabelSatuTugas(Tugas *daftarTugas, int indexTugas)
{
    int jumlahTugas = 1;
    Tugas daftarTugasBaru[jumlahTugas] = {
        {
            daftarTugas[indexTugas].idTugas,
            daftarTugas[indexTugas].namaTugas,
            daftarTugas[indexTugas].mataKuliah,
            daftarTugas[indexTugas].tenggatWaktu,
            daftarTugas[indexTugas].prioritas,
            daftarTugas[indexTugas].selesai,
        }};

    tabelTugas(daftarTugasBaru, jumlahTugas);
}

// Fungsi untuk menampilkan daftar tugas dalam format tabel
void lihatDaftarTugas()
{
    Tugas *daftarTugas = muatData();
    int jumlahTugas = hitungBarisFile("data.txt");
    int pilihan, opsiMin = 0, opsiMax = 3;
    string pilihanStr;
    bool pilihanValid;

    /*
    sortir = 1 --> Sortir berdasarkan ID (default)
    sortir = 2 --> Sortir berdasarkan tenggat waktu
    sortir = 3 --> Sortir berdasarkan prioritas
     */
    int sortir = 1;
    while (true)
    {
        tabelTugas(daftarTugas, jumlahTugas, sortir);
        menuSortirTugas(sortir);
        getline(cin, pilihanStr);
        pilihanValid = cekPilihan(pilihanStr, opsiMin, opsiMax);

        if (!pilihanValid)
        {
            cout << "Nomor pilihan " << pilihanStr << " tidak ada pada opsi, mohon pilih ulang!" << endl;
            continue;
        }

        pilihan = stoi(pilihanStr);

        if (pilihan == 0)
            break;

        sortir = pilihan;
    }
}

void menuUbahIsiTugas()
{
    cout << "Pilih yang ingin diubah: " << endl
         << "1. Nama Tugas" << endl
         << "2. Mata Kuliah" << endl
         << "3. Tenggat Waktu" << endl
         << "4. Prioritas" << endl
         << endl
         << "Pilih 0 untuk membatalkan dan kembali ke menu utama" << endl
         << endl
         << "Pilih opsi (1/2/3/4/0): ";
}

void ubahIsiTugas(Tugas *daftarTugas, int indexTugas, int pilihan)
{
    string daftarInstruksi[4] = {
        "Masukkan nama tugas baru",
        "Masukkan mata kuliah baru",
        "Masukkan tenggat waktu baru (DD/MM/YYYY)",
        "Masukkan prioritas baru (1:tinggi/2:sedang/3:rendah)",
    };
    string input;
    bool loop = true;

    while (loop)
    {
        cout << daftarInstruksi[pilihan - 1];
        cout << " (kosongi untuk membatalkan): ";
        getline(cin, input);
        cout << endl;

        if (empty(input))
        {
            cout << "Edit tugas berhasil dibatalkan!" << endl;
            break;
        };

        switch (pilihan)
        {
        case 1:
            if (!inputLegal(input))
            {
                cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
                break;
            }
            daftarTugas[indexTugas].namaTugas = input;
            loop = false;
            break;
        case 2:
            if (!inputLegal(input))
            {
                cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
                break;
            }
            daftarTugas[indexTugas].mataKuliah = input;
            loop = false;
            break;
        case 3:
            if (!formatTanggalValid(input))
            {
                cout << "Format tanggal tidak valid!" << endl;
                break;
            }
            daftarTugas[indexTugas].tenggatWaktu = input;
            loop = false;
            break;
        case 4:
            int opsiMin = 1, opsiMax = 3;
            if (!cekPilihan(input, opsiMin, opsiMax))
            {
                cout << "Pilihan " << input << " tidak ada pada opsi!" << endl;
                break;
            }
            daftarTugas[indexTugas].prioritas = stoi(input);
            loop = false;
            break;
        }
    }
}

void editTugas()
{
    int idTugas, jumlahTugas, indexTugas, pilihan, opsiPilihanMin = 0, opsiPilihanMax = 4;
    string idTugasStr, pilihanStr;
    bool idTugasValid, pilihanValid;
    Tugas *daftarTugas;

    while (true)
    {
        cout << "Masukkan ID tugas: ";
        getline(cin, idTugasStr);
        idTugasValid = cekNumerik(idTugasStr);

        if (!idTugasValid)
        {
            cout << "ID tugas tidak valid, mohon masukkan ulang!" << endl;
            continue;
        }

        idTugas = stoi(idTugasStr);
        daftarTugas = muatData();
        jumlahTugas = hitungBarisFile("data.txt");
        indexTugas = cariIndexTugas(daftarTugas, jumlahTugas, idTugas);

        if (indexTugas == -1)
        {
            cout << "Tugas dengan ID " << idTugas << " tidak ditemukan!" << endl;
            continue;
        }

        break;
    }

    tabelSatuTugas(daftarTugas, indexTugas);

    while (true)
    {
        menuUbahIsiTugas();
        getline(cin, pilihanStr);
        pilihanValid = cekPilihan(pilihanStr, opsiPilihanMin, opsiPilihanMax);

        if (!pilihanValid)
        {
            cout << "Pilihan " << pilihanStr << " tidak ada pada opsi!" << endl;
            continue;
        }

        pilihan = stoi(pilihanStr);
        break;
    }

    if (pilihan != 0)
    {
        ubahIsiTugas(daftarTugas, indexTugas, pilihan);
        simpanData(daftarTugas, jumlahTugas);
        cout << "Tugas dengan ID " << idTugas << " berhasil diedit!" << endl;
    }
}

// Fungsi untuk menghapus elemen dari array berdasarkan indeks
void hapusTugasByIndex(Tugas *daftarTugas, int &jumlahTugas, int index)
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
            simpanData(daftarTugas, jumlahTugas);
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

void tandaiSelesai()
{
    int idTugas, indexTugas, jumlahTugas;
    bool selesai;
    string idTugasStr;
    Tugas *daftarTugas;

    while (true)
    {
        cout << "Masukkan ID tugas: ";
        getline(cin, idTugasStr);
        if (!cekNumerik(idTugasStr))
        {
            cout << "ID tugas tidak valid!" << endl;
            continue;
        }
        idTugas = stoi(idTugasStr);
        daftarTugas = muatData();
        jumlahTugas = hitungBarisFile("data.txt");
        indexTugas = cariIndexTugas(daftarTugas, jumlahTugas, idTugas);

        if (indexTugas == -1)
        {
            cout << "Tugas dengan ID " << idTugas << " tidak ditemukan!" << endl;
            continue;
        }
        break;
    }
    selesai = !(daftarTugas[indexTugas].selesai);
    daftarTugas[indexTugas].selesai = selesai;
    simpanData(daftarTugas, jumlahTugas);

    cout << "Status tugas dengan ID " << idTugas << " berhasil diubah menjadi " << boolKeStatusTugas(selesai) << "!" << endl;
}

void simpanDanKeluar()
{
    cout << "Berhasil menyimpan dan keluar!" << endl;
    cout << "Terima kasih telah menggunakan program Pengelolaan Tugas Kuliah!" << endl;
}

void pesanPilihanError(int pilihan)
{
    cout << "Error: Nomor " << pilihan << " tidak ada pada opsi, mohon pilih ulang!" << endl;
}

void mulai()
{
    int pilihan;
    string pilihanStr;

    while (true)
    {
        menuUtama();
        getline(cin, pilihanStr);
        if (!cekNumerik(pilihanStr))
        {
            cout << "Pilihan harus berupa bilangan bulat!" << endl
                 << endl;
            continue;
        }
        pilihan = stoi(pilihanStr); // stoi = string to integer
        break;
    }

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

    if (pilihan != 6)
        mulai();
}

int main()
{
    mulai();
    return 0;
}
